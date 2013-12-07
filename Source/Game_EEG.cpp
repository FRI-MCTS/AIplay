//include header
#include "Game_EEG.hpp"

//TODO: moving avg copy array when duplicating game!

//constructor
Game_EEG::Game_EEG(Game_Engine* source_game)
{

	//game definition
	game_name = "EEG-phase";
	is_deterministic = true;

	simulation_timeStep = 0.001;
	num_timeSteps_perAction = 100;

	//call initialization procedures
	if(source_game == TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY)
		Initialize();
	else
		Copy_Initialize(source_game);
}

//destructor
Game_EEG::~Game_EEG(void)
{
	//release memory space
	Clear_Memory();
}

//init game settings
void Game_EEG::Init_Settings()
{

	//general debug settings
	show_warnings = TOM_DEBUG;
	
	//general game settings
	board_size = 9;
	number_players = 1;
	//maximum_allowed_moves = 3*3*3;	//to enable tunning TauXY (81 actions)
	maximum_allowed_moves = 3*3;
	maximum_plys = TOMGAME_EEG_TMP_MAX_PLYS;

}

//create duplicate game
Game_Engine* Game_EEG::Create_Duplicate_Game(bool copy_state, bool copy_history)
{
	//create new game by copying settings of this game
	Game_EEG* new_game = new Game_EEG(this);

	//set current state of new game
	if(copy_state)
		new_game->Copy_Game_State_From(this, copy_history, 0);
	else
		new_game->Game_Reset();
		
	//return pointer to new game object
	return new_game;
}

/**
Copies game state from target game
WARNING: move list is not copied

@param history_copy_start_index Defines the ammount of moves from beginning where to start copying history (useful if most of the target game is same as source)
*/
void Game_EEG::Copy_Game_State_From(Game_Engine* source, bool copy_history, int history_copy_start_index)
{
	//copygame state variables
	for(int i = 0; i < board_size; i++){
		state[i] = ((Game_EEG*)source)->state[i];
	}
	internal_score = ((Game_EEG*)source)->internal_score;
	trajectory_target = ((Game_EEG*)source)->trajectory_target;

	for(int j = 0; j < number_players; j++){
		current_number_moves[j] = source->current_number_moves[j];
		for(int i = 0; i < maximum_allowed_moves; i++){
			current_moves[j][i] = source->current_moves[j][i];
			//current_moves_list[j][i] = source->current_moves_list[j][i];	//making a list of moves in this phase is usually only waste of computation time
		}
		score[j] = source->score[j];
	}
	current_player = source->current_player;
	game_ended = source->game_ended;

	//copy history
	current_plys = source->current_plys;
	if(copy_history){
		for(int i = history_copy_start_index+1; i < current_plys+1; i++){
			history_moves[i] = source->history_moves[i];
		}
	}

	//copy moving average history
	movingAvg_val = ((Game_EEG*)source)->movingAvg_val;
	movingAvg_Cnt = ((Game_EEG*)source)->movingAvg_Cnt;
	for(int i = 0; i < TOMGAME_EEG_MOVING_AVG_SAMPLES; i++)
		movingAvg_Array[i] = ((Game_EEG*)source)->movingAvg_Array[i];

	simulation_timeStep = ((Game_EEG*)source)->simulation_timeStep;
	num_timeSteps_perAction = ((Game_EEG*)source)->num_timeSteps_perAction;

	//copy parameter values
	Cxx = ((Game_EEG*)source)->Cxx;
	Cxy = ((Game_EEG*)source)->Cxy;
	Cxz = ((Game_EEG*)source)->Cxz;
	Cyx = ((Game_EEG*)source)->Cyx;
	Cyy = ((Game_EEG*)source)->Cyy;
	Cyz = ((Game_EEG*)source)->Cyz;
	Czx = ((Game_EEG*)source)->Czx;
	Czy = ((Game_EEG*)source)->Czy;
	Czz = ((Game_EEG*)source)->Czz;
	iP = ((Game_EEG*)source)->iP;
	iQ = ((Game_EEG*)source)->iQ;
	iR = ((Game_EEG*)source)->iR;
	Tx = ((Game_EEG*)source)->Tx;
	Ty = ((Game_EEG*)source)->Ty;
	Tz = ((Game_EEG*)source)->Tz;
	sig_a = ((Game_EEG*)source)->sig_a;
	sig_theta = ((Game_EEG*)source)->sig_theta;

}

