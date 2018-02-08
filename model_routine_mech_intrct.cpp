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
void ModelRoutine::initJunctionSpAgent( const VIdx& vIdx0, const SpAgent& spAgent0, const VIdx& vIdx1, const SpAgent& spAgent1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */
) {
	/* MODEL START */

	link = false;

	/* MODEL END */

	return;
}

void ModelRoutine::computeMechIntrctSpAgent( const S32 iter, const VIdx& vIdx0, const SpAgent& spAgent0, const UBEnv& ubEnv0, const VIdx& vIdx1, const SpAgent& spAgent1, const UBEnv& ubEnv1, const VReal& dir/* unit direction vector from spAgent1 to spAgent0 */, const REAL& dist, MechIntrctData& mechIntrctData0, MechIntrctData& mechIntrctData1, BOOL& link, JunctionEnd& end0/* dummy if link == false */, JunctionEnd& end1/* dummy if link == false */, BOOL& unlink ) {
	/* MODEL START */

	REAL radiusSum = spAgent0.state.getRadius() + spAgent1.state.getRadius();
	REAL overlap = radiusSum - dist;

	agentType_t type0 = spAgent0.state.getType();
	agentType_t type1 = spAgent1.state.getType();

	/* if two cells are linked already, calculate probability of prolonged adhesion */
	if (spAgent0.junctionData.isLinked(spAgent1.junctionData) == true){
		/* check if the cell0 is LTO cell */
		if (type0 >= CELL_TYPE_LTO){
			if (spAgent0.state.getModelReal(CELL_MODEL_LTO_ADHESION_EXP_LVL) >= Util::getModelRand(MODEL_RNG_UNIFORM)){
				/* keep bond if probability is higher than random number */
				link = true;
			} else {
				/* break bond if not and the cell moves away from Lto cell*/
				WARNING("bond break")
				link = false;
				unlink = true;

				mechIntrctData1.setModelReal( CELL_MECH_REAL_DISPLACEMENT_X, -dir[0] * overlap);
				mechIntrctData1.setModelReal( CELL_MECH_REAL_DISPLACEMENT_Y, -dir[1] * overlap);
				mechIntrctData1.setModelReal( CELL_MECH_REAL_DISPLACEMENT_Z, -dir[2] * overlap);
			}
		}

		/* check if the cell1 is LTO cell */
		else if (type1 >= CELL_TYPE_LTO){
			if (spAgent1.state.getModelReal(CELL_MODEL_LTO_ADHESION_EXP_LVL) >= Util::getModelRand(MODEL_RNG_UNIFORM)){
				link = true;
			} else {
				WARNING("bond break")
				link = false;
				unlink = true;

				mechIntrctData0.setModelReal( CELL_MECH_REAL_DISPLACEMENT_X, dir[0] * overlap);
				mechIntrctData0.setModelReal( CELL_MECH_REAL_DISPLACEMENT_Y, dir[1] * overlap);
				mechIntrctData0.setModelReal( CELL_MECH_REAL_DISPLACEMENT_Z, dir[2] * overlap);
			}
		}

		else {
			ERROR("No bond should have formed between Lti and Ltin");
		}
		return;
	}

	/* cell contact */
	if(overlap >= 0) {
		/* link the cells and set a junction if the contact is between LTi/LTin and LTo */
		if (type0 == CELL_TYPE_LTO){
			/* check if it can have a stable bind */
			if (STABLE_BIND_PROBABILITY >= Util::getModelRand(MODEL_RNG_UNIFORM)){
				/* get number of stable Ltin bind formed so far */
				S32 ltinBindCount = spAgent0.state.getModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL);

				if (type1 == CELL_TYPE_LTIN){
					end0.setType(JUNCTION_END_TYPE_LTO_TO_LTIN);
					end1.setType(JUNCTION_END_TYPE_LTIN_TO_LTO);
					link = true;
				}

				/* Lti bind can be formed only when there was a stable Ltin bind formed before */
				else if (type1 == CELL_TYPE_LTI && ltinBindCount > 0){
					end0.setType(JUNCTION_END_TYPE_LTO_TO_LTI);
					end1.setType(JUNCTION_END_TYPE_LTI_TO_LTO);
					link = true;
				}
			}
		}

		/* same for when cell1 is Lto */
		else if (type1 == CELL_TYPE_LTO){
			if (STABLE_BIND_PROBABILITY >= Util::getModelRand(MODEL_RNG_UNIFORM)){
				S32 ltinBindCount = spAgent1.state.getModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT_TOTAL);

				if (type0 == CELL_TYPE_LTIN){
					end1.setType(JUNCTION_END_TYPE_LTO_TO_LTIN);
					end0.setType(JUNCTION_END_TYPE_LTIN_TO_LTO);
					link = true;
				}

				else if (type0 == CELL_TYPE_LTI && ltinBindCount > 0){
					end1.setType(JUNCTION_END_TYPE_LTO_TO_LTI);
					end0.setType(JUNCTION_END_TYPE_LTI_TO_LTO);
					link = true;
				}
			}
		}

		/* cell shoving; cells that did not form stable bind will move away from the Lto */
		mechIntrctData0.setModelReal( CELL_MECH_REAL_DISPLACEMENT_X, dir[0] * overlap * 0.5);
		mechIntrctData0.setModelReal( CELL_MECH_REAL_DISPLACEMENT_Y, dir[1] * overlap * 0.5);
		mechIntrctData0.setModelReal( CELL_MECH_REAL_DISPLACEMENT_Z, dir[2] * overlap * 0.5);

		mechIntrctData1.setModelReal( CELL_MECH_REAL_DISPLACEMENT_X, -dir[0] * overlap * 0.5);
		mechIntrctData1.setModelReal( CELL_MECH_REAL_DISPLACEMENT_Y, -dir[1] * overlap * 0.5);
		mechIntrctData1.setModelReal( CELL_MECH_REAL_DISPLACEMENT_Z, -dir[2] * overlap * 0.5);
	}

	/* no contact */
	else{
		link =false;
		unlink = false;
	}

	/* MODEL END */

	return;
}
#endif
