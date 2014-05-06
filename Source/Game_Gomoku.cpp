//include header
#include "Game_Gomoku.hpp"

//constructor
Game_Gomoku::Game_Gomoku(Game_Engine* source_game)
{

	//game definition
	game_name = "Gomoku";
	is_deterministic = true;

	//call initialization procedures
	if(source_game == TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY)
		Initialize();
	else
		Copy_Initialize(source_game);
}

//destructor
Game_Gomoku::~Game_Gomoku(void)
{
	//release memory space
	Clear_Memory();
}

//create duplicate game
Game_Engine* Game_Gomoku::Create_Duplicate_Game(bool copy_state, bool copy_history)
{
	//create new game by copying settings of this game
	Game_Gomoku* new_game = new Game_Gomoku(this);

	//set current state of new game
	if(copy_state)
		new_game->Copy_Game_State_From(this, copy_history);
	else
		new_game->Game_Reset();
		
	//return pointer to new game object
	return new_game;
}

//init game settings
void Game_Gomoku::Init_Settings()
{
	
	//general game settings
	board_length = TOMGAME_GOMOKU_BOARD_LENGTH;
	board_height = TOMGAME_GOMOKU_BOARD_HEIGHT;
	board_size = board_length * board_height;
	number_players = 2;
	maximum_allowed_moves = board_size;
	maximum_plys = board_size;
	param_score_win = TOMGAME_GOMOKU_SCORE_WIN;
	param_score_lose = TOMGAME_GOMOKU_SCORE_LOSE;
	param_score_draw = TOMGAME_GOMOKU_SCORE_DRAW;

	//general debug settings
	show_warnings = TOM_DEBUG;

	//game-specific settings
	win_connected_pieces = TOMGAME_GOMOKU_WIN_CONNECTED_PIECES;

}

//copy settings from source_game
void Game_Gomoku::Copy_Settings(Game_Engine* source_game)
{

	//general game settings
	board_length = source_game->board_length;
	board_height = source_game->board_height;
	board_size = source_game->board_size;
	number_players = source_game->number_players;
	maximum_allowed_moves = source_game->maximum_allowed_moves;
	maximum_plys = source_game->maximum_plys;
	param_score_win = source_game->param_score_win;
	param_score_lose = source_game->param_score_lose;
	param_score_draw = source_game->param_score_draw;

	//general debug settings
	show_warnings = source_game->show_warnings;

	//game-specific settings
	win_connected_pieces = ((Game_Gomoku*)source_game)->win_connected_pieces;

}

void Game_Gomoku::Allocate_Memory()
{
	//allocate resources - game state
	board_state = new char[board_size];
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

	//allocate resources - visualization
	output_board_lookup_char = new char[number_players+2];

	//define constant values - visualization
	output_board_lookup_char[0] = 'E';
	output_board_lookup_char[1] = '.';
	if(number_players > 0)
		output_board_lookup_char[2] = 'X';
	if(number_players > 1)
		output_board_lookup_char[3] = 'O';
	for(int i = 4; i < number_players; i++)
		output_board_lookup_char[i] = '?';	//non-specified character

	//DEBUG
	//for(int i = 0; i < maximum_allowed_moves+1; i++)
	//	printf(" %d",history_moves[i]);
	//printf("\n");
}

void Game_Gomoku::Clear_Memory()
{

	//release memory space
	delete[] board_state;
	delete[] current_number_moves;
	for(int i = 0; i < number_players; i++){
		delete[] current_moves[i];
		delete[] current_moves_list[i];
	}
	delete[] current_moves;
	delete[] current_moves_list;
	delete[] score;
	delete[] history_moves;
	delete[] output_board_lookup_char;
}

