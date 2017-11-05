/*

Copyright © 2013 Battelle Memorial Institute. All Rights Reserved.

NOTICE:  These data were produced by Battelle Memorial Institute (BATTELLE) under Contract No. DE-AC05-76RL01830 with the U.S. Department of Energy (DOE).  For a five year period from May 28, 2013, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, and perform publicly and display publicly, by or on behalf of the Government.  There is provision for the possible extension of the term of this license.  Subsequent to that period or any extension granted, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, distribute copies to the public, perform publicly and display publicly, and to permit others to do so.  The specific term of the license can be identified by inquiry made to BATTELLE or DOE.  NEITHER THE UNITED STATES NOR THE UNITED STATES DEPARTMENT OF ENERGY, NOR BATTELLE, NOR ANY OF THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, COMPLETENESS, OR USEFULNESS OF ANY DATA, APPARATUS, PRODUCT, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*/

#ifndef __MY_DEFINE_H__
#define __MY_DEFINE_H__

#include "biocellion.h"

/* define constants to be used inside model functions */

#define SYSTEM_DIMENSION 3

/* MODEL START */

enum cell_type_e {
  CELL_TYPE_LTI, //LTi cell
  CELL_TYPE_LTIN, //LTin cell
  CELL_TYPE_LTO, //LTo cell
  NUM_CELL_TYPES
};

enum cell_mech_real_e{
        CELL_MECH_REAL_FORCE_X,
        CELL_MECH_REAL_FORCE_Y,
        CELL_MECH_REAL_FORCE_Z,
        NUM_CELL_MECH_REALS
};

enum junction_end_type_e{
        JUNCTION_END_TYPE_A, /* junction between LTi/LTin cell and LTo cell, LTi/LTin side. */
        JUNCTION_END_TYPE_B, /* junction between LTi/LTin cell and LTo cell, LTo side. */
        NUM_JUNCTION_END_TYPES
};

enum junction_end_type_a_model_real_e {
        // JUNCTION_END_TYPE_A_MODEL_REAL_DIST,
        // JUNCTION_END_TYPE_A_MODEL_REAL_DIR_X,
        // JUNCTION_END_TYPE_A_MODEL_REAL_DIR_Y,
        // JUNCTION_END_TYPE_A_MODEL_REAL_DIR_Z,
        NUM_JUNCTION_END_TYPE_A_MODEL_REAL
};

enum junction_end_type_b_model_real_e {
        NUM_JUNCTION_END_TYPE_B_MODEL_REAL
};

enum model_rng_type_e {
  MODEL_RNG_UNIFORM,
  MODEL_RNG_GAUSSIAN,
  NUM_MODEL_RNGS
};

const S32 A_INI_N_CELLS[NUM_CELL_TYPES] = { 100, 100, 1 };
const REAL A_CELL_RADIUS[NUM_CELL_TYPES] = {2.0, 2.0, 5.0};
const REAL A_CELL_DIFFUSION_COEFF[NUM_CELL_TYPES] = {0.5, 0.5, 0};
const REAL A_CELL_D_MAX[NUM_CELL_TYPES] = {10.0, 10.0, 10.0};

const REAL A_CELL_ADHESION = 0.5; /* cell adhesion constant*/
const REAL A_CELL_SPRING_CONSTANT = 0.1; /*cell shoving constant*/

const REAL IF_GRID_SPACING = 10.0;

const REAL BASELINE_TIME_STEP_DURATION = 10.0;

const S32 NUM_STATE_AND_GRID_TIME_STEPS_PER_BASELINE = 1;

/* MODEL END */

#endif/* #ifndef __MY_DEFINE_H__ */
