//include header
#include "Game_Hex.hpp"
#include <set>

//constructor
Game_Hex::Game_Hex(Game_Engine* source_game)
{

	//game definition
	game_name = "Hex";
	is_deterministic = true;

	//call initialization procedures
	if(source_game == TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY)
		Initialize();
	else
		Copy_Initialize(source_game);
}

//destructor
Game_Hex::~Game_Hex(void)
{
	//release memory space
	Clear_Memory();
}

//create duplicate game
Game_Engine* Game_Hex::Create_Duplicate_Game(bool copy_state, const bool copy_history)
{
	//create new game by copying settings of this game
	Game_Hex* new_game = new Game_Hex(this);

	//set current state of new game
	if(copy_state)
		new_game->Copy_Game_State_From(this, copy_history);
	else
		new_game->Game_Reset();
		
	//return pointer to new game object
	return new_game;
}

//init game settings
void Game_Hex::Init_Settings()
{
	
	//general game settings
	board_length = TOMGAME_HEX_BOARD_LENGTH;
	board_height = TOMGAME_HEX_BOARD_HEIGHT;
	big_board_length = TOMGAME_HEX_BOARD_LENGTH + 2;
	big_board_height = TOMGAME_HEX_BOARD_HEIGHT + 2;
	board_size = board_length * board_height;
	big_board_size = big_board_length * big_board_height;
	number_players = 2;
	maximum_allowed_moves = board_size;
	maximum_plys = board_size;
	param_score_win = TOMGAME_HEX_SCORE_WIN;
	param_score_lose = TOMGAME_HEX_SCORE_LOSE;
	//param_score_draw = TOMGAME_HEX_SCORE_DRAW;

	//general debug settings
	show_warnings = TOM_DEBUG;

	//game-specific settings


}

//copy settings from source_game
void Game_Hex::Copy_Settings(Game_Engine* source_game)
{

	//general game settings
	board_length = source_game->board_length;
	board_height = source_game->board_height;
	big_board_length = ((Game_Hex*)source_game)->big_board_length;
	big_board_height = ((Game_Hex*)source_game)->big_board_height;
	board_size = source_game->board_size;
	big_board_size = ((Game_Hex*)source_game)->big_board_size;
	number_players = source_game->number_players;
	maximum_allowed_moves = source_game->maximum_allowed_moves;
	maximum_plys = source_game->maximum_plys;
	param_score_win = source_game->param_score_win;
	param_score_lose = source_game->param_score_lose;
	//param_score_draw = source_game->param_score_draw;

	//general debug settings
	show_warnings = source_game->show_warnings;

	//game-specific settings

}

void Game_Hex::Allocate_Memory()
{
	//allocate resources - game state
	board_state = new char[board_size];
	big_board_state = new char[big_board_size];
	current_number_moves = new int[number_players];
	current_moves = new bool*[number_players];
	current_moves_list = new int*[number_players];
	for(int i = 0; i < number_players; i++){
		current_moves[i] = new bool[maximum_allowed_moves];
		current_moves_list[i] = new int[maximum_allowed_moves];
	}
	score = new double[number_players];

	//init game state structures
	for(int i = 0; i < big_board_length; i++){	//top and bottom: player 2
		big_board_state[i] = -2;	
		big_board_state[i+big_board_size-big_board_length] = -3;
	}
	for(int i = 1; i < big_board_height-1; i++){	//left and right: player 1
		big_board_state[i*big_board_length] = 2;	
		big_board_state[(i+1)*big_board_length-1] = 3;
	}

	//allocate resources - history
	history_moves = new int[maximum_plys+1];	//added +1 to avoid access problems at game start, first value in array is -1
	history_moves[0] = -1;

	//allocate resources - visualization
	output_board_lookup_char = new char[number_players+2];

	//define constant values - visualization
	output_board_lookup_char[0] = 'E';
	output_board_lookup_char[1] = '.';
	if(number_players > 0)
		output_board_lookup_char[2] = 'X';	//player 1 must connect left and right edges
	if(number_players > 1)
		output_board_lookup_char[3] = 'O';	//player 2 must connect top and bottom edges
	for(int i = 4; i < number_players; i++)
		output_board_lookup_char[i] = '?';	//non-specified character

	//DEBUG
	//for(int i = 0; i < maximum_allowed_moves+1; i++)
	//	printf(" %d",history_moves[i]);
	//printf("\n");
}

