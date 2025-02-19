#!/usr/bin/env python3

# parse the _variables file and write the set of functions that will
# define the indices.

import argparse
import os
import re


def split_pair(pair_string):
    """given an option of the form "(val1, val2)", split it into val1 and
    val2"""
    return pair_string.replace("(", "").replace(")", "").replace(" ", "").split(",")


class Index(object):
    """an index that we want to set"""

    def __init__(self, name, f90_var, default_group=None, iset=None,
                 also_adds_to=None, count=1, cxx_var=None, ifdef=None):
        """ parameters:
               name: a descriptive name for the quantity
               f90_var: name of the variable in Fortran
               default_group: the name of a counter that we increment (e.g., NVAR)
               iset: a descriptive name for the set of the variables this belongs to
                     (e.g., conserved)
               also_adds_to: any other counters that we increment
               count: the number of variables in this group
               cxx_var: the name of the variable in C++
               ifdef: any ifdef that wraps this variable
        """
        self.name = name
        self.cxx_var = cxx_var
        self.f90_var = f90_var
        self.iset = iset
        self.default_group = default_group
        self.adds_to = also_adds_to

        # count may have different names in Fortran and C++
        if count.startswith("("):
            self.count, self.count_cxx = split_pair(count)
        else:
            self.count = count
            self.count_cxx = count

        self.ifdef = ifdef

    def __str__(self):
        return self.f90_var

    def get_set_string(self, val, set_default=None):
        """return the Fortran code that sets this variable index (to val).
        Here set_default is a value to set the key to in the case that
        a string value (like nspec) is 0

        """
        sstr = ""
        if self.ifdef is not None:
            sstr += "#ifdef {}\n".format(self.ifdef)

        if set_default is not None and self.count != "1":
            # this adds a test that the count is greater than 0
            sstr += "  if ({} > 0) then\n".format(self.count)
            sstr += "    {} = {}\n".format(self.f90_var, val)
            sstr += "  else\n"
            sstr += "    {} = {}\n".format(self.f90_var, set_default)
            sstr += "  endif\n"
        else:
            sstr += "  {} = {}\n".format(self.f90_var, val)

        if self.cxx_var is not None:
            sstr += "  call check_equal({},{}_in+1)\n".format(
                self.f90_var, self.cxx_var)
        if self.ifdef is not None:
            sstr += "#endif\n"
        sstr += "\n"
        return sstr

    def get_cxx_set_string(self):
        """get the C++ code that sets the variable index and increments the
        counters"""

        if self.iset == "primitive":
            counter = "qcnt"
        elif self.iset == "godunov":
            counter = "gcnt"
        else:
            counter = "cnt"

        sstr = ""
        if self.ifdef is not None:
            sstr += "#ifdef {}\n".format(self.ifdef)

        if self.count != "1":
            sstr += "  if ({} > 0) {{\n".format(self.count_cxx)
            sstr += "    {} = {};\n".format(self.cxx_var, counter)
            sstr += "    {} += {};\n".format(counter, self.count_cxx)
            sstr += "  }\n"
        else:
            sstr += "  {} = {};\n".format(self.cxx_var, counter)
            sstr += "  {} += {};\n".format(counter, self.count_cxx)

        if self.ifdef is not None:
            sstr += "#endif\n"
        sstr += "\n"
        return sstr


class Counter(object):
    """a simple object to keep track of how many variables there are in a
    set"""

    def __init__(self, name, starting_val=1):
        """name: the name of that counter (this will be used in Fortran)"""

        self.name = name
        self.numeric = starting_val
        self.strings = []

        self.starting_val = starting_val

    def increment(self, value):
        try:
            i = int(value)
        except ValueError:
            self.strings.append(value.strip())
        else:
            self.numeric += i

    def get_value(self, offset=0):
        """return the current value of the counter"""
        if len(self.strings) != 0:
            val = "{} + {}".format(self.numeric - offset,
                                   " + ".join(self.strings))
        else:
            val = "{}".format(self.numeric - offset)

        return val

    def get_set_string(self):
        """return the Fortran needed to set this as a parameter"""
        return "integer, parameter :: {} = {}".format(
            self.name, self.get_value(offset=self.starting_val))


