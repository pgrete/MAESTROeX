
module initdata_module

  use network, only: nspec
  use amrex_error_module
  use amrex_constants_module
  use amrex_fort_module, only : amrex_spacedim
  use base_state_geometry_module, only: nr_fine, max_radial_level
  use meth_params_module, only: nscal, rho_comp, rhoh_comp, temp_comp, &
       spec_comp, pi_comp, prob_lo, prob_hi
  use probin_module, only: velpert_height_loc, num_vortices, apply_vel_field, &
       velpert_scale, velpert_amplitude

  implicit none

  private

contains

  subroutine initdata(lev, time, lo, hi, &
       scal, scal_lo, scal_hi, nc_s, &
       vel, vel_lo, vel_hi, nc_v, &
       s0_init, p0_init, dx) bind(C, name="initdata")

    integer         , intent(in   ) :: lev, lo(3), hi(3)
    integer         , intent(in   ) :: scal_lo(3), scal_hi(3), nc_s
    integer         , intent(in   ) :: vel_lo(3), vel_hi(3), nc_v
    double precision, intent(in   ) :: time
    double precision, intent(inout) :: scal(scal_lo(1):scal_hi(1), &
         scal_lo(2):scal_hi(2), &
         scal_lo(3):scal_hi(3), 1:nc_s)
    double precision, intent(inout) :: vel(vel_lo(1):vel_hi(1), &
         vel_lo(2):vel_hi(2), &
         vel_lo(3):vel_hi(3), 1:nc_v)
    double precision, intent(in   ) :: s0_init(0:max_radial_level,0:nr_fine-1,1:nscal)
    double precision, intent(in   ) :: p0_init(0:max_radial_level,0:nr_fine-1)
    double precision, intent(in   ) :: dx(3)

    integer          :: i,j,k,r,vortex
    double precision :: xdist, ydist, rr, xloc(3), upert(3)
    double precision :: xloc_vortices(num_vortices), offset

    offset = (prob_hi(1) - prob_lo(1)) / num_vortices

    do i = 1, num_vortices
       xloc_vortices(i) = (dble(i-1) + 0.5d0) * offset + prob_lo(1)
    enddo

    do k=lo(3),hi(3)
       do j=lo(2),hi(2)
          do i=lo(1),hi(1)

             if (amrex_spacedim .eq. 1) then
                r = i
             else if (amrex_spacedim .eq. 2) then
                r = j
             else if (amrex_spacedim .eq. 3) then
                r = k
             end if

             ! set scalars using s0
             scal(i,j,k,rho_comp)  = s0_init(lev,r,rho_comp)
             scal(i,j,k,rhoh_comp) = s0_init(lev,r,rhoh_comp)
             scal(i,j,k,temp_comp) = s0_init(lev,r,temp_comp)
             scal(i,j,k,spec_comp:spec_comp+nspec-1) = &
                  s0_init(lev,r,spec_comp:spec_comp+nspec-1)

             ! initialize pi to zero for now
             scal(i,j,k,pi_comp) = 0.d0

          end do
       end do
    end do

    ! set velocity to zero
    vel(vel_lo(1):vel_hi(1),vel_lo(2):vel_hi(2),vel_lo(3):vel_hi(3),1:nc_v) = 0.d0

    if (apply_vel_field) then

       if (amrex_spacedim .eq. 3) call amrex_error("apply_vel_field not supported for 3d")

       do k=lo(3),hi(3)

          xloc(3) = prob_lo(3) + (dble(k) + HALF) * dx(3)

          do j=lo(2),hi(2)

             xloc(2) = prob_lo(2) + (dble(j) + HALF) * dx(2)
             ydist = xloc(2) - velpert_height_loc

             do i=lo(1),hi(1)

                upert(:) = ZERO

                xloc(1) = prob_lo(1) + (dble(i) + HALF) * dx(1)

                do vortex = 1, num_vortices

                   xdist = xloc(1) - xloc_vortices(vortex)

                   rr = sqrt(xdist**2 + ydist**2)

                   upert(1) = upert(1) - (ydist/velpert_scale) * &
                        velpert_amplitude * exp( -rr**2/(TWO*velpert_scale**2)) &
                        * (-ONE)**vortex

                   upert(2) = upert(2) + (xdist/velpert_scale) * &
                        velpert_amplitude * exp(-rr**2/(TWO*velpert_scale**2)) &
                        * (-ONE)**vortex

                end do

                vel(i,j,k,:) = vel(i,j,k,:) + upert(:)

             end do
          end do
       end do

    endif


  end subroutine initdata

  subroutine initdata_sphr(time, lo, hi, &
       scal, scal_lo, scal_hi, nc_s, &
       vel, vel_lo, vel_hi, nc_v, &
       s0_init, p0_init, &
       dx, r_cc_loc, r_edge_loc, &
       cc_to_r, ccr_lo, ccr_hi) bind(C, name="initdata_sphr")

    integer         , intent(in   ) :: lo(3), hi(3)
    integer         , intent(in   ) :: scal_lo(3), scal_hi(3), nc_s
    integer         , intent(in   ) :: vel_lo(3), vel_hi(3), nc_v
    integer         , intent(in   ) :: ccr_lo(3), ccr_hi(3)
    double precision, intent(in   ) :: time
    double precision, intent(inout) :: scal(scal_lo(1):scal_hi(1), &
         scal_lo(2):scal_hi(2), &
         scal_lo(3):scal_hi(3), nc_s)
    double precision, intent(inout) :: vel(vel_lo(1):vel_hi(1), &
         vel_lo(2):vel_hi(2), &
         vel_lo(3):vel_hi(3), nc_v)
    double precision, intent(in   ) :: s0_init(0:max_radial_level,0:nr_fine-1,1:nscal)
    double precision, intent(in   ) :: p0_init(0:max_radial_level,0:nr_fine-1)
    double precision, intent(in   ) :: dx(3)
    double precision, intent(in   ) :: r_cc_loc  (0:max_radial_level,0:nr_fine-1)
    double precision, intent(in   ) :: r_edge_loc(0:max_radial_level,0:nr_fine)
    double precision, intent(in   ) :: cc_to_r   (ccr_lo(1):ccr_hi(1), &
         ccr_lo(2):ccr_hi(2),ccr_lo(3):ccr_hi(3))

  end subroutine initdata_sphr

end module initdata_module
