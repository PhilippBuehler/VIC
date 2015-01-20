/******************************************************************************
 * @section DESCRIPTION
 *
 * C interface for CESM driver.
 *
 * @section LICENSE
 *
 * The Variable Infiltration Capacity (VIC) macroscale hydrological model
 * Copyright (C) 2014 The Land Surface Hydrology Group, Department of Civil
 * and Environmental Engineering, University of Washington.
 *
 * The VIC model is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

size_t              NF, NR;
size_t              current;
all_vars_struct    *all_vars = NULL;
atmos_data_struct  *atmos = NULL;
dmy_struct         *dmy = NULL;
filenames_struct    filenames;
filep_struct        filep;
domain_struct       global_domain;
global_param_struct global_param;
lake_con_struct     lake_con;
nc_file_struct      nc_hist_file;
nc_var_struct       nc_vars[N_OUTVAR_TYPES];
option_struct       options;
parameters_struct   param;
out_data_struct   **out_data;
save_data_struct   *save_data;
param_set_struct    param_set;
soil_con_struct    *soil_con = NULL;
veg_con_map_struct *veg_con_map = NULL;
veg_con_struct    **veg_con = NULL;
veg_hist_struct   **veg_hist = NULL;
veg_lib_struct    **veg_lib = NULL;

int vic_cesm_init(agrs_from_cesm) {

    filenames.global = "vic.globalconfig.txt";

    // Initialize Log Destination
    initialize_log();

    // read global parameters
    vic_start();

    // allocate memory
    vic_alloc();

    // initialize model parameters from parameter files
    vic_init();

    // restore model state, either using a cold start or from a restart file
    vic_restore();

    // initialize output structures
    vic_init_output();

}

int vic_cesm_run(agrs_from_cesm) {

    // read forcing data
    vic_force();

    // run vic over the domain
    vic_cesm_run();

    // if output:
    vic_write();

    // if save:
    if (state_flag) {
        vic_store();
    }
}

int vic_cesm_final(agrs_from_cesm) {

    // clean up
    vic_finalize();
}


