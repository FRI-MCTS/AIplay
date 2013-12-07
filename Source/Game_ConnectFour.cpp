//include header
#include "Game_ConnectFour.hpp"

//constructor
Game_ConnectFour::Game_ConnectFour(Game_Engine* source_game)
{

	//game definition
	game_name = "ConnectFour";
	is_deterministic = true;

	//call initialization procedures
	if(source_game == TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY)
		Initialize();
	else
		Copy_Initialize(source_game);
}

//destructor
Game_ConnectFour::~Game_ConnectFour(void)
{
	//release memory space
	Clear_Memory();
}

//create duplicate game
Game_Engine* Game_ConnectFour::Create_Duplicate_Game(bool copy_state, bool copy_history)
{
	//create new game by copying settings of this game
	Game_ConnectFour* new_game = new Game_ConnectFour(this);

	//set current state of new game
	if(copy_state)
		new_game->Copy_Game_State_From(this, copy_history);
	else
		new_game->Game_Reset();
		
	//return pointer to new game object
	return new_game;
}

void Game_ConnectFour::Init_Settings()
{
	//init game settings
	board_length = TOMGAME_FOURROW_BOARD_LENGTH;
	board_height = TOMGAME_FOURROW_BOARD_HEIGHT;
	board_size = board_length * board_height;
	number_players = 2;
	maximum_allowed_moves = board_length;
	maximum_plys = board_size;
	win_connected_pieces = TOMGAME_FOURROW_WIN_CONNECTED_PIECES;
	param_score_win = TOMGAME_FOURROW_SCORE_WIN;
	param_score_lose = TOMGAME_FOURROW_SCORE_LOSE;
	param_score_draw = TOMGAME_FOURROW_SCORE_DRAW;

	//init debug settings
	show_warnings = TOM_DEBUG;
}

//copy settings from source_game
void Game_ConnectFour::Copy_Settings(Game_Engine* source_game)
{

	//general game settings
	board_length = source_game->board_length;
	board_height = source_game->board_height;
	board_size = source_game->board_size;
	number_players = source_game->number_players;
	maximum_allowed_moves = source_game->maximum_allowed_moves;
	maximum_plys = source_game->maximum_plys;

	//general debug settings
	show_warnings = source_game->show_warnings;

	//game-specific settings
	win_connected_pieces = ((Game_ConnectFour*)source_game)->win_connected_pieces;

}

/**
@return 1 if game ended, 0 otherwise
*/
int Game_ConnectFour::Game_Dynamics(int selected_move)
{
	int piecePosition;

	//execute move - find where piece lands
	piecePosition = selected_move;
	while( (piecePosition+board_length < board_size)&&(board_state[piecePosition+board_length] == 0) )
		piecePosition += board_length;

	//put piece on board
	board_state[piecePosition] = current_player+1;

	//check ending condition
	game_ended = Check_Game_End(piecePosition);

	//update avaliable moves
	if(board_state[selected_move] != 0){
		current_moves[0][selected_move] = false;
		current_moves[1][selected_move] = false;
		current_number_moves[0]--;
		current_number_moves[1]--;
	}
	
	//update game status
	current_player = Get_Next_Player();

	//return feedback
	return (int)game_ended;
}


/**
Procedure for human move input
*/
int Game_ConnectFour::Human_Move_Input()
{
	int move_num, error, inVal1;

	//repeat until selected move is valid
	move_num = -1;
	while(move_num == -1){

		//print to standard output and get input
		fflush(stdout);
		cout << "Waiting for player" << current_player+1 << " input (X): ";

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

/**
Transform human input value in game move appropriate input
*/
int Game_ConnectFour::Human_Move_Translate(int human_move)
{
	return (human_move-1);
}

/**
Output current game state to standard output.
*/
void Game_ConnectFour::Output_Board_State()
{	
	int piecePosition;
	
	//find where last piece landed
	if(current_plys > 0){
		piecePosition = history_moves[current_plys];
		while( (piecePosition < board_size)&&(board_state[piecePosition] == 0) )
			piecePosition += board_length;
	}else
		piecePosition = -1;

	printf("\n   ");
	for(int i = 0; i < board_length; i++)
		printf(" %d",i+1);
	printf("\n    ");
	if(current_plys > 0){
		for(int i = 0; i < history_moves[current_plys]; i++)
			printf("  ");
		printf("%c", output_board_last_move_char);
	}
	for(int i = 0, k = 0; i < board_height; i++, k += board_length){
		printf("\n%2d ",i+1);
		for(int j = 0; j < board_length; j++){
			if(k+j != piecePosition)
				printf(" %c", output_board_lookup_char[board_state[k+j]+1]);
			else{
				if(j < board_length-1){
					printf("%c%c%c%c", output_board_last_move_char, output_board_lookup_char[board_state[k+j]+1], output_board_last_move_char, output_board_lookup_char[board_state[k+j+1]+1]);
					j++;
				}else
					printf("%c%c%c", output_board_last_move_char, output_board_lookup_char[board_state[k+j]+1], output_board_last_move_char);
			}
		}
	}
	printf("\n");

#if(!TOM_DEBUG)
	printf("\nPLY: %2d\n",current_plys);
#else
	printf("\nPLY: %2d \t Last move: %d\n",current_plys,history_moves[current_plys]);
#endif

	printf("\n");
	fflush(stdout);

}