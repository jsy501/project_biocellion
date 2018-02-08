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
		Place LTi and LTin cells randomly in the grid
		*/
		for (S32 i = 0; i < INI_N_CELLS[CELL_TYPE_LTI]; i++){
			VIdx vidx;
			VReal vOffset;
			SpAgentState state;

			/* randomly place cells in one of the unit grid boxes*/
			vidx[0] = (regionSize[0] - 1) * Util::getModelRand(MODEL_RNG_UNIFORM);
			vidx[1] = (regionSize[1] - 1) * Util::getModelRand(MODEL_RNG_UNIFORM);
			vidx[2] = (regionSize[2] - 1) * Util::getModelRand(MODEL_RNG_UNIFORM);

			/* randomly distribute cells within the unit grid box, (0,0,0) being the centre of the box */
			vOffset[0] = IF_GRID_SPACING * (Util::getModelRand(MODEL_RNG_UNIFORM) - 0.5);
			vOffset[1] = IF_GRID_SPACING * (Util::getModelRand(MODEL_RNG_UNIFORM) - 0.5);
			vOffset[2] = IF_GRID_SPACING * (Util::getModelRand(MODEL_RNG_UNIFORM) - 0.5);

			/* set cell parameters */
			state.setType(CELL_TYPE_LTI);
			state.setRadius(CELL_RADIUS[CELL_TYPE_LTI]);
			state.setModelReal(CELL_MODEL_LTI_SPEED, Util::getModelRand(MODEL_RNG_UNIFORM) * (CELL_SPEED_UPPER_BOUND - CELL_SPEED_LOWER_BOUND) + CELL_SPEED_LOWER_BOUND);

			CHECK(ifGridHabitableBoxData.get(vidx) == true);
			v_spAgentVIdx.push_back(vidx);
			v_spAgentOffset.push_back( vOffset );
			v_spAgentState.push_back(state);
		}

		for (S32 i = 0; i < INI_N_CELLS[CELL_TYPE_LTIN]; i++){
			VIdx vidx;
			VReal vOffset;
			SpAgentState state;

			vidx[0] = (regionSize[0] - 1) * Util::getModelRand(MODEL_RNG_UNIFORM);
			vidx[1] = (regionSize[1] - 1) * Util::getModelRand(MODEL_RNG_UNIFORM);
			vidx[2] = (regionSize[2] - 1) * Util::getModelRand(MODEL_RNG_UNIFORM);

			vOffset[0] = IF_GRID_SPACING * (Util::getModelRand(MODEL_RNG_UNIFORM) - 0.5);
			vOffset[1] = IF_GRID_SPACING * (Util::getModelRand(MODEL_RNG_UNIFORM) - 0.5);
			vOffset[2] = IF_GRID_SPACING * (Util::getModelRand(MODEL_RNG_UNIFORM) - 0.5);

			state.setType(CELL_TYPE_LTIN);
			state.setRadius(CELL_RADIUS[CELL_TYPE_LTIN]);
			state.setModelReal(CELL_MODEL_LTIN_SPEED, Util::getModelRand(MODEL_RNG_UNIFORM) * (CELL_SPEED_UPPER_BOUND - CELL_SPEED_LOWER_BOUND) + CELL_SPEED_LOWER_BOUND);

			CHECK(ifGridHabitableBoxData.get(vidx) == true);
			v_spAgentVIdx.push_back(vidx);
			v_spAgentOffset.push_back( vOffset );
			v_spAgentState.push_back(state);
		}

		/*
		Place LTo cell in the middle of the grid
		*/

		VIdx vidx;
		VReal vOffset;
		SpAgentState state;

		vidx[0] = (regionSize[0] - 1) / 2;
		vidx[1] = (regionSize[1] - 1) / 2;
		vidx[2] = (regionSize[2] - 1) / 2;

		vOffset[0] = 0.5 * IF_GRID_SPACING;
		vOffset[1] = 0.5 * IF_GRID_SPACING;
		vOffset[2] = 0.5 * IF_GRID_SPACING;

		state.setType(CELL_TYPE_LTO);
		state.setRadius(CELL_RADIUS[CELL_TYPE_LTO]);
		state.setModelReal(CELL_MODEL_LTO_SPEED, 0);	// Assumes that LTo cell does not move
		state.setModelReal(CELL_MODEL_LTO_CHEMO_EXP_LVL, LTO_CHEMO_EXP_MIN);
		state.setModelReal(CELL_MODEL_LTO_ADHESION_EXP_LVL, LTO_INITIAL_ADHESION_LVL);
		state.setModelInt(CELL_MODEL_LTO_LTI_BIND_COUNT_PREV, 0);
		state.setModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_PREV, 0);
		state.setModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL, 0);
		state.setModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL, 0);

		CHECK(ifGridHabitableBoxData.get(vidx) == true);
		v_spAgentVIdx.push_back(vidx);
		v_spAgentOffset.push_back( vOffset );
		v_spAgentState.push_back(state);
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

	/* count the number of Lti/Ltin bind to Lto and modify state of Lto */
	if (state.getType() >= CELL_TYPE_LTO){
		S32 previousLtiBindCount = state.getModelInt(CELL_MODEL_LTO_LTI_BIND_COUNT_PREV);
		S32 previousLtinBindCount = state.getModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_PREV);
		S32 totalLtiBindCount = state.getModelInt(CELL_MODEL_LTO_LTI_BIND_COUNT_TOTAL);
		S32 totalLtinBindCount = state.getModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL);
		S32 ltiBindCount = 0;
		S32 ltinBindCount = 0;

		for (S32 i = 0; i < junctionData.getNumJunctions(); i++){
			junctionEndType_t type = junctionData.getJunctionEndRef(i).getType();
			if (type == JUNCTION_END_TYPE_LTO_TO_LTI){
				ltiBindCount++;
			}
			else if (type == JUNCTION_END_TYPE_LTO_TO_LTIN){
				ltinBindCount++;
			}
		}

		/* overwrite current count to previous count */
		state.setModelInt(CELL_MODEL_LTO_LTI_BIND_COUNT_PREV, ltiBindCount);
		state.setModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_PREV, ltinBindCount);

		/* if bind count increased from previous step, add the difference to vcam count*/
		if (previousLtinBindCount < ltinBindCount){
			totalLtinBindCount += ltinBindCount - previousLtinBindCount;
		}
		if (previousLtiBindCount < ltiBindCount){
			totalLtiBindCount += ltiBindCount - previousLtiBindCount;
		}

		/* update total count */
		state.setModelInt(CELL_MODEL_LTO_LTI_BIND_COUNT_TOTAL, totalLtiBindCount);
		state.setModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL, totalLtinBindCount);

		REAL adhesionLvl = VCAM_SLOPE * VCAM_INCREMENT * (totalLtiBindCount + totalLtinBindCount);

		if (adhesionLvl > MAX_VCAM_PROBABILITY_THRESHOLD){
			adhesionLvl = MAX_VCAM_PROBABILITY_THRESHOLD;
		}

		state.setModelReal(CELL_MODEL_LTO_ADHESION_EXP_LVL, adhesionLvl);

		/* equation for chemokine expression of LTo cell */
		REAL chemoLvl = -LTO_CHEMO_EXP_MIN + LTO_CHEMO_EXP_INCREMENT_PER_LTI_CONTACT * totalLtiBindCount;
		if (chemoLvl > -LTO_CHEMO_EXP_MAX){
			chemoLvl = -LTO_CHEMO_EXP_MAX;
		}

		state.setModelReal(CELL_MODEL_LTO_CHEMO_EXP_LVL, chemoLvl);
	}

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

	agentType_t type = state.getType();

	/* get cell speed */
	REAL speed;
	switch (type){
		case CELL_TYPE_LTO:
		speed = state.getModelReal(CELL_MODEL_LTO_SPEED);
		break;

		case CELL_TYPE_LTI:
		speed = state.getModelReal(CELL_MODEL_LTI_SPEED);
		break;

		case CELL_TYPE_LTIN:
		speed = state.getModelReal(CELL_MODEL_LTIN_SPEED);
		break;
	}

	/* move if the cell is not connected to any other cells */
	if (speed > 0 && junctionData.getNumJunctions() == 0){

		/* if the current cell is LTi and there was at least one LTo-LTin bond and one LTo-LTi bond formed before, move LTi cell towards LTo cell under the effect of chemokine*/
		if (type == CELL_TYPE_LTI && Info::getRecentSummaryIntVal(SUMMARY_INT_LTO_LTIN_BIND_COUNT) > 0 && Info::getRecentSummaryIntVal(SUMMARY_INT_LTO_LTI_BIND_COUNT) > 0){
			VReal ltoPos;
			VReal ltiPos;
			VReal dir;
			REAL ltoChemoExpLvl = Info::getRecentSummaryRealVal(SUMMARY_REAL_LTO_CHEMO_EXP_LVL);

			ltoPos[0] = Info::getRecentSummaryRealVal(SUMMARY_REAL_LTO_POS_X);
			ltoPos[1] = Info::getRecentSummaryRealVal(SUMMARY_REAL_LTO_POS_Y);
			ltoPos[2] = Info::getRecentSummaryRealVal(SUMMARY_REAL_LTO_POS_Z);

			Util::changePosFormat2LvTo1Lv(vIdx, vOffset, ltiPos);

			dir = ltoPos - ltiPos;

			/* equation for chemokine level at the position of the LTi cell */
			REAL chemoLvl = 1 / (1 + exp(-(ltoChemoExpLvl * dir.length() + SIGMOID_CONSTANT)));

			/* if chemokine expression is higher than the minimum threshold that Lti cells respond to */
			if (chemoLvl > LTI_CHEMO_THRESHOLD){
				VReal normDir = dir.normalize(0, dir); /* normalised direction vector from LTi to LTo */

				/* add attraction movement to cell with probability model*/
				if (chemoLvl >= Util::getModelRand(MODEL_RNG_UNIFORM)){
					for (S32 dim = 0; dim < SYSTEM_DIMENSION; dim++){
						disp[dim] += speed * normDir[dim];
					}
				}
			}

			/* if lower than minimum threshold, move randomly */
			else {
				VReal dir;

				/* this prevents the case where all three values are 0 */
				while (true){
					dir[0] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
					dir[1] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
					dir[2] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
					if (dir[0] != 0 || dir[1] != 0 || dir[2] != 0){
						break;
					}
				}

				VReal normDir = dir.normalize(0, dir); // normalise vector

				/* multiply by speed(radius) */
				for (S32 dim = 0; dim < SYSTEM_DIMENSION; dim++){
					disp[dim] += normDir[dim] * speed;
				}
			}

			/* displacement due to cell shoving */
			disp[0] += mechIntrctData.getModelReal(CELL_MECH_REAL_DISPLACEMENT_X);
			disp[1] += mechIntrctData.getModelReal(CELL_MECH_REAL_DISPLACEMENT_Y);
			disp[2] += mechIntrctData.getModelReal(CELL_MECH_REAL_DISPLACEMENT_Z);
		}

		/* non chemokine affected lti cells that had a mechanical interaction previous step */
		else if (mechIntrctData.getModelReal(CELL_MECH_REAL_DISPLACEMENT_X) != 0 || mechIntrctData.getModelReal(CELL_MECH_REAL_DISPLACEMENT_Y) != 0
		|| mechIntrctData.getModelReal(CELL_MECH_REAL_DISPLACEMENT_Z != 0)){
			disp[0] += mechIntrctData.getModelReal(CELL_MECH_REAL_DISPLACEMENT_X);
			disp[1] += mechIntrctData.getModelReal(CELL_MECH_REAL_DISPLACEMENT_Y);
			disp[2] += mechIntrctData.getModelReal(CELL_MECH_REAL_DISPLACEMENT_Z);
		}

		/* cells that did not have a mechanical interaction previous step; move cell to random direction,
		this is same as picking a random point on a sphere with a radius of value of cell speed */
		else {
			VReal dir;

			/* this prevents the case where all three values are 0 */
			while (true){
				dir[0] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
				dir[1] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
				dir[2] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
				if (dir[0] != 0 || dir[1] != 0 || dir[2] != 0){
					break;
				}
			}

			VReal normDir = dir.normalize(0, dir); // normalise vector

			/* multiply by speed(radius) */
			for (S32 dim = 0; dim < SYSTEM_DIMENSION; dim++){
				disp[dim] += normDir[dim] * speed;
			}
		}

	}

	/* prevents cell moving more than grid spacing */
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