/**
Reset to initial values (restart game)
WARNING: move list is not created
*/
void Game_EEG::Game_Reset()
{
	//reset game state variables
	state[7] = 0.0;		//model output

	state[0] = 0.0;		//phase space x
	state[1] = 0.0;		//phase space y
	state[2] = 0.0;		//phase space z

	state[3] = 0.0;		//external input P
	state[4] = 0.0;		//external input Q
	state[5] = 0.0;		//external input R

	state[6] = 2.0;		//change rate

	//fixed for activation function (from paper)
	sig_a = 1.0;
	sig_theta = 4.0;

	//configuration as in Figure 3ac in paper
	//simulate 3s
	//Cxx = 24;	Cxy = -20;	Cxz = -15;
	//Cyx = 40;	Cyy = 0;	Cyz = 0;
	//Czx = 7;	Czy = 0;	Czz = 0;
	//iP = 3;		iQ = -2;	iR = 0;
	//Tx = 0.013;	Ty = 0.013;	Tz = 0.267;

	//configuration as in Figure 3df in paper
	//simulate 3s
	//Cxx = 23;	Cxy = -15;	Cxz = -10;
	//Cyx = 35;	Cyy = 0;	Cyz = 0;
	//Czx = 10;	Czy = 0;	Czz = 0;
	//iP = 0.5;		iQ = -5;	iR = -5;
	//Tx = 0.015;	Ty = 0.013;	Tz = 0.267;

	//configuration as in Figure 3gi in paper – MISTAKE in paper? looks like 3gi and 3jl are exchanged?
	//simulate 3s
	//Cxx = 23;	Cxy = -15;	Cxz = -10;
	//Cyx = 35;	Cyy = 0;	Cyz = 0;
	//Czx = 10;	Czy = 0;	Czz = 0;
	//iP = 3;		iQ = -5;	iR = -5;
	//Tx = 0.015;	Ty = 0.013;	Tz = 0.267;

	//configuration as in Figure 3jl in paper – MISTAKE in paper? looks like 3gi and 3jl are exchanged?
	//simulate 3s
	//Cxx = 25;	Cxy = -15;	Cxz = -10;
	//Cyx = 35;	Cyy = 0;	Cyz = 0;
	//Czx = 10;	Czy = 0;	Czz = 0;
	//iP = 4;		iQ = -5;	iR = -3;
	//Tx = 0.0225;	Ty = 0.03;	Tz = 0.12;

	//configuration as in Figure 4 in paper (iP should be changed from 3 to 5, but now it is fixed)
	//simulate 10s
	Cxx = 38;	Cxy = -29;	Cxz = -10;
	Cyx = 40;	Cyy = 0;	Cyz = 0;
	Czx = 20;	Czy = 0;	Czz = 0;
	iP = 0;		iQ = -2;	iR = 0;
	Tx = 0.013;	Ty = 0.013;	Tz = 0.267;

	//simulation time step should be at max 0.001 to ensure of numerical precision
	simulation_timeStep = 0.001;	//delta time in seconds per calculation
#if(TOMGAME_EEG_DISABLE_AVGERAGING)
	num_timeSteps_perAction = 1;	//number of calculations per output interval (1 equals to maximum resolution)
#else
	num_timeSteps_perAction = TOMGAME_EEG_MOVING_AVG_SAMPLES;
#endif

	state[3] = iP;		//external input P
	state[4] = iQ;		//external input Q
	state[5] = iR;		//external input R
	Tx = (1 / Tx);
	Ty = (1 / Ty);
	Tz = (1 / Tz);

	state[8] = Tx;

	//debug
	//Tx = 1;	Ty = 1;	Tz = 1;
	//simulation_timeStep = 0.01;
	//num_timeSteps_perAction = 100;

	//reset moving average history
	movingAvg_val = 0;
	movingAvg_Cnt = 0;
	for(int i = 0; i < TOMGAME_EEG_MOVING_AVG_SAMPLES; i++)
		movingAvg_Array[i] = 0.0;

	//reset score
	internal_score = 0.0;

	//reset allowed moves
	for(int j = 0; j < number_players; j++){
		current_number_moves[j] = maximum_allowed_moves;
		for(int i = 0; i < maximum_allowed_moves; i++){
			current_moves[j][i] = true;
			//current_moves_list[j][i] = i;	//making a list of moves in this phase is usually only waste of computation time
		}
		score[j] = 0.0;
	}
	current_player = 0;
	game_ended = false;

	//reset history
	current_plys = 0;
}

/**
@return Passive move (no action)
*/
int Game_EEG::Select_Move_Passive()
{
	return 4;
}