def doit(variables_file, odir, defines):

    # read the file and create a list of indices
    indices = []
    default_set = {}
    with open(variables_file, "r") as f:
        current_set = None
        default_group = None
        for line in f:
            if line.startswith("#") or line.strip() == "":
                continue
            elif line.startswith("@"):
                _, current_set, default_group = line.split()
                default_set[current_set] = default_group
            else:

                # this splits the line into separate fields.  A field is a
                # single word or a pair in parentheses like "(a, b)"
                fields = re.findall(
                    r'[\w\"\+\.-]+|\([\w+\.-]+\s*,\s*[\w\+\.-]+\)', line)

                name = fields[0]
                cxx_var = fields[1]
                f90_var = fields[2]
                adds_to = fields[3]
                count = fields[4]
                ifdef = fields[5]

                # we may be fed a pair of the form (SET, DEFINE),
                # in which case we only add to SET if we define
                # DEFINE
                if adds_to.startswith("("):
                    add_set, define = split_pair(adds_to)
                    if not define in defines:
                        adds_to = None
                    else:
                        adds_to = add_set

                if adds_to == "None":
                    adds_to = None
                if cxx_var == "None":
                    cxx_var = None
                if ifdef == "None":
                    ifdef = None

                indices.append(Index(name, f90_var, default_group=default_group,
                                     iset=current_set, also_adds_to=adds_to,
                                     count=count, cxx_var=cxx_var, ifdef=ifdef))

    # find the set of set names
    unique_sets = set([q.iset for q in indices])

    # we'll keep track of all the counters across all the sets.  This
    # will be used later to write a module that sets parameters with
    # the size of each set
    all_counters = []

    # loop over sets and create the functions
    for s in sorted(unique_sets):

        set_indices = [q for q in indices if q.iset == s]

        # add to
        adds_to = set(
            [q.adds_to for q in set_indices if q.adds_to is not None])

        # initialize the counters
        counter_main = Counter(default_set[s])
        counter_adds = []
        for a in adds_to:
            counter_adds.append(Counter(a))

        # write the lines to set the indices
        for i in set_indices:

            # if this variable has an ifdef, make sure it is in
            # defines, otherwise skip
            if i.ifdef is not None:
                if i.ifdef not in defines:
                    continue
            # increment the counters
            counter_main.increment(i.count)
            if i.adds_to is not None:
                for ca in counter_adds:
                    if ca.name == i.adds_to:
                        ca.increment(i.count)

        # store the counters for later writing
        all_counters += [counter_main]
        all_counters += counter_adds

    # write the module containing the size of the sets
    with open(os.path.join(odir, "../param_includes/state_sizes.f90"), "w") as ss:
        ss.write("module state_sizes_module\n")
        ss.write("   use network, only : nspec\n")
        ss.write("   implicit none\n")
        for ac in all_counters:
            ss.write("   {}\n".format(ac.get_set_string()))
        ss.write("end module state_sizes_module\n")


if __name__ == "__main__":

    # note: you need to put a space at the start of the string
    # that gives defines so that the '-' is not interpreted as
    # an option itself
    # https://stackoverflow.com/questions/16174992/cant-get-argparse-to-read-quoted-string-with-dashes-in-it

    parser = argparse.ArgumentParser()
    parser.add_argument("--odir", type=str, default="",
                        help="output directory")
    parser.add_argument("--defines", type=str, default="",
                        help="preprocessor defines to interpret")

    parser.add_argument("variables_file", type=str, nargs=1,
                        help="input variable definition file")
    args = parser.parse_args()

    if args.odir != "" and not os.path.isdir(args.odir):
        os.makedirs(args.odir)

    doit(args.variables_file[0], args.odir, args.defines)
