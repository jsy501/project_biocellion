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
        CELL_TYPE_LTI,
        CELL_TYPE_LTIN,
        CELL_TYPE_LTO,
        NUM_CELL_TYPES
};

enum cell_model_lto_int_e{
        CELL_MODEL_LTO_LTI_BIND_COUNT_PREV, // number of stable bind formed between LTo and LTi previous step
        CELL_MODEL_LTO_LTI_BIND_COUNT_TOTAL, // total number of stable bind formed between LTo and LTi regardless of prolonged contact
        CELL_MODEL_LTO_LTIN_BIND_COUNT_PREV, // number of stable bind formed between LTo and LTin previous step
        CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL, // total number of stable bind formed between LTo and LTin regardless of prolonged contact
        NUM_CELL_MODEL_LTO_INTS
};

enum cell_model_lto_real_e{
        CELL_MODEL_LTO_SPEED,
        CELL_MODEL_LTO_CHEMO_EXP_LVL,
        CELL_MODEL_LTO_ADHESION_EXP_LVL,
        NUM_CELL_MODEL_LTO_REALS
};

enum cell_model_lti_int_e{
        NUM_CELL_MODEL_LTI_INTS
};

enum cell_model_lti_real_e{
        CELL_MODEL_LTI_SPEED,
        CELL_MODEL_LTI_DIRECTION_X,
        CELL_MODEL_LTI_DIRECTION_Y,
        CELL_MODEL_LTI_DIRECTION_Z,
        CELL_MODEL_LTI_MOVE_LEFT,
        NUM_CELL_MODEL_LTI_REALS
};

enum cell_model_ltin_int_e{
        NUM_CELL_MODEL_LTIN_INTS
};

enum cell_model_ltin_real_e{
        CELL_MODEL_LTIN_SPEED,
        CELL_MODEL_LTIN_DIRECTION_X,
        CELL_MODEL_LTIN_DIRECTION_Y,
        CELL_MODEL_LTIN_DIRECTION_Z,
        CELL_MODEL_LTIN_MOVE_LEFT,
        NUM_CELL_MODEL_LTIN_REALS
};

/* used for storing cell displacement details*/
enum cell_mech_real_e{
        CELL_MECH_REAL_DIRECTION_X,
        CELL_MECH_REAL_DIRECTION_Y,
        CELL_MECH_REAL_DIRECTION_Z,
        NUM_CELL_MECH_REALS
};

enum junction_end_type_e{
        JUNCTION_END_TYPE_LTIN_TO_LTO, /* junction between LTin cell and LTo cell, LTin side. */
        JUNCTION_END_TYPE_LTO_TO_LTIN, /* junction between LTin cell and LTo cell, LTo side. */
        JUNCTION_END_TYPE_LTI_TO_LTO, /* junction between LTi cell and LTo cell, LTi side. */
        JUNCTION_END_TYPE_LTO_TO_LTI, /* junction between LTi cell and LTo cell, LTo side. */
        NUM_JUNCTION_END_TYPES
};

enum model_rng_type_e {
        MODEL_RNG_UNIFORM,
        MODEL_RNG_GAUSSIAN,
        NUM_MODEL_RNGS
};

enum extra_output_scalar_e{
        OUTPUT_CELL_POSITION_X,
        OUTPUT_CELL_POSITION_Y,
        OUTPUT_CELL_POSITION_Z,
        NUM_EXTRA_OUTPUT_SCALAR
};

/* summary output variables act as global variables. Used for getting information of LTo in agent update routines */
enum summary_type_real_e {
        /* position of LTo cell from origin(bottom left corner of the grid) */
        SUMMARY_REAL_LTO_POS_X,
        SUMMARY_REAL_LTO_POS_Y,
        SUMMARY_REAL_LTO_POS_Z,

        /* chemokine expression level of LTo cell */
        SUMMARY_REAL_LTO_CHEMO_EXP_LVL,

        /* adhesion expression level of LTo cell */
        SUMMARY_REAL_LTO_ADHESION_EXP_LVL,
        NUM_SUMMARY_REALS
};

enum summary_type_int_e{
        /* number of total stable binds formed regardless of prolonged contact */
        SUMMARY_INT_LTO_LTI_BIND_COUNT,
        SUMMARY_INT_LTO_LTIN_BIND_COUNT,
        NUM_SUMMARY_INTS
};

const REAL BASELINE_TIME_STEP_DURATION = 10; /* duration of one basline time step; second */
const REAL NUM_STEP_PER_MINUTE = 60 / BASELINE_TIME_STEP_DURATION;

/* cell presence percentage obtained from FACS(Fluorescence-activated cell sorting) at E15.5.
        required for calculating cell input rate */
const REAL LTIN_CELL_PERCENTAGE = 0.45;
const REAL LTI_CELL_PERCENTAGE = 0.37;

const S32 LTIN_CELL_INPUT_TIME = 24 * 60 * NUM_STEP_PER_MINUTE; /* time length before LTin cell migration ceases; step */
const S32 LTI_CELL_INPUT_TIME = 72 * 60 * NUM_STEP_PER_MINUTE; /* time length before cell migration ceases; step */

const REAL CELL_RADIUS[NUM_CELL_TYPES] = {8.0, 8.0, 24.0}; /* cell radius; micron */
const REAL CELL_MAX_INTERACTION_DISTANCE[NUM_CELL_TYPES] = {8.0, 8.0, 24.0}; /* maximum distance for physical cell interaction; micron */
const REAL CELL_SPEED_UPPER_BOUND = 8.8; /* upper bound for cell speed; microns per minute */
const REAL CELL_SPEED_LOWER_BOUND = 3.8; /* lower bound for cell speed; microns per minute */

const REAL LTO_CHEMO_EXP_MIN = 0.2; /* chemokine expression level for one LTi bind, used as a negative value therefore min */
const REAL LTO_CHEMO_EXP_MAX = 0.04; /* chemokine expression level for fully saturated LTi bind, used as a negative value therefore max */
const REAL LTO_CHEMO_EXP_INCREMENT_PER_LTI_CONTACT = 0.005; /* increase amount in LTo chemokine expression for every new LTi bind */
const REAL LTI_CHEMO_THRESHOLD = 0.3; /* threshold that LTi cells react to chemokine */
const REAL SIGMOID_CONSTANT = 10.0; /* sigmoid constand for adjusting the curve to meet y axis */

const REAL STABLE_BIND_PROBABILITY = 0.5; /* probability of two cells having a stable bind when in contact */
const REAL VCAM_SLOPE = 1.0;    /* gradient of linear vcam slop */
const REAL VCAM_INCREMENT = 0.05; /* vcam increment per stable contact */
const REAL MAX_VCAM_PROBABILITY_THRESHOLD = 0.65; /* maximum threshold for vcam value */

const REAL LTO_INITIAL_ADHESION_LVL = 0;

/* Biocellion domain is made of cubes with edge length of IF_GRID_SPACING;
therefore 1 grid unit is made of 50 1 micon long cubes, ie 50 microns in length*/
const REAL IF_GRID_SPACING = 30.0;

const S32 NUM_STATE_AND_GRID_TIME_STEPS_PER_BASELINE = 1;

/* MODEL END */

#endif/* #ifndef __MY_DEFINE_H__ */
