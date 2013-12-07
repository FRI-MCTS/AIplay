#ifndef _TOM_GAME_DIFFSIM_
#define _TOM_GAME_DIFFSIM_

//#includes
#include "Game_Engine.hpp"
#include "Support_InputFile.hpp"

//default values
#define TOMGAME_DIFFSIM_TMP_MAX_PLYS	200
#define TOMGAME_DIFFSIM_TMP_TARGET_PREDATORS	6.0
#define TOMGAME_DIFFSIM_TMP_TARGET_MAX_DIFF		20.0

#define TOMGAME_DIFFSIM_SCORE_POLY_EXP	1		//set polynomial exponent for score scaling (on interval [0,1]), value 1 is linear

#define TOMGAME_DIFFSIM_STEPFACT_RATE	2		//parameter step_factor change rate	(value of step_factor determines how much gamma changes when increased/decreased)
#define TOMGAME_DIFFSIM_STEPFACT_MAX	10
#define TOMGAME_DIFFSIM_STEPFACT_MIN	0.01
#define TOMGAME_DIFFSIM_PARAM_C_MAX		20
#define TOMGAME_DIFFSIM_PARAM_C_MIN		0.002



/**
Definiton of the Differential simulation engine.

*/
#pragma once
class Game_DiffSim : public Game_Engine
{

public:
	
	//public procedures - engine
	Game_DiffSim(Game_Engine* source_game = TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY);	//Constructor
	virtual ~Game_DiffSim(void);							//Destructor
	virtual void Initialize();
	virtual void Game_Reset();
	virtual int Select_Move_Passive();
	virtual void Calculate_Score();

	//public procedures - input
	virtual void Set_Target_Trajectory_From_File(const char* fileName, double normalize_shift = 0.0, double normalize_scale = 1.0);

	//public procedures - support
	virtual void Copy_Game_State_From(Game_Engine* source, const bool copy_history = TOMGAME_ENGINE_COPY_GAME_HISTORY, int history_copy_start_index = 0);
	virtual Game_Engine* Create_Duplicate_Game(bool copy_state = TOMGAME_ENGINE_COPY_GAME_STATE, const bool copy_history = TOMGAME_ENGINE_COPY_GAME_HISTORY);
	virtual int  Get_Next_Player(int);
	virtual int	 Human_Move_Input();
	virtual int  Human_Move_Translate(int human_move);

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
	virtual void Allocate_Memory();
	virtual void Clear_Memory();
	virtual void Copy_Settings(Game_Engine* source_game);
	virtual int  Game_Dynamics(int selected_move);
	virtual bool Check_Game_Win(int position);
	virtual void Set_Trajectory_Target();
	virtual double Calculate_Trajectory_Error();

	//private procedures - support

	//private variables - game state


	//game-specific settings
	double simulation_timeStep;
	int num_timeSteps_perAction;

	//game-specific variables
	double* state;				//current simulation state (state variables and parameters)
	double** trajectory_target;	//2D array of desired trajectories: number_samples (maximum_plys) x observred_state_atributes
	double trajectory_error;	//current error from desired trajectory
	double internal_score;		//sum of errors from start


};

#endif