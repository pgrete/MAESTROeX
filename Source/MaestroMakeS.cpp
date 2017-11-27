
#include <Maestro.H>

using namespace amrex;

void
Maestro::Make_S_cc (Vector<MultiFab>& S_cc,
                    const Vector<MultiFab>& scal,
                    const Vector<MultiFab>& rho_omegadot,
                    const Vector<MultiFab>& rho_Hnuc,
                    const Vector<MultiFab>& rho_Hext,
                    const Vector<MultiFab>& thermal)
{

    for (int lev=0; lev<=finest_level; ++lev) {

        // get references to the MultiFabs at level lev
        MultiFab& S_cc_mf = S_cc[lev];
        const MultiFab& scal_mf = scal[lev];
        const MultiFab& rho_odot_mf = rho_omegadot[lev];
        const MultiFab& rho_Hnuc_mf = rho_Hnuc[lev];
        const MultiFab& rho_Hext_mf = rho_Hext[lev];
        const MultiFab& thermal_mf = thermal[lev];

        // Loop over boxes (make sure mfi takes a cell-centered multifab as an argument)
        for ( MFIter mfi(S_cc_mf); mfi.isValid(); ++mfi ) {

            // Get the index space of the valid region
            const Box& validBox = mfi.validbox();

            // call fortran subroutine
            // use macros in AMReX_ArrayLim.H to pass in each FAB's data, 
            // lo/hi coordinates (including ghost cells), and/or the # of components
            // We will also pass "validBox", which specifies the "valid" region.
            make_S_cc(ARLIM_3D(validBox.loVect()), ARLIM_3D(validBox.hiVect()),
                      BL_TO_FORTRAN_3D(S_cc_mf[mfi]),
                      BL_TO_FORTRAN_FAB(scal_mf[mfi]),
                      BL_TO_FORTRAN_FAB(rho_odot_mf[mfi]),
                      BL_TO_FORTRAN_3D(rho_Hnuc_mf[mfi]),
                      BL_TO_FORTRAN_3D(rho_Hext_mf[mfi]),
                      BL_TO_FORTRAN_3D(thermal_mf[mfi]));
        }

    }

    for (int lev=finest_level-1; lev>=0; --lev)
    {
        AverageDownTo(lev,S_cc,0,1); // average lev+1 down to lev
    }

}


void
Maestro::Make_NodalRHS (const Vector<MultiFab>& S_cc,
                        Vector<MultiFab>& nodalrhs,
                        const Vector<Real>& Sbar,
                        const Vector<Real>& beta0)
{

    Vector<MultiFab> ccrhs(finest_level+1);

    for (int lev=0; lev<=finest_level; ++lev) {

        // build ccrhs
        ccrhs[lev].define(grids[lev], dmap[lev], 1, 1);

        // fill ccrhs
        // get references to the MultiFabs at level lev
              MultiFab& ccrhs_mf = ccrhs[lev];
        const MultiFab& S_cc_mf = S_cc[lev];

        // Loop over boxes (make sure mfi takes a cell-centered multifab as an argument)
        for ( MFIter mfi(S_cc_mf); mfi.isValid(); ++mfi ) {

            // Get the index space of the valid region
            const Box& validBox = mfi.validbox();

            // call fortran subroutine
            // use macros in AMReX_ArrayLim.H to pass in each FAB's data, 
            // lo/hi coordinates (including ghost cells), and/or the # of components
            // We will also pass "validBox", which specifies the "valid" region.
            make_ccrhs(lev, ARLIM_3D(validBox.loVect()), ARLIM_3D(validBox.hiVect()),
                       BL_TO_FORTRAN_3D(ccrhs_mf[mfi]),
                       BL_TO_FORTRAN_3D(S_cc_mf[mfi]),
                       Sbar.dataPtr(), beta0.dataPtr());
        }
    }

    // fill ghost cells using first-order extrapolation
    for (int lev=0; lev<=finest_level; ++lev) {
        FillPatch(lev, t_old, ccrhs[lev], ccrhs, ccrhs, 0, 1, bcs_f);
    }

    // make_nodalrhs
    for (int lev=0; lev<=finest_level; ++lev) {

        // get references to the MultiFabs at level lev
              MultiFab& nodalrhs_mf = nodalrhs[lev];
        const MultiFab& ccrhs_mf = ccrhs[lev];

        // Loop over boxes (make sure mfi takes a cell-centered multifab as an argument)
        for ( MFIter mfi(ccrhs_mf); mfi.isValid(); ++mfi ) {

            // Get the index space of the valid region
            const Box& validBox = mfi.validbox();

            // call fortran subroutine
            // use macros in AMReX_ArrayLim.H to pass in each FAB's data, 
            // lo/hi coordinates (including ghost cells), and/or the # of components
            // We will also pass "validBox", which specifies the "valid" region.
            make_nodalrhs(lev, ARLIM_3D(validBox.loVect()), ARLIM_3D(validBox.hiVect()),
                          BL_TO_FORTRAN_3D(nodalrhs_mf[mfi]),
                          BL_TO_FORTRAN_3D(ccrhs_mf[mfi]));
        }
    }
}
