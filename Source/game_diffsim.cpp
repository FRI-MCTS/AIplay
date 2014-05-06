//include header
#include "Game_DiffSim.hpp"

//constructor
Game_DiffSim::Game_DiffSim(Game_Engine* source_game)
{

	//game definition
	game_name = "LotkaVolterra";
	is_deterministic = true;

	simulation_timeStep = 0.001;	//one calculation simulates 1ms
	num_timeSteps_perAction = 250;	//250 samples per output, with 1ms per calculation this means 4 output samples per second

	//call initialization procedures
	if(source_game == TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY)
		Initialize();
	else
		Copy_Initialize(source_game);
}

//destructor
Game_DiffSim::~Game_DiffSim(void)
{
	//release memory space
	Clear_Memory();
}

//allocate memory and initialize variables
void Game_DiffSim::Initialize()
{

	//initialize values common to all derivate objects
	Initialize_Common();

	//initialize settings
	Init_Settings();

	//allocate resources
	Allocate_Memory();
	
	//reset game state
	Game_Reset();
}

//create duplicate game
Game_Engine* Game_DiffSim::Create_Duplicate_Game(bool copy_state, bool copy_history)
{
	//create new game by copying settings of this game
	Game_DiffSim* new_game = new Game_DiffSim(this);

	//set current state of new game
	if(copy_state)
		new_game->Copy_Game_State_From(this, copy_history);
	else
		new_game->Game_Reset();
		
	//return pointer to new game object
	return new_game;
}

//init game settings
void Game_DiffSim::Init_Settings()
{

	//general debug settings
	show_warnings = TOM_DEBUG;
	
	//general game settings
	board_size = 8;
	number_players = 1;
	maximum_allowed_moves = 3*3;
	maximum_plys = TOMGAME_DIFFSIM_TMP_MAX_PLYS;

}

//copy settings from source_game
void Game_DiffSim::Copy_Settings(Game_Engine* source_game)
{

	//general game settings
	//board_length = source_game->board_length;
	//board_height = source_game->board_height;
	board_size = source_game->board_size;
	number_players = source_game->number_players;
	maximum_allowed_moves = source_game->maximum_allowed_moves;
	maximum_plys = source_game->maximum_plys;
	//param_score_win = source_game->param_score_win;
	//param_score_lose = source_game->param_score_lose;
	//param_score_draw = source_game->param_score_draw;
	trajectory_target = ((Game_DiffSim*)source_game)->trajectory_target;	//copy only pointer, not values

	//general debug settings
	show_warnings = source_game->show_warnings;

	//game-specific settings

}

void Game_DiffSim::Allocate_Memory()
{
	//allocate resources - game state
	state = new double[board_size];	
	current_number_moves = new int[number_players];
	current_moves = new bool*[number_players];
	current_moves_list = new int*[number_players];
	for(int i = 0; i < number_players; i++){
		current_moves[i] = new bool[maximum_allowed_moves];
		current_moves_list[i] = new int[maximum_allowed_moves];
	}
	score = new double[number_players];

	//allocate resources - history
	history_moves = new int[maximum_plys+1];	//added +1 to avoid access problems at game start, first value in array is -1
	history_moves[0] = -1;

	//allocate target trajectory
	trajectory_target = new double*[maximum_plys];
	for(int i = 0; i < maximum_plys; i++)
		trajectory_target[i] = new double[1];
	Set_Trajectory_Target();

	////allocate resources - visualization
	//output_board_lookup_char = new char[number_players+2];

	////define constant values - visualization
	//output_board_lookup_char[0] = 'E';
	//output_board_lookup_char[1] = '.';
	//if(number_players > 0)
	//	output_board_lookup_char[2] = 'X';
	//if(number_players > 1)
	//	output_board_lookup_char[3] = 'O';
	//for(int i = 4; i < number_players; i++)
	//	output_board_lookup_char[i] = '?';	//non-specified character

	//DEBUG
	//for(int i = 0; i < maximum_allowed_moves+1; i++)
	//	printf(" %d",history_moves[i]);
	//printf("\n");
}

