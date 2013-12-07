//include header
#include "Game_TicTacToe.hpp"

//constructor
Game_TicTacToe::Game_TicTacToe(Game_Engine* source_game)
{

	//game definition
	game_name = "TicTacToe";
	is_deterministic = true;

	//call initialization procedures
	if(source_game == TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY)
		Initialize();
	else
		Copy_Initialize(source_game);
}

//destructor
Game_TicTacToe::~Game_TicTacToe(void)
{
	//release memory space
	Clear_Memory();
}

//create duplicate game
Game_Engine* Game_TicTacToe::Create_Duplicate_Game(bool copy_state, bool copy_history)
{
	//create new game by copying settings of this game
	Game_TicTacToe* new_game = new Game_TicTacToe(this);

	//set current state of new game
	if(copy_state)
		new_game->Copy_Game_State_From(this, copy_history);
	else
		new_game->Game_Reset();
		
	//return pointer to new game object
	return new_game;
}

void Game_TicTacToe::Init_Settings()
{
	//init game settings
	board_length = TOMGAME_TICTACTOE_BOARD_LENGTH;
	board_height = TOMGAME_TICTACTOE_BOARD_LENGTH;
	board_size = board_length * board_height;
	number_players = 2;
	maximum_allowed_moves = board_size;
	maximum_plys = board_size;
	win_connected_pieces = TOMGAME_TICTACTOE_WIN_CONNECTED_PIECES;
	param_score_win = TOMGAME_TICTACTOE_SCORE_WIN;
	param_score_lose = TOMGAME_TICTACTOE_SCORE_LOSE;
	param_score_draw = TOMGAME_TICTACTOE_SCORE_DRAW;
	 
	//init debug settings
	show_warnings = TOM_DEBUG;
}

//hardcoded win combinations for 3x3 tic tac toe
bool Game_TicTacToe::Check_Game_Win(int selected_move)
{
	bool win = false;

	if(selected_move == 0){
		if	(
				((board_state[1] == current_player+1) && (board_state[2] == current_player+1)) ||
				((board_state[3] == current_player+1) && (board_state[6] == current_player+1)) ||
				((board_state[4] == current_player+1) && (board_state[8] == current_player+1))
			)
			win = true;
	} else if (selected_move == 1){
		if	(
				((board_state[0] == current_player+1) && (board_state[2] == current_player+1)) ||
				((board_state[4] == current_player+1) && (board_state[7] == current_player+1))
			)
			win = true;
	} else if (selected_move == 2){
		if	(
				((board_state[0] == current_player+1) && (board_state[1] == current_player+1)) ||
				((board_state[4] == current_player+1) && (board_state[6] == current_player+1)) ||
				((board_state[5] == current_player+1) && (board_state[8] == current_player+1))
			)
			win = true;
	} else if (selected_move == 3){
		if	(
				((board_state[0] == current_player+1) && (board_state[6] == current_player+1)) ||
				((board_state[4] == current_player+1) && (board_state[5] == current_player+1))
			)
			win = true;
	} else if (selected_move == 4){
		if	(
				((board_state[0] == current_player+1) && (board_state[8] == current_player+1)) ||
				((board_state[1] == current_player+1) && (board_state[7] == current_player+1)) ||
				((board_state[2] == current_player+1) && (board_state[6] == current_player+1)) ||
				((board_state[3] == current_player+1) && (board_state[5] == current_player+1))
			)
			win = true;
	} else if (selected_move == 5){
		if	(
				((board_state[2] == current_player+1) && (board_state[8] == current_player+1)) ||
				((board_state[3] == current_player+1) && (board_state[4] == current_player+1))
			)
			win = true;
	} else if (selected_move == 6){
		if	(
				((board_state[0] == current_player+1) && (board_state[3] == current_player+1)) ||
				((board_state[2] == current_player+1) && (board_state[4] == current_player+1)) ||
				((board_state[7] == current_player+1) && (board_state[8] == current_player+1))
			)
			win = true;
	} else if (selected_move == 7){
		if	(
				((board_state[1] == current_player+1) && (board_state[4] == current_player+1)) ||
				((board_state[6] == current_player+1) && (board_state[8] == current_player+1))
			)
			win = true;
	} else if (selected_move == 8){
		if	(
				((board_state[2] == current_player+1) && (board_state[5] == current_player+1)) ||
				((board_state[0] == current_player+1) && (board_state[4] == current_player+1)) ||
				((board_state[6] == current_player+1) && (board_state[7] == current_player+1))
			)
			win = true;
	}

	return win;
}
