/*

Copyright © 2013 Battelle Memorial Institute. All Rights Reserved.

NOTICE:  These data were produced by Battelle Memorial Institute (BATTELLE) under Contract No. DE-AC05-76RL01830 with the U.S. Department of Energy (DOE).  For a five year period from May 28, 2013, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, and perform publicly and display publicly, by or on behalf of the Government.  There is provision for the possible extension of the term of this license.  Subsequent to that period or any extension granted, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, distribute copies to the public, perform publicly and display publicly, and to permit others to do so.  The specific term of the license can be identified by inquiry made to BATTELLE or DOE.  NEITHER THE UNITED STATES NOR THE UNITED STATES DEPARTMENT OF ENERGY, NOR BATTELLE, NOR ANY OF THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, COMPLETENESS, OR USEFULNESS OF ANY DATA, APPARATUS, PRODUCT, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*/

/* DO NOT USE FUNCTIONS THAT ARE NOT THREAD SAFE (e.g. rand(), use Util::getModelRand() instead) */

#include "biocellion.h"

#include "model_routine.h"

/* MODEL START */

#include "model_define.h"

/* MODEL END */

using namespace std;

#if HAS_SPAGENT
void ModelRoutine::addSpAgents( const BOOL init, const VIdx& startVIdx, const VIdx& regionSize, const IfGridBoxData<BOOL>& ifGridHabitableBoxData, Vector<VIdx>& v_spAgentVIdx, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentOffset ) {/* initialization */
	/* MODEL START */

	if (init == true){

		/*
		Place LTo cell in the middle of the grid
		*/
		VIdx vidx;
		VReal vOffset;
		SpAgentState state;

		vidx[0] = regionSize[0]/2;
		vidx[1] = regionSize[1]/2;
		vidx[2] = regionSize[2]/2;

		vOffset[0] = 0.5 * IF_GRID_SPACING;
		vOffset[1] = 0.5 * IF_GRID_SPACING;
		vOffset[2] = 0.5 * IF_GRID_SPACING;

		state.setType(CELL_TYPE_C);
		state.setRadius(A_CELL_RADIUS[CELL_TYPE_C]);

		CHECK(ifGridHabitableBoxData.get(vidx) == true);
		v_spAgentVIdx.push_back(vidx);
		v_spAgentOffset.push_back( vOffset );
		v_spAgentState.push_back(state);

		/*
		Place LTi and LTin cells randomly in the grid
		*/
		for (S32 i = 0; i < NUM_CELL_TYPES-1; i++){
			for (S32 j = 0; j < A_INI_N_CELLS[i]; j++){
				VIdx vidx;
				VReal vOffset;
				SpAgentState state;

				vidx[0] = regionSize[0] * Util::getModelRand(MODEL_RNG_UNIFORM) - 1;
				vidx[1] = regionSize[1] * Util::getModelRand(MODEL_RNG_UNIFORM) - 1;
				vidx[2] = regionSize[2] * Util::getModelRand(MODEL_RNG_UNIFORM) - 1;

				state.setType(i);
				state.setRadius(A_CELL_RADIUS[i]);

				CHECK(ifGridHabitableBoxData.get(vidx) == true);
				v_spAgentVIdx.push_back(vidx);
				v_spAgentOffset.push_back( vOffset );
				v_spAgentState.push_back(state);
			}
		}
	}

	/* MODEL END */

	return;
}

void ModelRoutine::spAgentCRNODERHS( const S32 odeNetIdx, const VIdx& vIdx, const SpAgent& spAgent, const NbrUBEnv& nbrUBEnv, const Vector<double>& v_y, Vector<double>& v_f ) {
	/* MODEL START */

	/* MODEL END */

	return;
}

void ModelRoutine::updateSpAgentState( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */ ) {
	/* MODEL START */

	/* MODEL END */

	return;
}

void ModelRoutine::spAgentSecretionBySpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, Vector<SpAgentState>& v_spAgentState, Vector<VReal>& v_spAgentDisp ) {
	/* MODEL START */

	/* MODEL END */

	return;
}

void ModelRoutine::updateSpAgentBirthDeath( const VIdx& vIdx, const SpAgent& spAgent, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, BOOL& divide, BOOL& disappear ) {
	/* MODEL START */

	divide = false;
        disappear = false;

	/* MODEL END */

	return;
}

void ModelRoutine::adjustSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& state/* INOUT */, VReal& disp ) {/* if not dividing or disappearing */
	/* MODEL START */

	S32 type = state.getType();

	if (A_CELL_DIFFUSION_COEFF[type] > 0.0){

		REAL f_prw = SQRT(2 * A_CELL_DIFFUSION_COEFF[type] * BASELINE_TIME_STEP_DURATION);
		for (S32 dim = 0; dim < SYSTEM_DIMENSION; dim++){
			disp[dim] = f_prw * Util::getModelRand(MODEL_RNG_GAUSSIAN);
		}
	}

	for (S32 dim = 0; dim < SYSTEM_DIMENSION; dim++){
		if (disp[dim] > IF_GRID_SPACING){
			disp[dim] = IF_GRID_SPACING;
		}
		else if (disp[dim] < (IF_GRID_SPACING * -1.0)){
			disp[dim] = IF_GRID_SPACING * -1.0;
		}
	}

	/* MODEL END */

	return;
}

void ModelRoutine::divideSpAgent( const VIdx& vIdx, const JunctionData& junctionData, const VReal& vOffset, const MechIntrctData& mechIntrctData, const NbrUBEnv& nbrUBEnv, SpAgentState& motherState/* INOUT */, VReal& motherDisp, SpAgentState& daughterState, VReal& daughterDisp, Vector<BOOL>& v_junctionDivide, BOOL& motherDaughterLinked, JunctionEnd& motherEnd, JunctionEnd& daughterEnd ) {
	/* MODEL START */

	/* MODEL END */

	return;
}
#endif