void Game_DiffSim::Clear_Memory()
{

	//release memory space
	delete[] state;	
	for(int i = 0; i < maximum_plys; i++)
		delete[] trajectory_target[i];
	delete[] trajectory_target;
	delete[] current_number_moves;
	for(int i = 0; i < number_players; i++){
		delete[] current_moves[i];
		delete[] current_moves_list[i];
	}
	delete[] current_moves;
	delete[] current_moves_list;
	delete[] score;
	delete[] history_moves;
	//delete[] output_board_lookup_char);
}

/**
Reset to initial values (restart game)
WARNING: move list is not created
*/
void Game_DiffSim::Game_Reset()
{
	//reset game state variables
	state[0] = 20.0;	//prey
	state[1] = 5.0;		//predator

	state[2] = 0.4;		//alpha
	state[3] = 0.06;	//beta
	state[4] = 0.2;		//gamma
	state[5] = 0.02;	//delta

	state[6] = 0.1;		//change rate (%)
	state[7] = 10.0;	//number of lookahead steps (number of actions in simulated samples)

	internal_score = 0.0;

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
Copies game state from target game
WARNING: move list is not copied

@param history_copy_start_index Defines the ammount of moves from beginning where to start copying history (useful if most of the target game is same as source)
*/
void Game_DiffSim::Copy_Game_State_From(Game_Engine* source, bool copy_history, int history_copy_start_index)
{
	//copygame state variables
	for(int i = 0; i < board_size; i++){
		state[i] = ((Game_DiffSim*)source)->state[i];
	}
	internal_score = ((Game_DiffSim*)source)->internal_score;
	trajectory_target = ((Game_DiffSim*)source)->trajectory_target;

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

	simulation_timeStep = ((Game_DiffSim*)source)->simulation_timeStep;
	num_timeSteps_perAction = ((Game_DiffSim*)source)->num_timeSteps_perAction;
}

/**
Read target trajectory from file

@param normalize_shift Add value to all samples (linear shift), default 0.0
@param normalize_scale Multiply all samples with scaling factor, default 1.0
*/
void Game_DiffSim::Set_Target_Trajectory_From_File(const char* fileName, double normalize_shift, double normalize_scale)
{
	double** inputData;
	int nRows, nColumns;

	if( Read_Input_File_Double(&inputData,&nRows,&nColumns,fileName) ){
		
#if(!TOM_OUTPUT_TO_MATLAB)
			printf("Game_DiffSim: Set_Target_Trajectory_From_File(): File successfully read: num samples %d, num atributes %d\n",nRows,nColumns);
#endif

		//clear memory
		for(int i = 0; i < maximum_plys; i++)
			delete[] trajectory_target[i];
		delete[] trajectory_target;
		delete[] history_moves;

		//apply input data
		//TODO num atributes/dimensions = nColumns;
		maximum_plys = nRows;
		trajectory_target = inputData;

		//reallocate memory
		history_moves = new int[maximum_plys+1];	//added +1 to avoid access problems at game start, first value in array is -1
		history_moves[0] = -1;

		//normalize
		for(int i = 0; i < nRows; i++)
			for(int j = 0; j < nColumns; j++)
				trajectory_target[i][j] = (trajectory_target[i][j] + normalize_shift)*normalize_scale;

		//reset game not needed AFAIK
		//Game_Reset();
	}
}

/**
Rule defining next player on move: two-player game

@return Next player on move
*/
int Game_DiffSim::Get_Next_Player(int player)
{
	return Get_Next_Player_SinglePlayer(player);
}

/**
@return Passive move (no action)
*/
int Game_DiffSim::Select_Move_Passive()
{
	return 4;
}

/**
@return 1 if game ended, 0 otherwise
*/
int Game_DiffSim::Game_Dynamics(int selected_move)
{
	double prey, pred, d_prey, d_pred;
	double a,b,c,d;
	double stepFact;
	double tmpChk;

	prey = state[0];
	pred = state[1];
	a = state[2];
	b = state[3];
	c = state[4];
	d = state[5];
	stepFact = state[6];

	//execute move
	//--- parameter changes
	if(selected_move < 3){
		if(stepFact > TOMGAME_DIFFSIM_STEPFACT_MIN*TOMGAME_DIFFSIM_STEPFACT_RATE)	//lower limit
			stepFact /= TOMGAME_DIFFSIM_STEPFACT_RATE;
		else
			stepFact = TOMGAME_DIFFSIM_STEPFACT_MIN;
	}else if(selected_move < 6){
		stepFact = stepFact;
	}else{
		if(stepFact < TOMGAME_DIFFSIM_STEPFACT_MAX/TOMGAME_DIFFSIM_STEPFACT_RATE)	//upper limit
			stepFact *= TOMGAME_DIFFSIM_STEPFACT_RATE;
		else
			stepFact = TOMGAME_DIFFSIM_STEPFACT_MAX;
	}
	if(selected_move % 3 == 0){
		if( c*(1-stepFact) > TOMGAME_DIFFSIM_PARAM_C_MIN )
			c = c*(1-stepFact);
		else
			c = TOMGAME_DIFFSIM_PARAM_C_MIN;
	}else if(selected_move % 3 == 1){
		c = c;
	}else{
		if( c*(1+stepFact) < TOMGAME_DIFFSIM_PARAM_C_MAX )
			c = c*(1+stepFact);
		else
			c = TOMGAME_DIFFSIM_PARAM_C_MAX;
	}

	//--- simulation dynamics
    for(int i = 0; i < num_timeSteps_perAction; i++){
        d_prey =  prey * (a - b*pred);
        d_pred = -pred * (c - d*prey);
        prey += d_prey * simulation_timeStep;
        pred += d_pred * simulation_timeStep;
	}

	//check ending condition
	game_ended = Check_Game_Win(selected_move);

	//update avaliable moves
	//TODO

	//update game status
	tmpChk = prey + pred + a + b + c + d + stepFact;
	if (tmpChk - tmpChk == 0.0){	//check for infinity
		state[0] = prey;
		state[1] = pred;
		state[2] = a;
		state[3] = b;
		state[4] = c;
		state[5] = d;
		state[6] = stepFact;
	}

	//get current difference (error) from target trajectory
	trajectory_error = Calculate_Trajectory_Error();

	//update score 
	//-current implementation scores squared error
	//-current implementation is numerically imprecise if big numbers arise, better to use online division and multiplication x = (x*(plys-1) + pred)/plys	
	//if(trajectory_error*trajectory_error < TOMGAME_DIFFSIM_TMP_TARGET_MAX_DIFF)
		internal_score -= trajectory_error*trajectory_error;
		//internal_score -= abs(trajectory_error);
	//else
	//	internal_score += 0;	//no increase in score if error exceeds limit

	//return feedback
	return (int)game_ended;
}

//define goal in these two procedures
void Game_DiffSim::Set_Trajectory_Target()
{
	for(int i = 0; i < maximum_plys; i++)
		trajectory_target[i][0] = TOMGAME_DIFFSIM_TMP_TARGET_PREDATORS;
}

double Game_DiffSim::Calculate_Trajectory_Error()
{
	return state[1] - trajectory_target[current_plys][0];
}

/**
Simulation of dynamic environment with differential equations has no winning conditions
*/
bool Game_DiffSim::Check_Game_Win(int selected_move)
{
	if(current_plys >= maximum_plys-1)
		return true;
	else
		return false;
}

void Game_DiffSim::Calculate_Score()
{
	if(current_plys > 0){
		//normalize score - currently rapresented by standard deviation
		score[0] = sqrt(-internal_score / current_plys);
		//score[0] = (-internal_score / current_plys);
		if( score[0] < TOMGAME_DIFFSIM_TMP_TARGET_MAX_DIFF ){
			score[0] = 1.0 - score[0] / TOMGAME_DIFFSIM_TMP_TARGET_MAX_DIFF;

#if(TOMGAME_DIFFSIM_SCORE_POLY_EXP != 1)
			//exponential score scaling (by polynomial) 
			score[0] = pow(score[0],TOMGAME_DIFFSIM_SCORE_POLY_EXP);
#endif

		}else{
			score[0] = 0.0;
		}
	}
}

/**
Procedure for human move input
*/
int Game_DiffSim::Human_Move_Input()
{
	int move_num, error, inVal1;

	//repeat until selected move is valid
	move_num = -1;
	while(move_num == -1){

		//print to standard output and get input
		fflush(stdout);
		cout << " \t Waiting for player" << current_player+1 << " input (1 - " << maximum_allowed_moves << "): ";

		//check for input error
		error = scanf("%d",&inVal1);
		if(error == 0){
			cin.clear();
			cin.ignore();
			cout << "invalid input!" << endl;

		//check if input value valid
		}else{
			move_num = Human_Move_Translate( inVal1);
			if( Validate_Move(move_num) == false ){
				cout << "invalid move!" << endl;
				move_num = -1;
			}
		}
	}

	//return human input value
	return move_num;
}

int Game_DiffSim::Human_Move_Translate(int human_move)
{
	return (human_move-1);
}

void Game_DiffSim::Output_Board_State()
{	

#if(TOM_OUTPUT_TO_MATLAB)
	
	Output_Board_State_Raw();
	return;

#else

	if(current_plys <= 0){
		printf("\nt %3d  a ---: \t x %5.6f  y %5.6f \t t -----   e ----- \t c %9.6f  s %9.6f",
			current_plys,	//time step
			state[0],	//prey
			state[1],	//predator
			state[4],	//control variable gamma (variable c in Game_Dynamics)
			state[6]	//gamma changing step (stepFact in Game_Dynamics)
			//state[7]	//MCTS single simulation duration (number of actions before forced end)
		);
	}else{
		printf("\nt %3d  a %3d: \t x %5.6f  y %5.6f \t t %5.6f  e %5.6f \t c %9.6f  s %9.6f",
			current_plys,	//time step
			history_moves[current_plys],	//last action
			state[0],	//prey
			state[1],	//predator
			trajectory_target[current_plys-1][0],	//current target value
			trajectory_error,	//current error from desired trajectory
			state[4],	//control variable gamma (variable c in Game_Dynamics)
			state[6]	//gamma changing step (stepFact in Game_Dynamics)
			//state[7]	//MCTS single simulation duration (number of actions before forced end)
		);
	}
	fflush(stdout);

#endif
}

/**
DEFAULT: Output current board (game) state to standard output.
*/
void Game_DiffSim::Output_Board_State_Raw()
{	
#if(!TOM_OUTPUT_TO_MATLAB)
	if(current_plys <= 0){

		printf("\n%3d ---: %5.6f %5.6f  ----- -----  %9.6f %9.6f",
			current_plys,	//time step
			state[0],	//prey
			state[1],	//predator
			state[4],	//control variable gamma (variable c in Game_Dynamics)
			state[6]	//gamma changing step (stepFact in Game_Dynamics)
			//state[7]	//MCTS single simulation duration (number of actions before forced end)
		);

	}else{
		printf("\n%3d %3d: %5.6f %5.6f  %5.6f %5.1f  %9.6f %9.6f",
			current_plys,	//time step
			history_moves[current_plys],	//last action
			state[0],	//prey
			state[1],	//predator
			trajectory_target[current_plys-1][0],	//current target value
			trajectory_error,	//current error from desired trajectory
			state[4],	//control variable gamma (variable c in Game_Dynamics)
			state[6]	//gamma changing step (stepFact in Game_Dynamics)
			//state[7]	//MCTS single simulation duration (number of actions before forced end)
		);
	}
#else
	if(current_plys <= 0){
	}else{
		printf("\n%3d %3d: %5.6f %5.6f  %5.6f %5.9f  %9.6f %9.6f",
			current_plys,	//time step
			history_moves[current_plys],	//last action
			state[0],	//prey
			state[1],	//predator
			trajectory_target[current_plys-1][0],	//current target value
			trajectory_error,	//current error from desired trajectory
			state[4],	//control variable gamma (variable c in Game_Dynamics)
			state[6]	//gamma changing step (stepFact in Game_Dynamics)
			//state[7]	//MCTS single simulation duration (number of actions before forced end)
		);
	}
#endif


	fflush(stdout);

}