
#include <Maestro.H>

using namespace amrex;

// advance solution to final time
void
Maestro::Evolve ()
{

	init_base_state(s0_init.dataPtr(),p0_init.dataPtr(),rho0_old.dataPtr(),
	                rhoh0_old.dataPtr(),p0_old.dataPtr(),tempbar.dataPtr(),
	                tempbar_init.dataPtr());

	InitFromScratch(0.0);

	Vector<MultiFab> phi(finest_level+1);
	Vector<Real> phi_exact((max_radial_level+1)*nr_fine);
	Vector<Real> phi_avg((max_radial_level+1)*nr_fine);

	Vector<Real> error(nr_fine);

	for (int lev=0; lev<=finest_level; ++lev) {
		phi[lev].define(grids[lev], dmap[lev], 1, 0);
		phi[lev].setVal(0.);
	}

	std::fill(phi_exact.begin(), phi_exact.end(), 0.);
	std::fill(phi_avg.begin(), phi_avg.end(), 0.);
	std::fill(error.begin(), error.end(), 0.);

	std::copy(p0_old.begin(), p0_old.end(), phi_exact.begin());

	Print() << "Putting 1d array on Cartesian" << std::endl;

	Put1dArrayOnCart(phi_exact, phi, 0, 0);

	Average(phi, phi_avg, 0);

	// Compare the initial and final phi
	for (int lev=0; lev<=max_radial_level; ++lev) {
		for (int r=0; r < nr_fine; ++r) {
			int idx = lev*nr_fine + r;
			error[r] = phi_exact[idx] - phi_avg[idx];

			Real abs_norm = error[r];

			Real rel_norm = (phi_exact[idx] == 0.0) ? 0.0 : error[r] / phi_exact[idx];

			Print() << "\tPhi = " << phi_exact[idx] << ",   Abs norm = " << abs_norm << ",  Rel norm = " << rel_norm << std::endl;
		}

	}

}
