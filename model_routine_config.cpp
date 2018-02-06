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

void ModelRoutine::updateIfGridSpacing( REAL& ifGridSpacing ) {
	/* MODEL START */

	ifGridSpacing = IF_GRID_SPACING;

	/* MODEL END */

	return;
}

void ModelRoutine::updateOptModelRoutineCallInfo( OptModelRoutineCallInfo& callInfo ) {
	/* MODEL START */

	callInfo.numComputeMechIntrctIters = 1;
	callInfo.numUpdateIfGridVarPreStateAndGridStepIters = 0;
	callInfo.numUpdateIfGridVarPostStateAndGridStepIters = 0;

	/* MODEL END */

	return;
}

void ModelRoutine::updateDomainBdryType( domain_bdry_type_e a_domainBdryType[DIMENSION] ) {
	/* MODEL START */

	a_domainBdryType[0] = DOMAIN_BDRY_TYPE_NONPERIODIC_HARD_WALL;
	a_domainBdryType[1] = DOMAIN_BDRY_TYPE_NONPERIODIC_HARD_WALL;
	a_domainBdryType[2] = DOMAIN_BDRY_TYPE_NONPERIODIC_HARD_WALL;

	/* MODEL END */

	return;
}

void ModelRoutine::updatePDEBufferBdryType( pde_buffer_bdry_type_e& pdeBufferBdryType ) {
	/* MODEL START */

	pdeBufferBdryType = PDE_BUFFER_BDRY_TYPE_HARD_WALL;

	/* MODEL END */

	return;
}

void ModelRoutine::updateTimeStepInfo( TimeStepInfo& timeStepInfo ) {
	/* MODEL START */

	timeStepInfo.durationBaselineTimeStep = BASELINE_TIME_STEP_DURATION;
	timeStepInfo.numStateAndGridTimeStepsPerBaseline = 1;

	/* MODEL END */

	return;
}

void ModelRoutine::updateSyncMethod( sync_method_e& mechIntrctSyncMethod, sync_method_e& updateIfGridVarSyncMethod/* dummy if both callUpdateIfGridVarPreStateAndGridStep and callUpdateIfGridVarPostStateAndGridStep are set to false in ModelRoutine::updateOptModelRoutineCallInfo */ ) {
	/* MODEL START */

	mechIntrctSyncMethod = SYNC_METHOD_PER_ATTR; /* Sync per attribute*/
	updateIfGridVarSyncMethod = SYNC_METHOD_PER_ATTR;

	/* MODEL END */

	return;
}

#if HAS_SPAGENT
void ModelRoutine::updateSpAgentInfo( Vector<SpAgentInfo>& v_spAgentInfo ) {/* set the mechanical interaction range & the numbers of model specific variables */
	/* MODEL START */

	MechModelVarInfo modelVarInfo;

	/*
	If this option is selected, the differences from the initial
	value are summed—e.g. if three different model routines set the value of variable a to
	3, 5, and 9, respectively, then a is set to 3 + 5 + 9 (assuming that the initial value is 0).
	*/
	modelVarInfo.syncMethod = VAR_SYNC_METHOD_DELTA;

	v_spAgentInfo.resize(NUM_CELL_TYPES);

	for (S32 i = 0; i < NUM_CELL_TYPES; i++){
		SpAgentInfo info;

		info.dMax = CELL_D_MAX[i]; /* maximum direct physio-mechanical interaction distance */

		/*
		The interface grid spacing should be equal to or larger than the maximum
		direct physico-mechanical interaction distance between any two discrete agents.
		*/
		// CHECK( info.dMax <= IF_GRID_SPACING );

		info.numBoolVars = 0;
		info.numStateModelInts = 0;

		if (i == CELL_TYPE_LTO){
			info.numStateModelInts = NUM_CELL_MODEL_LTO_INTS;
	                info.numStateModelReals = NUM_CELL_MODEL_LTO_REALS;
		}
		else if (i == CELL_TYPE_LTI){
			info.numStateModelReals = NUM_CELL_MODEL_LTI_REALS;
		}
		else if (i == CELL_TYPE_LTIN){
			info.numStateModelReals = NUM_CELL_MODEL_LTIN_REALS;
		}

		/*
		Provide the information about model specific REAL type temporary mechanical
		interaction state attributes for this discrete agent type.
		*/
                info.v_mechIntrctModelRealInfo.assign(NUM_CELL_MECH_REALS, modelVarInfo);
                info.v_mechIntrctModelIntInfo.clear();
                info.v_odeNetInfo.clear();

		v_spAgentInfo[i] = info;
	}

	/* MODEL END */

	return;
}
#endif

void ModelRoutine::updateJunctionEndInfo( Vector<JunctionEndInfo>& v_junctionEndInfo ) {/* set the numbers of model specific variables */
	/* MODEL START */

	v_junctionEndInfo.resize(NUM_JUNCTION_END_TYPES);

	v_junctionEndInfo[JUNCTION_END_TYPE_LTI_TO_LTO].numModelReals = 0;
	v_junctionEndInfo[JUNCTION_END_TYPE_LTI_TO_LTO].numModelInts = 0;

	v_junctionEndInfo[JUNCTION_END_TYPE_LTO_TO_LTI].numModelReals = 0;
	v_junctionEndInfo[JUNCTION_END_TYPE_LTO_TO_LTI].numModelInts = 0;

	v_junctionEndInfo[JUNCTION_END_TYPE_LTIN_TO_LTO].numModelReals = 0;
	v_junctionEndInfo[JUNCTION_END_TYPE_LTIN_TO_LTO].numModelInts = 0;

	v_junctionEndInfo[JUNCTION_END_TYPE_LTO_TO_LTIN].numModelReals = 0;
	v_junctionEndInfo[JUNCTION_END_TYPE_LTO_TO_LTIN].numModelInts = 0;

	/* MODEL END */

	return;
}

