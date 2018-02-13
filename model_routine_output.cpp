/*

Copyright © 2013 Battelle Memorial Institute. All Rights Reserved.

NOTICE:  These data were produced by Battelle Memorial Institute (BATTELLE) under Contract No. DE-AC05-76RL01830 with the U.S. Department of Energy (DOE).  For a five year period from May 28, 2013, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, and perform publicly and display publicly, by or on behalf of the Government.  There is provision for the possible extension of the term of this license.  Subsequent to that period or any extension granted, the Government is granted for itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license in this data to reproduce, prepare derivative works, distribute copies to the public, perform publicly and display publicly, and to permit others to do so.  The specific term of the license can be identified by inquiry made to BATTELLE or DOE.  NEITHER THE UNITED STATES NOR THE UNITED STATES DEPARTMENT OF ENERGY, NOR BATTELLE, NOR ANY OF THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, COMPLETENESS, OR USEFULNESS OF ANY DATA, APPARATUS, PRODUCT, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*/

/* DO NOT USE FUNCTIONS THAT ARE NOT THREAD SAFE (e.g. rand(), use Util::getModelRand() instead) */

#include "biocellion.h"

#include "model_routine.h"

/* UESR START */

#include "model_define.h"

/* UESR END */

using namespace std;

#if HAS_SPAGENT
void ModelRoutine::updateSpAgentOutput( const VIdx& vIdx, const SpAgent& spAgent, REAL& color, Vector<REAL>& v_extraScalar, Vector<VReal>& v_extraVector ) {
	/* MODEL START */

	color = spAgent.state.getType();

	/* change LTo color depending on its state; immature, vcam expressed, chemokine expressed */
	if (color == CELL_TYPE_LTO){
		if (spAgent.state.getModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL) > 0){
			color++;
			if (spAgent.state.getModelInt(CELL_MODEL_LTO_LTI_BIND_COUNT_TOTAL) > 0){
				color++;
			}
		}
	}

	CHECK( v_extraScalar.size() == NUM_EXTRA_OUTPUT_SCALAR );

	VReal cellPos;
	Util::changePosFormat2LvTo1Lv(vIdx, spAgent.vOffset, cellPos);

	v_extraScalar[OUTPUT_CELL_POSITION_X] = cellPos[0];
	v_extraScalar[OUTPUT_CELL_POSITION_Y] = cellPos[1];
	v_extraScalar[OUTPUT_CELL_POSITION_Z] = cellPos[2];

      	CHECK( v_extraVector.size() == 0 );

	/* MODEL END */

	return;
}
#endif

void ModelRoutine::updateSummaryVar( const VIdx& vIdx, const NbrUBAgentData& nbrUBAgentData, const NbrUBEnv& nbrUBEnv, Vector<REAL>& v_realVal/* [elemIdx] */, Vector<S32>& v_intVal/* [elemIdx] */ ) {
	/* MODEL START */

	const UBAgentData& ubAgentData = *(nbrUBAgentData.getConstPtr(0,0,0));
	for (S32 i = 0; i < (S32)ubAgentData.v_spAgent.size(); i++){
		SpAgent agent = ubAgentData.v_spAgent[i];
		if (agent.state.getType() == CELL_TYPE_LTO){
			VReal ltoPos;

			/* get absolute coordinates of LTo cell */
			for (S32 i = 0; i < SYSTEM_DIMENSION; i++){
				Util::changePosFormat2LvTo1Lv(vIdx, agent.vOffset, ltoPos);
			}

			v_realVal[SUMMARY_REAL_LTO_POS_X] = ltoPos[0];
			v_realVal[SUMMARY_REAL_LTO_POS_Y] = ltoPos[1];
			v_realVal[SUMMARY_REAL_LTO_POS_Z] = ltoPos[2];

			v_realVal[SUMMARY_REAL_LTO_CHEMO_EXP_LVL] = agent.state.getModelReal(CELL_MODEL_LTO_CHEMO_EXP_LVL);
			v_realVal[SUMMARY_REAL_LTO_ADHESION_EXP_LVL] = agent.state.getModelReal(CELL_MODEL_LTO_ADHESION_EXP_LVL);

			v_intVal[SUMMARY_INT_LTO_LTI_BIND_COUNT] = agent.state.getModelInt(CELL_MODEL_LTO_LTI_BIND_COUNT_TOTAL);
			v_intVal[SUMMARY_INT_LTO_LTIN_BIND_COUNT] = agent.state.getModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL);

			break;
		}
	}

	/* MODEL END */

	return;
}