/**
@return 1 if game ended, 0 otherwise
*/
int Game_EEG::Game_Dynamics(int selected_move)
{
	double x, y, z, dx, dy, dz;
	double p, q, r;
	double stepFact;
	double tmpChk;

	x = state[0];
	y = state[1];
	z = state[2];
	p = state[3];
	q = state[4];
	r = state[5];
	stepFact = state[6];
	Tx = 1 / state[8];

	//execute move
	//--- parameter changes
	if(selected_move < maximum_allowed_moves/3){
		if(stepFact > TOMGAME_EEG_STEPFACT_MIN*TOMGAME_EEG_STEPFACT_RATE)	//lower limit
			stepFact /= TOMGAME_EEG_STEPFACT_RATE;
		else
			stepFact = TOMGAME_EEG_STEPFACT_MIN;
	}else if(selected_move < maximum_allowed_moves/3*2){
		stepFact = stepFact;
	}else{
		if(stepFact < TOMGAME_EEG_STEPFACT_MAX/TOMGAME_EEG_STEPFACT_RATE)	//upper limit
			stepFact *= TOMGAME_EEG_STEPFACT_RATE;
		else
			stepFact = TOMGAME_EEG_STEPFACT_MAX;
	}
	if(selected_move % 3 < 1){
		if( p-stepFact > TOMGAME_EEG_PARAM_P_MIN )
			p = p-stepFact;
		else
			p = TOMGAME_EEG_PARAM_P_MIN;
	}else if(selected_move % 3 < 2){
		p = p;
	}else{
		if( p+stepFact < TOMGAME_EEG_PARAM_P_MAX )
			p = p+stepFact;
		else
			p = TOMGAME_EEG_PARAM_P_MAX;
	}

	//UNCOMMENT THIS TO ENABLE tunning TauXY (must also set maximum_allowed_moves to 81 in Init_Settings())
	//if(selected_move % 3 < 3){
	//	if( Tx-stepFact*0.005 > TOMGAME_EEG_PARAM_T_MIN )
	//		Tx = Tx-stepFact*0.005;
	//	else
	//		Tx = TOMGAME_EEG_PARAM_T_MIN;
	//}else if(selected_move % 3 < 6){
	//	Tx = Tx;
	//}else{
	//	if( Tx+stepFact*0.005 < TOMGAME_EEG_PARAM_T_MAX )
	//		Tx = Tx+stepFact*0.005;
	//	else
	//		Tx = TOMGAME_EEG_PARAM_T_MAX;
	//}

	Tx = 1 / Tx;
	Ty = Tx;

	//--- simulation dynamics
    for(int i = 0; i < num_timeSteps_perAction; i++){

		dx = (-x + Activation_function(Cxx*x + Cxy*y + Cxz*z + p)) * Tx;
        dy = (-y + Activation_function(Cyx*x + Cyy*y + Cyz*z + q)) * Ty;
	    dz = (-z + Activation_function(Czx*x + Czy*y + Czz*z + r)) * Tz;
        x += dx * simulation_timeStep;
        y += dy * simulation_timeStep;
		z += dz * simulation_timeStep;

		//if(x > 1.0)
		//	x = 1.0;
		//else if(x < 0.0)
		//	x = 0.0;
		//if(y > 1.0)
		//	y = 1.0;
		//else if(y < 0.0)
		//	y = 0.0;
		//if(z > 1.0)
		//	z = 1.0;
		//else if(z < 0.0)
		//	z = 0.0;

		//calculate moving average value
#if(!TOMGAME_EEG_DISABLE_AVGERAGING)
		movingAvg_Array[movingAvg_Cnt] = x / TOMGAME_EEG_MOVING_AVG_SAMPLES;
		movingAvg_val += movingAvg_Array[movingAvg_Cnt];	//add newest sample

		if(movingAvg_Cnt < TOMGAME_EEG_MOVING_AVG_SAMPLES - 1)
			movingAvg_Cnt++;
		else
			movingAvg_Cnt = 0;

		movingAvg_val -= movingAvg_Array[movingAvg_Cnt];	//remove oldest sample
#endif
	}

	//hardcoded change input values
	//if( (current_plys+1) % 100 == 0) p += 0.2;

	//update game status
	tmpChk = x + y + z + p + q + r + stepFact;
	if (tmpChk - tmpChk == 0.0){	//check for infinity
		state[0] = x;
		state[1] = y;
		state[2] = z;
		state[3] = p;
		state[4] = q;
		state[5] = r;
		state[6] = stepFact;
		state[8] = Tx;
	}

	//update avaliable moves
	//TODO

	//set model output
#if(TOMGAME_EEG_DISABLE_AVGERAGING)
	state[7] = state[0];	//direct x value
#else
	state[7] = movingAvg_val;
#endif
	//state[7] = state[1];	//direct y value

	//check ending condition
	game_ended = Check_Game_Win(selected_move);

	//get current difference (error) from target trajectory
	trajectory_error = Calculate_Trajectory_Error();

	//update score 
	//-current implementation scores squared error
	//-current implementation is numerically imprecise if big numbers arise, better to use online division and multiplication x = (x*(plys-1) + pred)/plys	
	//if(trajectory_error*trajectory_error < TOMGAME_EEG_TMP_TARGET_MAX_DIFF)
		//internal_score -= trajectory_error*trajectory_error;
		internal_score -= abs(trajectory_error);
	//else
	//	internal_score += 0;	//no increase in score if error exceeds limit

	//return feedback
	return (int)game_ended;
}