/**
Reset to initial values (restart game)
WARNING: move list is not created
*/
void Game_Gomoku::Game_Reset()
{
	//reset game state variables
	for(int i = 0; i < board_size; i++){
		board_state[i] = 0;
	}
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
void Game_Gomoku::Copy_Game_State_From(Game_Engine* source, const bool copy_history, int history_copy_start_index)
{
	//copygame state variables
	for(int i = 0; i < board_size; i++){
		board_state[i] = source->board_state[i];
	}
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
}

/**
Rule defining next player on move: two-player game

@return Next player on move
*/
int Game_Gomoku::Get_Next_Player(int player)
{
	return Get_Next_Player_TwoPlayer(player);
}

/**
@return 1 if game ended, 0 otherwise
*/
int Game_Gomoku::Game_Dynamics(int selected_move)
{

	//execute move
	board_state[selected_move] = current_player+1;

	//check ending condition
	game_ended = Check_Game_End(selected_move);

	//update avaliable moves
	current_moves[0][selected_move] = false;
	current_moves[1][selected_move] = false;
	current_number_moves[0]--;
	current_number_moves[1]--;
	
	//update game status
	current_player = Get_Next_Player();	//by default: alternating 2-player game

	//return feedback
	return (int)game_ended;
}

/**
Board winning positions for Gomoku: checks for a number (win_connected_pieces) of connected pieces from last placed piece (selected move) in horizontal, vertical and diagonal direction
*/
bool Game_Gomoku::Check_Game_Win(int selected_move)
{
	int x,y;
	int xs,xe,ys,ye;
	int rx, ry;
	int connected;

	y = (int)(selected_move / board_length);
	x = (int)(selected_move % board_length);

	xs = max(x - (win_connected_pieces-1), 0);
	xe = min(x + (win_connected_pieces-1), board_length-1);
	ys = max(y - (win_connected_pieces-1), 0);
	ye = min(y + (win_connected_pieces-1), board_height-1);
	
	rx = selected_move - x;
	ry = selected_move - (y-ys)*board_length;

	//check winning condition in all directions
	//horizontal
	connected = 1;
	for(int i = xs; i < x; i++){
		if( board_state[rx+i] == (current_player+1) )
			connected++;
		else
			connected = 1;
	}
	if(connected == win_connected_pieces)
		return true;
	for(int i = x+1; i <= xe; i++){
		if( board_state[rx+i] == (current_player+1) ){
			connected++;
			if(connected == win_connected_pieces)
				return true;
		}else{
			i = xe;	//break the loop, win combination not possible
		}
	}

	//vertical
	connected = 1;
	for(int i = ys, t = ry; i < y; i++, t += board_length){
		if( board_state[t] == (current_player+1) )
			connected++;
		else
			connected = 1;
	}
	if(connected == win_connected_pieces)
		return true;
	for(int i = y+1, t = selected_move; i <= ye; i++){
		t += board_length;
		if( board_state[t] == (current_player+1) ){
			connected++;
			if(connected == win_connected_pieces)
				return true;
		}else{
			i = ye;	//break the loop, win combination not possible
		}
	}

	//diagonal left to right
	connected = 1;
	for(int i = 1, t = selected_move - min(x-xs, y-ys)*(board_length+1); i <= min(x-xs, y-ys); i++, t += (board_length+1)){
		if( board_state[t] == (current_player+1) )
			connected++;
		else
			connected = 1;
	}
	if(connected == win_connected_pieces)
		return true;
	for(int i = 1, t = selected_move; i <= min(xe-x, ye-y); i++){
		t += (board_length+1);
		if( board_state[t] == (current_player+1) ){
			connected++;
			if(connected == win_connected_pieces)
				return true;
		}else{
			i = ye;	//break the loop, win combination not possible
		}
	}

	//diagonal right to left
	connected = 1;
	for(int i = 1, t = selected_move - min(xe-x, y-ys)*(board_length-1); i <= min(xe-x, y-ys); i++, t += (board_length-1)){
		if( board_state[t] == (current_player+1) )
			connected++;
		else
			connected = 1;
	}
	if(connected == win_connected_pieces)
		return true;
	for(int i = 1, t = selected_move; i <= min(x-xs, ye-y); i++){
		t += (board_length-1);
		if( board_state[t] == (current_player+1) ){
			connected++;
			if(connected == win_connected_pieces)
				return true;
		}else{
			i = ye;	//break the loop, win combination not possible
		}
	}

	//no win
	return false;

}

int Game_Gomoku::Human_Move_Translate(int human_move)
{
	return (human_move-1-board_length);
}