void ModelRoutine::updatePhiPDEInfo( Vector<PDEInfo>& v_phiPDEInfo ) {
	/* MODEL START */

	v_phiPDEInfo.clear();

	/* MODEL END */

	return;
}

void ModelRoutine::updateIfGridModelVarInfo( Vector<IfGridModelVarInfo>& v_ifGridModelRealInfo, Vector<IfGridModelVarInfo>& v_ifGridModelIntInfo ) {
	/* MODEL START */

	v_ifGridModelRealInfo.clear();
	v_ifGridModelIntInfo.clear();

	/* MODEL END */

	return;
}

void ModelRoutine::updateRNGInfo( Vector<RNGInfo>& v_rngInfo ) {
	/* MODEL START */

	CHECK(NUM_MODEL_RNGS == 2);

	v_rngInfo.resize(NUM_MODEL_RNGS);

	RNGInfo rngInfo;

	rngInfo.type = RNG_TYPE_UNIFORM;
	rngInfo.param0 = 0.0;
	rngInfo.param1 = 1.0;
	rngInfo.param2 = 0.0;
	v_rngInfo[MODEL_RNG_UNIFORM] = rngInfo;

	rngInfo.type = RNG_TYPE_GAUSSIAN;
	rngInfo.param0 = 0.0;
	rngInfo.param1 = 1.0;
	rngInfo.param2 = 0.0;
	v_rngInfo[MODEL_RNG_GAUSSIAN] = rngInfo;

	/* MODEL END */

	return;
}

void ModelRoutine::updateFileOutputInfo( FileOutputInfo& fileOutputInfo ) {
	/* MODEL START */

	fileOutputInfo.particleOutput = true;
	fileOutputInfo.v_particleExtraOutputScalarVarName.clear();
        fileOutputInfo.v_particleExtraOutputVectorVarName.clear();
        fileOutputInfo.v_gridPhiOutput.clear();
        fileOutputInfo.v_gridPhiOutputDivideByKappa.clear();

	/* MODEL END */

	return;
}

void ModelRoutine::updateSummaryOutputInfo( Vector<SummaryOutputInfo>& v_summaryOutputRealInfo, Vector<SummaryOutputInfo>& v_summaryOutputIntInfo ) {
	/* MODEL START */

	SummaryOutputInfo info;

	v_summaryOutputIntInfo.resize(NUM_SUMMARY_INTS);
	v_summaryOutputRealInfo.resize(NUM_SUMMARY_REALS);

	info.name = "LToLTiBindCount";
	info.type = SUMMARY_TYPE_MAX;
	v_summaryOutputIntInfo[SUMMARY_INT_LTO_LTI_BIND_COUNT] = info;

	info.name = "LToLTinBindCount";
	info.type = SUMMARY_TYPE_MAX;
	v_summaryOutputIntInfo[SUMMARY_INT_LTO_LTIN_BIND_COUNT] = info;

	info.name = "LToPosX";
	info.type = SUMMARY_TYPE_MAX;
	v_summaryOutputRealInfo[SUMMARY_REAL_LTO_POS_X] = info;

	info.name = "LToPosY";
	info.type = SUMMARY_TYPE_MAX;
	v_summaryOutputRealInfo[SUMMARY_REAL_LTO_POS_Y] = info;

	info.name = "LToPosZ";
	info.type = SUMMARY_TYPE_MAX;
	v_summaryOutputRealInfo[SUMMARY_REAL_LTO_POS_Z] = info;

	info.name = "LToChemoExpLvl";
	info.type = SUMMARY_TYPE_MIN;
	v_summaryOutputRealInfo[SUMMARY_REAL_LTO_CHEMO_EXP_LVL] = info;

	info.name = "LToAdhesionExpLvl";
	info.type = SUMMARY_TYPE_MAX;
	v_summaryOutputRealInfo[SUMMARY_REAL_LTO_ADHESION_EXP_LVL] = info;

	/* MODEL END */

	return;
}

void ModelRoutine::initGlobal( Vector<U8>& v_globalData ) {
	/* MODEL START */

	/* MODEL END */

	return;
}

void ModelRoutine::init( void ) {
	/* MODEL START */

	/* MODEL END */

	return;
}

void ModelRoutine::term( void ) {
	/* MODEL START */

	/* MODEL END */

	return;
}

void ModelRoutine::setPDEBuffer( const VIdx& startVIdx, const VIdx& regionSize, BOOL& isPDEBuffer ) {
	/* MODEL START */

	isPDEBuffer = false;

	/* MODEL END */

	return;
}

void ModelRoutine::setHabitable( const VIdx& vIdx, BOOL& isHabitable ) {
	/* MODEL START */

	isHabitable = true;

	/* MODEL END */

	return;
}