//define goal in these two procedures
void Game_EEG::Set_Trajectory_Target()
{
	for(int i = 0; i < maximum_plys; i++)
		trajectory_target[i][0] = TOMGAME_EEG_TMP_TARGET_X;
}

double Game_EEG::Calculate_Trajectory_Error()
{
	return state[7] - trajectory_target[current_plys][0];
	//return ( (state[7] - trajectory_target[current_plys][0] + 0.001) / (trajectory_target[current_plys][0] + 0.002) );
}

void Game_EEG::Calculate_Score()
{
	if(current_plys > 0){
		//normalize score - currently rapresented by standard deviation
		//score[0] = sqrt(-internal_score / current_plys);
		score[0] = (-internal_score / current_plys);
		if( score[0] < TOMGAME_EEG_TMP_TARGET_MAX_DIFF ){
			score[0] = 1.0 - score[0] / TOMGAME_EEG_TMP_TARGET_MAX_DIFF;

#if(TOMGAME_EEG_SCORE_POLY_EXP != 1)
			//exponential score scaling (by polynomial) 
			score[0] = pow(score[0],TOMGAME_EEG_SCORE_POLY_EXP);
#endif

		}else{
			score[0] = 0.0;
		}
	}
}

//currently implemented as a sigmoid function
double Game_EEG::Activation_function(double iVal)
{
	return ( 1 / ( 1 + exp( -sig_a * ( iVal - sig_theta ) ) ) );
}

void Game_EEG::Output_Board_State()
{	

#if(TOM_OUTPUT_TO_MATLAB)
	
	Output_Board_State_Raw();
	return;

#else

	if(current_plys <= 0){
		printf("\nt %3d  a ---: \t x %5.6f  y %5.6f \t t -----   e ----- \t P %9.6f  s %9.6f",
			current_plys,	//time step
			state[0],	//x
			state[7],	//model output
			state[3],	//control variable "input P"
			state[6]	//gamma changing step (stepFact in Game_Dynamics)
			//state[7]	//MCTS single simulation duration (number of actions before forced end)
		);
	}else{
		printf("\nt %3d  a %3d: \t x %5.6f  y %5.6f \t t %5.6f  e %5.6f \t P %9.6f  s %9.6f",
			current_plys,	//time step
			history_moves[current_plys],	//last action
			state[0],	//x
			state[7],	//model output
			trajectory_target[current_plys-1][0],	//current target value
			trajectory_error,	//current error from desired trajectory
			state[3],	//control variable "input P"
			//state[6]	//gamma changing step (stepFact in Game_Dynamics)
			1/state[8]
			//state[7]	//MCTS single simulation duration (number of actions before forced end)
		);
	}
	fflush(stdout);

#endif
}

/**
DEFAULT: Output current board (game) state to standard output.
*/
void Game_EEG::Output_Board_State_Raw()
{	
#if(!TOM_OUTPUT_TO_MATLAB)
	if(current_plys <= 0){

		printf("\n%3d ---: %5.6f %5.6f  ----- -----  %9.6f %9.6f",
			current_plys,	//time step
			state[0],	//x
			state[7],	//model output
			state[3],	//control variable "input P"
			state[6]	//gamma changing step (stepFact in Game_Dynamics)
		);

	}else{
		printf("\n%3d %3d: %5.6f %5.6f  %5.6f %5.6f  %9.6f %9.6f",
			current_plys,	//time step
			history_moves[current_plys],	//last action
			state[0],	//x
			state[7],	//model output
			trajectory_target[current_plys-1][0],	//current target value
			trajectory_error,	//current error from desired trajectory
			state[3],	//control variable "input P"
			//state[6]	//gamma changing step (stepFact in Game_Dynamics)
			1/state[8]
		);
	}
#else
	if(current_plys <= 0){
	}else{
		printf("\n%3d %3d: %5.6f %5.6f  %5.6f %5.9f  %9.6f %9.6f",
			current_plys,	//time step
			history_moves[current_plys],	//last action
			state[0],	//x
			state[7],	//model output
			trajectory_target[current_plys-1][0],	//current target value
			trajectory_error,	//current error from desired trajectory
			state[3],	//control variable "input P"
			//state[6]	//gamma changing step (stepFact in Game_Dynamics)
			1/state[8]	//control variable "tau XY"
		);
	}
#endif


	fflush(stdout);

}
