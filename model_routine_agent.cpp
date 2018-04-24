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

	OUTPUT(2, "cellInput");
	if (init == true){
		/* Place LTo cell in the middle of the grid at the beginning of simulation */

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
		state.setModelReal(CELL_MODEL_LTO_PROLONGED_ADHESION_PROB, LTO_INITIAL_ADHESION_LVL);
		state.setModelInt(CELL_MODEL_LTO_LTI_BIND_COUNT_PREV, 0);
		state.setModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_PREV, 0);
		state.setModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL, 0);
		state.setModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL, 0);

		CHECK(ifGridHabitableBoxData.get(vidx) == true);
		v_spAgentVIdx.push_back(vidx);
		v_spAgentOffset.push_back(vOffset);
		v_spAgentState.push_back(state);
	}

	/* add H cells every baseline time at cell input rate */
	else{

		REAL totalHCells = (regionSize[0] * IF_GRID_SPACING / CELL_RADIUS[CELL_TYPE_LTIN]) *
		(regionSize[1] * IF_GRID_SPACING / CELL_RADIUS[CELL_TYPE_LTIN]) *
		(regionSize[2] * IF_GRID_SPACING / CELL_RADIUS[CELL_TYPE_LTIN]);
		REAL ltinInputNumE15 = (totalHCells / 100) * LTIN_CELL_PERCENTAGE;
		REAL ltiInputNumE15 = (totalHCells / 100) * LTI_CELL_PERCENTAGE;

		REAL ltinInputRate = ltinInputNumE15 / (24 * 60 * NUM_STEP_PER_MINUTE);
		REAL ltiInputRate = ltiInputNumE15 / (24 * 60 * NUM_STEP_PER_MINUTE);
		REAL ltinInputRemainder = ltinInputRate - (S32)ltinInputRate;
		REAL ltiInputRemainder = ltiInputRate - (S32)ltiInputRate;

		/* if flag is greater than 1, add 1 additional cell and subtract 1 from flag*/
		if (ltinInputRemainder >= Util::getModelRand(MODEL_RNG_UNIFORM)){
			ltinInputRate += 1;
		}

		if (ltiInputRemainder >= Util::getModelRand(MODEL_RNG_UNIFORM)){
			ltiInputRate += 1;
		}

		if (Info::getCurBaselineTimeStep() <= LTI_CELL_INPUT_TIME){
			for (S32 i = 0; i < (S32)ltiInputRate; i++){
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

				REAL speed = Util::getModelRand(MODEL_RNG_UNIFORM) * (CELL_SPEED_UPPER_BOUND - CELL_SPEED_LOWER_BOUND) + CELL_SPEED_LOWER_BOUND;
				state.setModelReal(CELL_MODEL_LTI_SPEED, speed / NUM_STEP_PER_MINUTE);

				CHECK(ifGridHabitableBoxData.get(vidx) == true);
				v_spAgentVIdx.push_back(vidx);
				v_spAgentOffset.push_back( vOffset );
				v_spAgentState.push_back(state);
			}
		}

		if (Info::getCurBaselineTimeStep() <= LTIN_CELL_INPUT_TIME){
			for (S32 i = 0; i < (S32)ltinInputRate; i++){
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

				REAL speed = Util::getModelRand(MODEL_RNG_UNIFORM) * (CELL_SPEED_UPPER_BOUND - CELL_SPEED_LOWER_BOUND) + CELL_SPEED_LOWER_BOUND;
				state.setModelReal(CELL_MODEL_LTI_SPEED, speed / NUM_STEP_PER_MINUTE);

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

	agentType_t type = state.getType();

	/* if the cell is LTo, count the number of Lti/Ltin bind to Lto and modify state of Lto */
	if (type == CELL_TYPE_LTO){
		OUTPUT(2, "updateCellState");
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

		/* if bind count increased from previous step, add the difference to total bind count which is used for vcam expression*/
		if (previousLtinBindCount < ltinBindCount){
			totalLtinBindCount += ltinBindCount - previousLtinBindCount;
		}
		if (previousLtiBindCount < ltiBindCount){
			totalLtiBindCount += ltiBindCount - previousLtiBindCount;
		}

		// /* update active step count */
		// if (totalLtinBindCount > 0){
		// 	state.incModelInt(CELL_MODEL_LTO_NUM_ACTIVE_STEPS, 1);
		// }

		/* update total count */
		state.setModelInt(CELL_MODEL_LTO_LTI_BIND_COUNT_TOTAL, totalLtiBindCount);
		state.setModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL, totalLtinBindCount);

		REAL probability = VCAM_SLOPE * VCAM_INCREMENT * (totalLtiBindCount + totalLtinBindCount);

		if (probability > MAX_VCAM_PROBABILITY_THRESHOLD){
			probability = MAX_VCAM_PROBABILITY_THRESHOLD;
		}

		state.setModelReal(CELL_MODEL_LTO_PROLONGED_ADHESION_PROB, probability);

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

	// /* if the cell is LTo and reached division time after activation, set divide parameter true */
	// if (spAgent.state.getType() == CELL_TYPE_LTO){
	// 	S32 activeStep = spAgent.state.getModelInt(CELL_MODEL_LTO_NUM_ACTIVE_STEPS);
	// 	if (activeStep > 0 && activeStep % LTO_DIVISION_TIME == 0){
	// 		divide = true;
	// 		disappear = false;
	// 	}
	// } else{
	// 	divide = false;
	// 	disappear = false;
	// }

	divide = false;
	disappear =false;

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
		/* get how many times it moved with current direction
		CELL_MODEL_LTI_MOVE_REMAIN is in the same index as CELL_MODEL_LTIN_MOVE_REMAIN so either can be used */
		S32 moveRemain = state.getModelInt(CELL_MODEL_LTI_MOVE_REMAIN);

		/* if the cell moved for a minute or was just added, give it a new random direction to move
		this is same as picking a random point on a sphere with a radius of 1 */
		if (moveRemain == 0){
			VReal dir;

			while (true){
				dir[0] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
				dir[1] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
				dir[2] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
				/* this prevents the case where all three values are 0 */
				if (dir[0] != 0 || dir[1] != 0 || dir[2] != 0){
					break;
				}
			}

			VReal normDir = dir.normalize(0, dir); // normalise vector
			state.setModelReal(CELL_MODEL_LTI_DIRECTION_X, normDir[0]);
			state.setModelReal(CELL_MODEL_LTI_DIRECTION_Y, normDir[1]);
			state.setModelReal(CELL_MODEL_LTI_DIRECTION_Z, normDir[2]);


			/* if the current cell is LTi and there was at least one LTo-LTin bond and one LTo-LTi bond formed before,
			 set direction of LTi cell towards LTo cell under the effect of chemokine.
			 This check will happen every minute */
			if (type == CELL_TYPE_LTI && Info::getRecentSummaryIntVal(SUMMARY_INT_LTO_LTIN_BIND_COUNT) > 0 &&
				Info::getRecentSummaryIntVal(SUMMARY_INT_LTO_LTI_BIND_COUNT) > 0){
				VReal ltoPos;
				VReal ltiPos;
				VReal dir;
				REAL ltoChemoExpLvl = Info::getRecentSummaryRealVal(SUMMARY_REAL_LTO_CHEMO_EXP_LVL);

				ltoPos[0] = Info::getRecentSummaryRealVal(SUMMARY_REAL_LTO_POS_X);
				ltoPos[1] = Info::getRecentSummaryRealVal(SUMMARY_REAL_LTO_POS_Y);
				ltoPos[2] = Info::getRecentSummaryRealVal(SUMMARY_REAL_LTO_POS_Z);

				Util::changePosFormat2LvTo1Lv(vIdx, vOffset, ltiPos);

				/* get vector direction */
				dir = ltoPos - ltiPos;

				/* equation for chemokine level at the position of the LTi cell */
				REAL chemoLvl = 1 / (1 + exp(-(ltoChemoExpLvl * dir.length() + SIGMOID_CONSTANT)));

				/* if chemokine expression is higher than the minimum threshold that Lti cells respond to */
				if (chemoLvl > LTI_CHEMO_THRESHOLD){
					VReal normDir = dir.normalize(0, dir); /* normalised direction vector from LTi to LTo */

					/* set move direction to LTo cell with probability model using chemokine level*/
					if (chemoLvl >= Util::getModelRand(MODEL_RNG_UNIFORM)){
						state.setModelReal(CELL_MODEL_LTI_DIRECTION_X, normDir[0]);
						state.setModelReal(CELL_MODEL_LTI_DIRECTION_Y, normDir[1]);
						state.setModelReal(CELL_MODEL_LTI_DIRECTION_Z, normDir[2]);
					}

					/* if not affected, the cell will move in random direction as set above */
				}
			}

			// reset move count
			moveRemain = NUM_STEP_PER_MINUTE;
		}

		/* if the cell had a mechanical interaction previous step, change its direction to opposite to the collided agent
		This will take priority over any direction changes*/
		if (mechIntrctData.getModelReal(CELL_MECH_REAL_DIRECTION_X) != 0 || mechIntrctData.getModelReal(CELL_MECH_REAL_DIRECTION_Y) != 0
		|| mechIntrctData.getModelReal(CELL_MECH_REAL_DIRECTION_Z != 0)){
			state.setModelReal(CELL_MODEL_LTI_DIRECTION_X, mechIntrctData.getModelReal(CELL_MECH_REAL_DIRECTION_X));
			state.setModelReal(CELL_MODEL_LTI_DIRECTION_Y, mechIntrctData.getModelReal(CELL_MECH_REAL_DIRECTION_Y));
			state.setModelReal(CELL_MODEL_LTI_DIRECTION_Z, mechIntrctData.getModelReal(CELL_MECH_REAL_DIRECTION_Z));
		}


		/* move cell using its speed and direction */
		disp[0] += state.getModelReal(CELL_MODEL_LTI_DIRECTION_X) * speed;
		disp[1] += state.getModelReal(CELL_MODEL_LTI_DIRECTION_Y) * speed;
		disp[2] += state.getModelReal(CELL_MODEL_LTI_DIRECTION_Z) * speed;
		moveRemain -= 1;

		state.setModelInt(CELL_MODEL_LTI_MOVE_REMAIN, moveRemain);
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

	// /* copy all model variables from mother to daughter cell */
	// daughterState.setType(motherState.getType());
	// daughterState.setRadius(motherState.getRadius());
	//
	// for (S32 i = 0; i < NUM_CELL_MODEL_LTO_REALS; i++){
	// 	daughterState.setModelReal(i, motherState.getModelReal(i));
	// }
	//
	// for (S32 i = 0; i < NUM_CELL_MODEL_LTO_INTS; i++){
	// 	daughterState.setModelInt(i, motherState.getModelInt(i));
	// }
	//
	// WARNING("check");
	// /* get random direction for cell division; daughter cell will be created at this direction */
	// VReal dir;
	// while (true){
	// 	dir[0] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
	// 	dir[1] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
	// 	dir[2] = Util::getModelRand(MODEL_RNG_GAUSSIAN);
	// 	/* this prevents the case where all three values are 0 */
	// 	if (dir[0] != 0 || dir[1] != 0 || dir[2] != 0){
	// 		break;
	// 	}
	// }
	// VReal normDir = dir.normalize(0, dir); // normalise vector
	//
	// REAL separationDis = motherState.getRadius() * 0.5;
	// for (S32 dim = 0; dim < SYSTEM_DIMENSION; dim++){
	// 	motherDisp[dim] = -dir[dim] * separationDis;
	// }
	//
	// for (S32 dim = 0; dim < SYSTEM_DIMENSION; dim++){
	// 	daughterDisp[dim] = dir[dim] * separationDis;
	// }
	//
	// motherDaughterLinked = false;

	/* MODEL END */

	return;
}
#endif
