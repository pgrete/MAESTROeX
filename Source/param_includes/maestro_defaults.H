
// This file is automatically created by parse_maestro_params.py.  To update
// or add runtime parameters, please edit _cpp_parameters and then run
// mk_params.sh

int         Maestro::reflux_type = 1;
int         Maestro::maestro_verbose = 1;
std::string Maestro::model_file = "";
bool        Maestro::perturb_model = false;
bool        Maestro::print_init_hse_diag = false;
amrex::Real Maestro::stop_time = -1.0;
int         Maestro::max_step = 1;
amrex::Real Maestro::cfl = 0.5;
amrex::Real Maestro::init_shrink = 1.0;
amrex::Real Maestro::small_dt = 1.e-10;
amrex::Real Maestro::max_dt_growth = 1.1;
amrex::Real Maestro::max_dt = 1.e33;
amrex::Real Maestro::fixed_dt = -1.0;
amrex::Real Maestro::nuclear_dt_fac = -1.0;
bool        Maestro::use_soundspeed_firstdt = false;
bool        Maestro::use_divu_firstdt = false;
int         Maestro::spherical = 0;
bool        Maestro::octant = false;
int         Maestro::do_2d_planar_octant = 0;
int         Maestro::regrid_int = -1;
int         Maestro::amr_buf_width = -1;
int         Maestro::drdxfac = 1;
int         Maestro::minwidth = 8;
amrex::Real Maestro::min_eff = 0.9;
bool        Maestro::use_tpert_in_tagging = false;
int         Maestro::plot_int = 0;
int         Maestro::small_plot_int = 0;
amrex::Real Maestro::plot_deltat = -1.0;
amrex::Real Maestro::small_plot_deltat = -1.0;
int         Maestro::chk_int = 0;
amrex::Real Maestro::chk_deltat = -1.0;
bool        Maestro::plot_h_with_use_tfromp = true;
bool        Maestro::plot_spec = true;
bool        Maestro::plot_omegadot = true;
bool        Maestro::plot_Hext = false;
bool        Maestro::plot_Hnuc = true;
bool        Maestro::plot_eta = false;
bool        Maestro::plot_trac = false;
bool        Maestro::plot_base_state = true;
bool        Maestro::plot_gpi = true;
bool        Maestro::plot_cs = false;
bool        Maestro::plot_grav = false;
std::string Maestro::plot_base_name = "plt";
std::string Maestro::small_plot_base_name = "smallplt";
std::string Maestro::check_base_name = "chk";
int         Maestro::diag_buf_size = 10;
bool        Maestro::plot_ad_excess = false;
bool        Maestro::plot_processors = false;
bool        Maestro::plot_pidivu = false;
std::string Maestro::small_plot_vars = "rho p0 magvel";
int         Maestro::init_iter = 4;
int         Maestro::init_divu_iter = 4;
std::string Maestro::restart_file = "";
bool        Maestro::restart_into_finer = false;
bool        Maestro::do_initial_projection = true;
int         Maestro::mg_verbose = 1;
int         Maestro::cg_verbose = 0;
int         Maestro::mg_cycle_type = 3;
int         Maestro::hg_cycle_type = 3;
int         Maestro::hg_bottom_solver = -1;
int         Maestro::mg_bottom_solver = -1;
int         Maestro::max_mg_bottom_nlevels = 1000;
int         Maestro::mg_bottom_nu = 10;
int         Maestro::mg_nu_1 = 2;
int         Maestro::mg_nu_2 = 2;
bool        Maestro::hg_dense_stencil = true;
bool        Maestro::do_sponge = false;
amrex::Real Maestro::sponge_kappa = 10.e0;
amrex::Real Maestro::sponge_center_density = 3.e6;
amrex::Real Maestro::sponge_start_factor = 3.333e0;
bool        Maestro::plot_sponge_fdamp = false;
amrex::Real Maestro::anelastic_cutoff_density = -1.0;
amrex::Real Maestro::base_cutoff_density = -1.0;
amrex::Real Maestro::burning_cutoff_density = -1.0;
bool        Maestro::burning_invert_cutoff_density = false;
amrex::Real Maestro::buoyancy_cutoff_factor = 5.0;
amrex::Real Maestro::dpdt_factor = 0.0;
bool        Maestro::do_planar_invsq_grav = false;
amrex::Real Maestro::planar_invsq_mass = 0.0;
bool        Maestro::evolve_base_state = true;
bool        Maestro::use_exact_base_state = false;
bool        Maestro::fix_base_state = false;
bool        Maestro::average_base_state = false;
bool        Maestro::do_smallscale = false;
bool        Maestro::do_eos_h_above_cutoff = true;
int         Maestro::enthalpy_pred_type = 1;
int         Maestro::species_pred_type = 1;
bool        Maestro::use_delta_gamma1_term = true;
bool        Maestro::use_etarho = true;
int         Maestro::slope_order = 4;
amrex::Real Maestro::grav_const = -1.5e10;
int         Maestro::ppm_type = 1;
int         Maestro::bds_type = 0;
int         Maestro::ppm_trace_forces = 0;
int         Maestro::beta0_type = 1;
bool        Maestro::use_linear_grav_in_beta0 = false;
amrex::Real Maestro::rotational_frequency = 0.0;
amrex::Real Maestro::co_latitude = 0.0;
amrex::Real Maestro::rotation_radius = 1.0e6;
amrex::Real Maestro::mach_max_abort = -1.0;
bool        Maestro::drive_initial_convection = false;
int         Maestro::stop_initial_convection = -1;
bool        Maestro::restart_with_vel_field = false;
bool        Maestro::use_alt_energy_fix = true;
bool        Maestro::use_thermal_diffusion = false;
int         Maestro::temp_diffusion_formulation = 2;
int         Maestro::thermal_diffusion_type = 1;
bool        Maestro::limit_conductivity = false;
bool        Maestro::do_burning = true;
std::string Maestro::burner_threshold_species = "";
amrex::Real Maestro::burner_threshold_cutoff = 1.e-10;
bool        Maestro::do_subgrid_burning = false;
amrex::Real Maestro::reaction_sum_tol = 1.e-10;
amrex::Real Maestro::small_temp = 5.e6;
amrex::Real Maestro::small_dens = 1.e-5;
bool        Maestro::use_tfromp = false;
bool        Maestro::use_eos_e_instead_of_h = false;
bool        Maestro::use_pprime_in_tfromp = false;
int         Maestro::s0_interp_type = 3;
int         Maestro::w0_interp_type = 2;
int         Maestro::s0mac_interp_type = 1;
int         Maestro::w0mac_interp_type = 1;
#ifdef AMREX_DEBUG
int         Maestro::print_fortran_warnings = 1;
#else
int         Maestro::print_fortran_warnings = 0;
#endif
#ifdef AMREX_DEBUG
int         Maestro::print_update_diagnostics = 1;
#else
int         Maestro::print_update_diagnostics = 0;
#endif
int         Maestro::track_grid_losses = 0;
int         Maestro::sum_interval = -1;
amrex::Real Maestro::sum_per = -1.0e0;
int         Maestro::show_center_of_mass = 0;
int         Maestro::hard_cfl_limit = 1;
std::string Maestro::job_name = "";
int         Maestro::output_at_completion = 1;
amrex::Real Maestro::reset_checkpoint_time = -1.e200;
int         Maestro::reset_checkpoint_step = -1;
bool        Maestro::use_particles = false;
bool        Maestro::store_particle_vels = false;
bool        Maestro::do_heating = false;
int         Maestro::sdc_iters = 1;
bool        Maestro::sdc_couple_mac_velocity = false;
amrex::Real Maestro::eps_init_proj_cart = 1.e-12;
amrex::Real Maestro::eps_init_proj_sph = 1.e-10;
amrex::Real Maestro::eps_divu_cart = 1.e-12;
amrex::Real Maestro::eps_divu_sph = 1.e-10;
amrex::Real Maestro::divu_iter_factor = 100.;
amrex::Real Maestro::divu_level_factor = 10.;
amrex::Real Maestro::eps_mac = 1.e-10;
amrex::Real Maestro::eps_mac_max = 1.e-8;
amrex::Real Maestro::mac_level_factor = 10.;
amrex::Real Maestro::eps_mac_bottom = 1.e-3;
amrex::Real Maestro::eps_hg = 1.e-12;
amrex::Real Maestro::eps_hg_max = 1.e-10;
amrex::Real Maestro::hg_level_factor = 10.;
amrex::Real Maestro::eps_hg_bottom = 1.e-4;
