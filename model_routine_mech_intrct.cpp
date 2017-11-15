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

	/* exit if two cells are bound already */
	if (spAgent0.junctionData.isLinked(spAgent1.junctionData) == true){
		return;
	}

        REAL R = spAgent0.state.getRadius() + spAgent1.state.getRadius();
	REAL mag;

	agentType_t type0 = spAgent0.state.getType();
	agentType_t type1 = spAgent1.state.getType();

	/* cell contact */
        if( dist <= R ) {
		/* link the cells and set a junction if the contact is between LTi/LTin and LTo */
		if (type0 == CELL_TYPE_LTO){
			S32 ltinBindCount = spAgent0.state.getModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT);

			if (type1 == CELL_TYPE_LTIN){
				WARNING("LINK FORMED");
				end0.setType(JUNCTION_END_TYPE_LTO_TO_LTIN);
				end1.setType(JUNCTION_END_TYPE_LTIN_TO_LTO);
				link = true;
			}

			else if (type1 == CELL_TYPE_LTI && ltinBindCount > 0){
				WARNING("LINK FORMED");
				end0.setType(JUNCTION_END_TYPE_LTO_TO_LTI);
				end1.setType(JUNCTION_END_TYPE_LTI_TO_LTO);
				link = true;
			}

			else{
				link = false;
			}
		}

		else if (type1 == CELL_TYPE_LTO){
			S32 ltinBindCount = spAgent1.state.getModelInt(CELL_MODEL_LTO_LTIN_BIND_COUNT);

			if (type0 == CELL_TYPE_LTIN){
				WARNING("LINK FORMED");
				end1.setType(JUNCTION_END_TYPE_LTO_TO_LTIN);
				end0.setType(JUNCTION_END_TYPE_LTIN_TO_LTO);
				link = true;
			}

			else if (type0 == CELL_TYPE_LTI && ltinBindCount > 0){
				WARNING("LINK FORMED");
				end1.setType(JUNCTION_END_TYPE_LTO_TO_LTI);
				end0.setType(JUNCTION_END_TYPE_LTI_TO_LTO);
				link = true;
			}

			else{
				link = false;
			}
		}

		/* cell shoving */
		else{
			mag = CELL_SPRING_CONSTANT * ( R - dist );
		}
        }
	else{
		link =false;
		unlink = false;
	}

	mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_X, dir[0] * mag);
	mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_Y, dir[1] * mag);
	mechIntrctData0.setModelReal( CELL_MECH_REAL_FORCE_Z, dir[2] * mag);

	mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_X, -dir[0] * mag);
	mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_Y, -dir[1] * mag);
	mechIntrctData1.setModelReal( CELL_MECH_REAL_FORCE_Z, -dir[2] * mag);

	/* MODEL END */

	return;
}
#endif
