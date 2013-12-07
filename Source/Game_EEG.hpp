#ifndef _TOM_GAME_EEG_
#define _TOM_GAME_EEG_

//#includes
#include "Game_DiffSim.hpp"

#define TOMGAME_EEG_DISABLE_AVGERAGING		0

//default values
#if(TOMGAME_EEG_DISABLE_AVGERAGING)
#define TOMGAME_EEG_TMP_MAX_PLYS			10000	//simulation time in miliseconds
#else
#define TOMGAME_EEG_TMP_MAX_PLYS			800		//number of simulated samples -> whole simulated time depends on TOMGAME_EEG_MOVING_AVG_SAMPLES (on number of samples per second)
#endif

#define TOMGAME_EEG_TMP_TARGET_X			0.25
#define TOMGAME_EEG_TMP_TARGET_MAX_DIFF		1.0//10.0

#define TOMGAME_EEG_SCORE_POLY_EXP	1		//set polynomial exponent for score scaling (on interval [0,1]), value 1 is linear

#define TOMGAME_EEG_STEPFACT_RATE	10		//parameter step_factor change rate	(value of step_factor determines how much gamma changes when increased/decreased)
#define TOMGAME_EEG_STEPFACT_MAX	10
#define TOMGAME_EEG_STEPFACT_MIN	0.01
#define TOMGAME_EEG_PARAM_P_MAX		20
#define TOMGAME_EEG_PARAM_P_MIN		-10
#define TOMGAME_EEG_PARAM_T_MAX		0.050
#define TOMGAME_EEG_PARAM_T_MIN		0.001


#define TOMGAME_EEG_MOVING_AVG_SAMPLES	((int)(250/16))		//miliseconds per output sample (250 -> 4/seconds per sample = 4 Hz)
			//for EEG_input1 this should be ((int)(250/1))
			//for EEG_input2 this should be ((int)(250/4))
			//for EEG_input3 this should be ((int)(250/16))

/**
EEG epilepsy spiking model based on paper:
Wang et al. "Phase space apporoach for modeling of epileptic dynamics"

*/
#pragma once
class Game_EEG : public Game_DiffSim
{

public:
	
	//public procedures - engine
	Game_EEG(Game_Engine* source_game = TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY);	//Constructor
	virtual ~Game_EEG(void);
	virtual int Select_Move_Passive();
	virtual void Game_Reset();
	virtual void Calculate_Score();

	//public procedures - support
	virtual void Copy_Game_State_From(Game_Engine* source, const bool copy_history, int history_copy_start_index);
	virtual Game_Engine* Create_Duplicate_Game(bool copy_state = TOMGAME_ENGINE_COPY_GAME_STATE, const bool copy_history = TOMGAME_ENGINE_COPY_GAME_HISTORY);
	
	//public procedures - debug and visualization
	virtual void Output_Board_State();
	virtual void Output_Board_State_Raw();

	//public variables - game settings

	//public variables - game state

	//public variables - game history

	//public variables - debug


protected:
	//private procedures
	virtual void Init_Settings();
	virtual int  Game_Dynamics(int selected_move);
	virtual void Set_Trajectory_Target();
	virtual double Calculate_Trajectory_Error();

	//private procedures - support

	//private variables - game state
	virtual double Activation_function(double iVal);

	double Cxx, Cxy, Cxz, Cyx, Cyy, Cyz, Czx, Czy, Czz;
	double iP,iQ,iR;
	double Tx, Ty, Tz;

	double sig_a, sig_theta;

	double movingAvg_Array[TOMGAME_EEG_MOVING_AVG_SAMPLES];
	int    movingAvg_Cnt;
	double movingAvg_val;
};

#endif