void Game_Hex::Clear_Memory()
{

	//release memory space
	delete[] board_state;
	delete[] big_board_state;
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
void Game_Hex::Game_Reset()
{
	//reset game state variables
	for(int i = 0; i < board_size; i++){
		board_state[i] = 0;
	}
	for(int i = big_board_length; i < big_board_size; i += big_board_length){
		for(int j = 1; j < big_board_length-1; j++){
			big_board_state[i+j] = 0;
		}
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
void Game_Hex::Copy_Game_State_From(Game_Engine* source, const bool copy_history, int history_copy_start_index)
{
	//copygame state variables
	for(int i = 0; i < board_size; i++){
		board_state[i] = source->board_state[i];
	}
	for(int i = big_board_length; i < big_board_size; i += big_board_length){
		for(int j = 1; j < big_board_length-1; j++){
			big_board_state[i+j] = ((Game_Hex*)source)->big_board_state[i+j];
		}
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
int Game_Hex::Get_Next_Player(int player)
{
	return Get_Next_Player_TwoPlayer(player);
}

/**
@return 1 if game ended, 0 otherwise
*/
int Game_Hex::Game_Dynamics(int selected_move)
{

	//execute move
	board_state[selected_move] = current_player+1;

	//check ending condition
	game_ended = Check_Game_End(selected_move);

	//update avaliable moves
	current_moves[0][selected_move] = false;
	current_number_moves[0]--;

	//THE SWAP RULE (allowed only to the 2nd player after the first move of 1st player)
	if((current_plys == 0)&&(TOMGAME_HEX_SWAP_RULE))
		;//current_moves[1][selected_move] = true;
	else{
		current_moves[1][selected_move] = false;
		current_number_moves[1]--;
	}
	if((current_plys == 1)&&(TOMGAME_HEX_SWAP_RULE)){
		current_moves[1][ history_moves[0] ] = false;
		current_number_moves[1]--;
	}

	//update game status
	current_player = Get_Next_Player();	//by default: alternating 2-player game

	//return feedback
	return (int)game_ended;
}

/**
Board winning positions for Hex: checks for connected pieces from one side to the opposite
*/
bool Game_Hex::Check_Game_Win(int selected_move)
{
	////VALUES IN big_board_length
	//0 - empty intersection
	//1 - player1 piece not connected to left or right border
	//2 - player1 piece connected to left border
	//3 - player1 piece connected to right border
	//-1 - player2 piece not connected to top or bottom border
	//-2 - player2 piece connected to top border
	//-3 - player2 piece connected to bottom border

	int x,y,current,xb,yb;
	int tempL, tempR, tempT, tempB;

	//optimization v1
	x = (int)(selected_move % board_length);
	y = (int)(selected_move / board_length);

	xb = x+1;
	yb = y+1;
	current = xb + yb * big_board_length;

	sosedje[0] = current - big_board_length;
	sosedje[1] = current - big_board_length + 1;
	sosedje[2] = current - 1;
	sosedje[3] = current + 1;
	sosedje[4] = current + big_board_length - 1;
	sosedje[5] = current + big_board_length;

	//player 1 has L and R
	if(current_player+1 == 1){
		if(xb == 1){
			for(int i = 0; i < 6; i++){
				if(big_board_state[sosedje[i]] == 3)
					//win
					return true;
			} 
			//no win
			big_board_state[current] = 1;

			Flood(current, 1, 2);

			return false;
		}
		else if(xb == (big_board_length-2)){
			for(int i = 0; i < 6; i++){
				if(big_board_state[sosedje[i]] == 2)
					//win
					return true;
			} 
			//no win
			big_board_state[current] = 1;
			
			Flood(current, 1, 3);

			return false;
		}
		else{
			tempL=0;
			tempR=0;
			for(int i = 0; i < 6; i++){
				if(big_board_state[sosedje[i]] == 2)
					tempL=1;
				else if(big_board_state[sosedje[i]] == 3)
					tempR=1;

			} 
			if((tempL == 1) && (tempR == 1))
				//win
				return true;
			else{
				//no win
				big_board_state[current] = 1;
				if(tempL == 1)
					Flood(current, 1, 2);
				else if(tempR == 1)
					Flood(current, 1, 3);

				return false;
			}
		}
	}

	//player 2 has T and B
	else{
		if(yb == 1){
			for(int i = 0; i < 6; i++){
				if(big_board_state[sosedje[i]] == -3)
					//win
					return true;
			} 
			//no win
			big_board_state[current] = -1;

			Flood(current, -1, -2);

			return false;
		}
		else if(yb == big_board_height-2){
			for(int i = 0; i < 6; i++){
				if(big_board_state[sosedje[i]] == -2)
					//win
					return true;
			} 
			//no win
			big_board_state[current] = -1;
			
			Flood(current, -1, -3);

			return false;

		}
		else{
			tempT=0;
			tempB=0;
			for(int i = 0; i < 6; i++){
				if(big_board_state[sosedje[i]] == -2)
					tempT=1;
				else if(big_board_state[sosedje[i]] == -3)
					tempB=1;

			} 
			if((tempT == 1) && (tempB == 1))
				//win
				return true;
			else{
				//no win
				big_board_state[current] = -1;
				if(tempT == 1)
					Flood(current, -1, -2);
				else if(tempB == 1)
					Flood(current, -1, -3);

				return false;
			}
		}
	}
}

void Game_Hex::Flood(int position, const int oldValue, const int newValue)
{
    if(big_board_state[position] == oldValue)
    {
		big_board_state[position] = newValue;
        Flood(position - big_board_length,oldValue,newValue);
        Flood(position - big_board_length + 1,oldValue,newValue);
        Flood(position - 1,oldValue,newValue);
        Flood(position + 1,oldValue,newValue);
        Flood(position + big_board_length - 1,oldValue,newValue);
        Flood(position + big_board_length,oldValue,newValue);
    }
}

int Game_Hex::Human_Move_Translate(int human_move)
{
	return (human_move-1-board_length);
}


/**
DEFAULT: Output current board (game) state to standard output.
*/
void Game_Hex::Output_Board_State()
{	
	//Output_Board_State_Raw();
	//return;

	gmp->Print("\n   ");
	for(int i = 0; i < board_length; i++)
		gmp->Print(" %d",i+1);
	gmp->Print("\n");
	for(int i = 0, k = 0; i < board_height; i++, k += board_length){
		gmp->Print("\n%2d ",i+1);
		//-----ADD-----corrected output for hex
		for(int temp = 0; temp < i; temp++)
			gmp->Print(" ");
		//-----END-----
		for(int j = 0; j < board_length; j++){
			if(k+j != history_moves[current_plys])
				gmp->Print(" %c", output_board_lookup_char[board_state[k+j]+1]);
			else{
				if(j < board_length-1){
					gmp->Print("%c%c%c%c", output_board_last_move_char, output_board_lookup_char[board_state[k+j]+1], output_board_last_move_char, output_board_lookup_char[board_state[k+j+1]+1]);
					j++;
				}else
					gmp->Print("%c%c%c", output_board_last_move_char, output_board_lookup_char[board_state[k+j]+1], output_board_last_move_char);
			}
		}
	}
	gmp->Print("\n");

#if(!TOM_DEBUG)
	gmp->Print("\nPLY: %2d\n",current_plys);
#else
	gmp->Print("\nPLY: %2d \t Last move: %d\n",current_plys,history_moves[current_plys]);
#endif

	gmp->Print("\n");
	fflush(stdout);
}

/**
DEFAULT: Output current board (game) state to standard output.
*/
void Game_Hex::Output_Board_State_Raw()
{	
	gmp->Print("\n   ");
	for(int i = 0; i < board_length; i++)
		gmp->Print(" %d",i+1);
	gmp->Print("\n");
	for(int i = 0, k = 0; i < board_height; i++, k += board_length){
		gmp->Print("\n%2d ",i+1);
		//-----ADD-----corrected output for hex
		for(int temp = 0; temp < i; temp++)
			gmp->Print(" ");
		//-----END-----
		for(int j = 0; j < board_length; j++){
			if(k+j != history_moves[current_plys])
				gmp->Print(" %d", board_state[k+j]);
			else{
				if(j < board_length-1){
					gmp->Print("+%d+%d", board_state[k+j], board_state[k+j+1]);
					j++;
				}else
					gmp->Print("+%d+", board_state[k+j]);
			}
		}
	}
	gmp->Print("\n");

	gmp->Print("\nPLY: %2d \t Last move: %d\n",current_plys,history_moves[current_plys]);

	gmp->Print("\n");
	fflush(stdout);

}