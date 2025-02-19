module make_analytic_solution_module

  use amrex_error_module
  use amrex_constants_module
  use base_state_geometry_module, only: center
  use probin_module, only: ambient_h, ambient_dens, &
       t0, peak_h, diffusion_coefficient
  use meth_params_module, only: prob_lo

  implicit none

  private

contains

  subroutine make_analytic_solution(lo, hi, solution, s_lo, s_hi, dx, time) &
       bind(C, name="make_analytic_solution")

    integer, intent(in) :: lo(3), hi(3), s_lo(3), s_hi(3)
    double precision, intent(inout) :: solution(s_lo(1):s_hi(1),s_lo(2):s_hi(2),s_lo(3):s_hi(3))
    double precision, intent(in   ) :: dx(3)
    double precision, intent(in   ), value :: time

    integer :: n, i, j, k
    double precision :: x, y, dist2

    solution(s_lo(1):s_hi(1),s_lo(2):s_hi(2),s_lo(3):s_hi(3)) = ZERO

    do k = lo(3), hi(3)

       do j = lo(2), hi(2)

          y = prob_lo(2) + (dble(j)+HALF) * dx(2) - center(2)

          do i = lo(1), hi(1)

             x = prob_lo(1) + (dble(i)+HALF) * dx(1) - center(1)

             dist2 = x**2 + y**2

             solution(i,j,k) = f(time,dist2)

          enddo
       enddo
    enddo

  end subroutine make_analytic_solution

  function f(t,x) result(r)
    double precision :: t, x
    double precision :: r

    r = (peak_h - ambient_h) * (t0 / (t+t0)) * &
         dexp(-x / (FOUR * diffusion_coefficient * (t+t0))) + ambient_h

  end function f

end module make_analytic_solution_module
