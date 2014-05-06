//include circular reference
#include "Player_Engine.hpp"

//include header
#include "Game_Engine.hpp"

//allocate memory and initialize variables
void Game_Engine::Initialize()
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

//initialize values common to all derivate objects
void Game_Engine::Initialize_Common()
{
	//game state
	players = NULL;

	//visualization variables
	output_board_last_move_char = '+';

	//init global experiment/testing counter
	experiment_repeat_index = 0;
}

/**
Initializes game object by copying settings from source_game and allocates resources.
Does not RESET game state.
*/
void Game_Engine::Copy_Initialize(Game_Engine* source_game)
{

	//initialize values common to all derivate objects
	Initialize_Common();

	//copy settings
	Copy_Settings(source_game);

	//allocate resources
	Allocate_Memory();

}

void Game_Engine::Settings_Apply_Changes()
{
	Clear_Memory();
	Allocate_Memory();
	Game_Reset();
}

void Game_Engine::Settings_Reset()
{
	Clear_Memory();
	Initialize();
}


/**
Support function for selecting a random move. Notice that srand(time(NULL)) should be called at main program start.

@return Number of selected move
*/
int Game_Engine::Select_Move_Random()
{
	int r;

#if(TOM_DEBUG)
	if(current_number_moves[current_player] <= 0){
		gmp->Print("!!ERROR!! Game Engine: Select_Move_Random(): no actions allowed, player %d, number of plays %d\n",current_player,current_plys);
		return -1;
	}
#endif

#if(TOMGAME_ENGINE_DISABLE_RANDOM)
	r = 0;
#else
	r = (int)( (rand()/(float)(RAND_MAX+1)) * (current_number_moves[current_player]) );
#endif

	return Select_Move_Unsafe(r);
}

/**
Support function for selecting a passive move. Must be implemented by each game, otherwise warning is returned and move 0.

@return 0
*/
int Game_Engine::Select_Move_Passive()
{
	
#if(TOM_DEBUG)
	if(current_number_moves[current_player] <= 0){
		gmp->Print("!!ERROR!! Game Engine: Select_Move_Passive(): no actions allowed, player %d, number of plays %d\n",current_player,current_plys);
		return -1;
	}
#endif

	gmp->Print("!!WARNING!! Game Engine: Select_Move_Passive(): game '%s' passive move not defined, selecting action 0\n", (this->game_name).c_str());
	return 0;
}

/**
Support function for creating a list of moves
*/
void Game_Engine::Make_Moves_List()
{
	int num_found_moves = 0;

	//check all moves until all available have been found
	for(int i = 0; i < board_size, num_found_moves < current_number_moves[current_player]; i++){

		//memorize available moves
		if(current_moves[current_player][i]){
			current_moves_list[current_player][num_found_moves] = i;
			num_found_moves++;
		}

	}
}

/**
UNSAFE: Support function for selecting a move from the set of available moves, without validity check

@param serial_number Which move to pick from the set of available moves starting from lowest
@return Number of selected move
*/
int Game_Engine::Select_Move_Unsafe(int serial_number)
{
	
	//find available action
	int selected = -1;
	for(int i = 0; i < maximum_allowed_moves; i++){
		if(current_moves[current_player][i] == true){
			if(serial_number == 0){
				selected = i;
				i = maximum_allowed_moves;
			}
			serial_number--;
		}
	}

	//check for errors - DEBUG
	//if(selected == -1){
	//	if(disable_warnings == 0){
	//		gmp->Print("! ERROR: Go Game Engine: serial number to large by %d, player %d, number of plays since game start %d, num moves %d\n",serial_number,current_player,number_plys,current_number_moves[current_player]);
	//		output_state_to_STD();
	//		for(int i = 0; i < board_height; i++){
	//			for(int j = 0; j < board_length; j++)
	//				gmp->Print("%d ",current_moves[current_player][i*board_length+j+1]);
	//			gmp->Print("\n");
	//		}

	//		getc(stdin);
	//	}
	//}
	//gmp->Print("sel: %d, p: %d\n",selected, current_player+1);	//DEBUG

	return selected;
}


/**
Support function for selecting a move from the set of available moves, with validity check

@param serial_number Which move to pick from the set of available moves starting from lowest
@return Number of selected move, returns -1 if error
*/
int Game_Engine::Select_Move(int serial_number)
{
	
	//check if move is safe and valid
	if( (serial_number >= 0) && (serial_number < current_number_moves[current_player]) ){

		return Select_Move_Unsafe(serial_number);

	//move is invalid
	}else{
		if(show_warnings){
			gmp->Print("!!ERROR!! Game Engine: Select_Move(): move serial label invalid, player %d, move %d, number of plays %d\n",current_player,serial_number,current_plys);
		}
		return -1;
	}

}

//DEFAULT: move validation
bool Game_Engine::Validate_Move(int selected_move)
{
	if(	(selected_move >= 0) &&
		(selected_move < maximum_allowed_moves) &&
		(current_moves[current_player][selected_move])
	  )
		return true;
	else
		return false;
}

/**
DEFAULT: Check validity and play selected move.
*/
int Game_Engine::Play_Move(int selected_move)
{
	
	//check end game
	if(game_ended){

		//illegal move selected
		if(show_warnings){
			gmp->Print("!!ERROR!! Game Engine: Play_Move(): moves not allowed after game end, player %d, move %d, ply %d\n",current_player,selected_move,current_plys);
		}
		return -1;

	//check move validity
	}else if( Validate_Move(selected_move) == false){

		//illegal move selected
		if(show_warnings){
			gmp->Print("!!ERROR!! Game Engine: Play_Move(): wrong move, player %d, move %d, ply %d\n",current_player,selected_move,current_plys);
		}
		return -1;

	}else{

		//play legal move
		return Play_Move_Unsafe(selected_move);

	}

}

/**
UNSAFE: Play selected move without checking validity.

@return 1 if game ended, 0 otherwise
*/
int Game_Engine::Play_Move_Unsafe(int selected_move)
{
	
	int feedback;

	//execute game dynamics by applying selected move
	feedback = Game_Dynamics(selected_move);

	//update history info
	current_plys++;
	history_moves[current_plys] = selected_move;

	////--DEBUG
	//if(feedback){
	//if(current_plys >= maximum_allowed_moves){
	//	//Output_Moves_History();
	//	Output_Board_State();
	//}
	////--END-DEBUG

	return feedback;
}


int Game_Engine::Get_Next_Player_SinglePlayer(int p)
{
	return p;
}

int Game_Engine::Get_Next_Player_TwoPlayer(int p)
{
	return (1-p);
}
int Game_Engine::Get_Next_Player_MultiPlayer(int p)
{
	if(p+1 == number_players)
		return 0;
	else
		return (p+1);

}
int Game_Engine::Get_Previous_Player_MultiPlayer(int p)
{
	if(p-1 < 0)
		return (number_players-1);
	else
		return (p-1);
}


/**
DEFAULT: Check ending condition and set score
@return TRUE if game ended, FALSE otherwise
*/
bool Game_Engine::Check_Game_End_SinglePlayer(int position)
{
	
	//game ends because of a win
	if( Check_Game_Win(position) ){

		//works for one players only!
		score[0] = param_score_win;
		return true;

	//game ends because of ply limit
	}else if(current_plys >= maximum_plys-1){ // -1 is because current_plays was not yet increased after last move

		for(int i = 0; i < number_players; i++)
			score[i] = param_score_draw;
		return true;
	
	//game does not end yet
	}else

		return false;

}
bool Game_Engine::Check_Game_End_TwoPlayer(int position)
{
	
	//game ends because of a win
	if( Check_Game_Win(position) ){

		//works for two players only!
		score[current_player] = param_score_win;
		score[1-current_player] = param_score_lose;

		return true;

	//game ends because of ply limit
	}else if(current_plys >= maximum_plys-1){ // -1 is because current_plays was not yet increased after last move

		for(int i = 0; i < number_players; i++)
			score[i] = param_score_draw;
		return true;
	
	//game does not end yet
	}else

		return false;

}
bool Game_Engine::Check_Game_End_MultiPlayer(int position)
{
	
	//game ends because of a win
	if( Check_Game_Win(position) ){

		//works for arbitrary number of players
		for(int i = 0; i < number_players; i++)
			score[i] = param_score_lose;
		score[current_player] = param_score_win;

		return true;

	//game ends because of ply limit
	}else if(current_plys >= maximum_plys-1){ // -1 is because current_plays was not yet increased after last move

		for(int i = 0; i < number_players; i++)
			score[i] = param_score_draw;
		return true;
	
	//game does not end yet
	}else

		return false;

}

/**
DEFAULT: procedure for two-valued human move input
*/
int Game_Engine::Human_Move_Input()
{
	int move_num, error, inVal1, inVal2;

	//repeat until selected move is valid
	move_num = -1;
	while(move_num == -1){

		//print to standard output and get input
		fflush(stdout);
		cout << "Waiting for player" << (current_player+1) << " input (Y X): ";

		//check for input error
		error = scanf("%d %d",&inVal1, &inVal2);
		if(error == 0){
			cin.clear();
			cin.ignore();
			cout << "invalid input!" << endl;

		//check if input value valid
		}else{
			move_num = Human_Move_Translate( inVal1*board_length + inVal2);
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
DEFAULT: Transform human input value in game move appropriate input; by default same value is returned
*/
int Game_Engine::Human_Move_Translate(int m)
{
	return m;
}

/**
DEFAULT: Output current board (game) state to standard output.
*/
void Game_Engine::Output_Board_State()
{	
	//Output_Board_State_Raw();
	//return;

	gmp->Print("\n   ");
	for(int i = 0; i < board_length; i++)
		gmp->Print(" %d",i+1);
	gmp->Print("\n");
	for(int i = 0, k = 0; i < board_height; i++, k += board_length){
		gmp->Print("\n%2d ",i+1);
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
void Game_Engine::Output_Board_State_Raw()
{	
	gmp->Print("\n   ");
	for(int i = 0; i < board_length; i++)
		gmp->Print(" %d",i+1);
	gmp->Print("\n");
	for(int i = 0, k = 0; i < board_height; i++, k += board_length){
		gmp->Print("\n%2d ",i+1);
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

/**
DEBUG procedure: a random sequence of moves to check correct implementation
*/
void Game_Engine::Debug_Test_Sequence()
{

	Game_Reset();
	Settings_Reset();
	Game_Reset();
	for(int i = 0; i < maximum_plys+1; i++){
		Play_Move(Select_Move_Random());
		fflush(stdout);
		Output_Board_State();
	}

	Settings_Reset();
	Game_Reset();
	Settings_Reset();
	for(int i = 0; i < maximum_plys+1; i++){
		Play_Move(Select_Move_Random());
		fflush(stdout);
		Output_Board_State();
	}

	Settings_Reset();
	for(int i = 0; i < maximum_plys+1; i++){
		if(game_ended == 0){
			Play_Move_Unsafe(Select_Move_Random());
			fflush(stdout);
			Output_Board_State();
		}else
			i = maximum_plys;
	}

	fflush(stdout);
	gmp->Print("Moves history:");
	for(int i = 0; i < maximum_plys+1; i++)
		gmp->Print(" %d", history_moves[i]);
	gmp->Print("\n");

	fflush(stdout);
	Settings_Reset();

	for(int i = 0; i < 10; i++)
		Game_Reset();

	for(int i = 0; i < 10; i++)
		Settings_Reset();

	fflush(stdout);
}

/**
DEBUG procedure: execute a given number (default = 1) of random moves and outputs of board state
*/
void Game_Engine::Debug_Random_Move_Output(int number_moves)
{
	for(int i = 0; i < number_moves; i++){
		Play_Move(Select_Move_Random());
		Output_Board_State();
	}
}

/**

*/
void Game_Engine::Output_Moves_History()
{
	gmp->Print("Number plys: %3d,  sequence:",current_plys);
	for(int i = 0; i < current_plys+1; i++)
		gmp->Print(" %d", history_moves[i]);
	gmp->Print("\n");
}

/**
Play a 1v1 human game by standard input/output.
For each move the player inputs the horizontal and vertical coordinate, e.g.: 4 6
PASS action is played with: 0 0

@param board_width Size of game board
@param showCurrentScore Display score after each play
*/
void Game_Engine::Simulate_Human_Game()
{

	//instantiate players
	Player_Engine* players[] = {new Player_Human(this), new Player_Human(this)};

	//restart and play a game
	Game_Reset();
	Simulate_Output_Game(players);

	//clean up
	delete players[0];
	delete players[1];

}

/**
Show and play a single game on standard output.

@param players Array of two pointers to Player_Engine objects
*/
void Game_Engine::Simulate_Output_Game(Player_Engine** players)
{

	int feedback, bestPlayer, multipleWinners;
	double bestOutcome;

	//check if players are correctly linked to game, otherwise exit procedure
	players = Validate_Players(players);
	if(players == NULL)
		return;
	
	//starting board state
	Output_Board_State();

	//play game until end is signaled
	feedback = (int)game_ended;
	while(feedback == 0){

		//DEBUG
		//if(current_plys == 23)
		//	gmp->Print("");

		//play next move
#if(TOM_DEBUG)
		feedback = Play_Move( players[current_player]->Get_Move() );
#else
		feedback = Play_Move_Unsafe( players[current_player]->Get_Move() );
#endif
		//output game state
		Output_Board_State();

	}

	//calculate score and output winners
#if(!TOM_OUTPUT_TO_MATLAB)
	Calculate_Score();
	if(number_players == 1){	//single player game
		gmp->Print("\nGAME ENDED: player scored %f\n", score[0]);
	}else{						//two or more players
		bestOutcome = -DBL_MAX;
		bestPlayer = 0;
		multipleWinners = 1;
		for(int i = 0; i < number_players; i++){
			if(score[i] > bestOutcome){
				bestOutcome = score[i];
				bestPlayer = i;
				multipleWinners = 1;
			}else if(score[i] == bestOutcome){
				multipleWinners++;
			}
		}
		if(multipleWinners > 1){
			if(multipleWinners == number_players){
				gmp->Print("GAME ENDED: draw, players score %f\n", bestOutcome);
			}else{
				gmp->Print("GAME ENDED: shared win by %d players (", multipleWinners);
				for(int i = 0; i < number_players; i++)
					if(score[i] == bestOutcome)
						gmp->Print(" %d",i+1);
				gmp->Print(" ) with score %f\n", bestOutcome);
			}
		}else{
			gmp->Print("\nGAME ENDED: player %d wins with score %f\n", bestPlayer+1, bestOutcome);
		}
	}
#endif

	//ignore last key input when waiting for "press any key"
#if(!TOM_OUTPUT_TO_MATLAB)
	cin.ignore();
#endif

	////wait for user input after game end
	//fflush(stdout);
	//cout << endl << endl << "Press any key to exit.";
	//cin.get();

}

/**
Procedure executes given number of games with specified players, for player learning purpose.

@param output_depth sets how detailed is the output/visualization
*/
void Game_Engine::Learn_Players(int num_games, int output_depth, Player_Engine** players)
{

	int nextMove, feedback;
	int output_interval, next_output;
	double cpu_time;

	//check if players are correctly linked to game, otherwise exit procedure
	players = Validate_Players(players);
	if(players == NULL)
		return;

	//output depth 0
#if(!TOM_OUTPUT_TO_MATLAB)
	if(output_depth >= TOMGAME_OUTPUT_DEPTH0){		
		gmp->Print("\nLearning game %s [%d games]", game_name.c_str(), num_games);
		for(int i = 0; i < this->number_players; i++)
			gmp->Print(", Player %d: %s", i+1, players[i]->player_name.c_str());
		gmp->Print(" |");
		if(output_depth == TOMGAME_OUTPUT_DEPTH0){
			output_interval = (int)(num_games/10);
			next_output = output_interval;
		}
	}
#endif

	//measure time
	cpu_time = getCPUTime();

	//execute games
	for(int g = 0; g < num_games; g++){

		//reset game state (start new game)
		Game_New();

		//call new-game procedure for players 
		for(int i = 0; i < number_players; i++)
			players[i]->New_Game();

		//play game until end is signaled
		feedback = (int)game_ended;
		while(feedback == 0){

			//current player selects next move
			nextMove = players[current_player]->Get_Move();

			//call before-move procedure for all players
			for(int i = 0; i < number_players; i++){
				players[i]->Before_Move(nextMove);
			}

			//simulate game dynamics with selected move
#if(TOM_DEBUG)
			feedback = Play_Move(nextMove);
#else
			feedback = Play_Move_Unsafe(nextMove);
#endif
			//call after-move procedure for all players
			for(int i = 0; i < number_players; i++){
				players[i]->After_Move(nextMove);
			}

		}

		//calculate score
		Calculate_Score();

		//call end-game procedure for players, game must not be resetted before this call
		for(int i = 0; i < number_players; i++)
			players[i]->End_Game();

		//output depth 1
#if(!TOM_OUTPUT_TO_MATLAB)
		if(output_depth == TOMGAME_OUTPUT_DEPTH0){
			if(g >= next_output){
				gmp->Print(".");
				next_output += output_interval;
			}
		}else if(output_depth >= TOMGAME_OUTPUT_DEPTH1){
			gmp->Print("\nL1 Game num %3d \t plys %2d \t score",g+1,current_plys);
			for(int i = 0; i < number_players; i++)
				gmp->Print("  %3.1f", score[i]);
		}
#endif

	}

	//output depth 0, with players final output
#if(!TOM_OUTPUT_TO_MATLAB)
	if(output_depth >= TOMGAME_OUTPUT_DEPTH0){
		cpu_time = getCPUTime()-cpu_time;
		if(output_depth > TOMGAME_OUTPUT_DEPTH0)
			gmp->Print("\n");
		else
			gmp->Print("\t ");
		gmp->Print("Runtime: %9.3f s",cpu_time);
		for(int i = 0; i < number_players; i++)
			if(players[i]->final_output_enabled)
				players[i]->Output();
	}
#endif
}
void Game_Engine::Learn_Two_Players(int num_games, int output_depth, Player_Engine* player1, Player_Engine* player2)
{
	Player_Engine *tmpPlayers[] = {player1,player2};
	Learn_Players(num_games, output_depth, tmpPlayers);
}

double Game_Engine::Evaluate_Players(int num_repeats, int num_games, int output_depth, Player_Engine** players, bool rotate_starting_player, int return_score_player_num, Tom_Sample_Storage<double>** score_output, int intermediate_output, const int measure_time_per_move)
{

	int nextMove, feedback, bestPlayer, multipleWinners;
	double bestOutcome;
	int output_interval, next_output;
	double cpu_time;
	double cpu_time1;
	double scr_out_time;

	//check if players are correctly linked to game, otherwise exit procedure
	players = Validate_Players(players);
	if(players == NULL)
		return -1;

	//output depth 1
	if(output_depth >= TOMGAME_OUTPUT_DEPTH1){		
		gmp->Print("\nEval on game %s [%d repeats %d games]", game_name.c_str(), num_repeats, num_games);
		for(int i = 0; i < this->number_players; i++)
			gmp->Print(", Player %d: %s", i+1, players[i]->player_name.c_str());
		gmp->Print(" |");
		if(output_depth == TOMGAME_OUTPUT_DEPTH1){
			output_interval = (int)(num_games*num_repeats/10);
			next_output = output_interval;
		}
	}

	//allocate counters
	int* win_count_total = new int[number_players+1];	//total counter for: draws | player1 | player2 | ...
	int* win_count_local = new int[number_players+1];	//single repeat counter for: draws | player1 | player2 | ...
	double* score_count_total = new double[number_players];
	double* score_count_local = new double[number_players];
	double* players_move_time_sum = new double[number_players];
	int* players_move_count = new int[number_players];

	//initialize counter values
	win_count_total[0] = 0;
	for(int i = 0; i < number_players; i++){
		win_count_total[i+1] = 0;
		score_count_total[i] = 0.0;
		players_move_time_sum[i] = 0.0;
		players_move_count[i] = 0;
	}

	//measure time
	cpu_time = getCPUTime();
	scr_out_time = getCPUTime();

	//execute repeats
	for(batch_repeat_index = 0; batch_repeat_index < num_repeats; batch_repeat_index++){
		
		//reset counter values
		win_count_local[0] = 0;
		for(int i = 0; i < number_players; i++){
			win_count_local[i+1] = 0;
			score_count_local[i] = 0.0;
		}

		//reset players
		for(int i = 0; i < number_players; i++)
			if(players[i]->external_reset_enabled)
				players[i]->Reset();

		//execute games
		for(game_repeat_index = 0; game_repeat_index < num_games; game_repeat_index++){

			//reset game state (start new game)
			Game_New();

			//call new-game procedure for players 
			for(int i = 0; i < number_players; i++)
				players[i]->New_Game();

			//play game until end is signaled
			feedback = (int)game_ended;
			while(feedback == 0){

				//measure move time
				cpu_time1 = getCPUTime();

				//current player selects next move
				nextMove = players[current_player]->Get_Move();
				players_move_count[current_player]++;

				//call before-move procedure for all players
				for(int i = 0; i < number_players; i++){
					players[i]->Before_Move(nextMove);
				}

				//remember cpu time (cumulative sum)
				players_move_time_sum[current_player] += ( getCPUTime()-cpu_time1 );

				//simulate game dynamics with selected move
#if(TOM_DEBUG)
				feedback = Play_Move(nextMove);
#else
				feedback = Play_Move_Unsafe(nextMove);
#endif
				//call after-move procedure for all players
				cpu_time1 = getCPUTime();
				for(int i = 0; i < number_players; i++){
					players[i]->After_Move(nextMove);
				}
				players_move_time_sum[current_player] += ( getCPUTime()-cpu_time1 );
			}

			//calculate score
			Calculate_Score();

			//call end-game procedure for players, game must not be resetted before this call
			for(int i = 0; i < number_players; i++)
				players[i]->End_Game();

			// --- update evaluation statistics ---

			//store score to external data structure
			if(score_output != NULL){			
				score_output[0]->Add_Sample(score[0]);
				score_output[1]->Add_Sample(score[1]);
			}

			//find winning player
			if(number_players == 1){	//single player game
				score_count_total[0] += score[0];
				score_count_local[0] += score[0];
			}else{
				bestOutcome = -DBL_MAX;
				bestPlayer = 0;
				multipleWinners = 1;
				for(int i = 0; i < number_players; i++){
					if(score[i] > bestOutcome){
						bestOutcome = score[i];
						bestPlayer = i;
						multipleWinners = 1;
					}else if(score[i] == bestOutcome){
						multipleWinners++;
					}

					//save score statistics
					score_count_total[i] += score[i];
					score_count_local[i] += score[i];
				}

				//single winner
				if(multipleWinners == 1){
					win_count_total[bestPlayer+1]++;
					win_count_local[bestPlayer+1]++;

				//multiple players with equal highest score
				}else{
					//game ended in draw (all players have equal score)
					if(multipleWinners == number_players){
						win_count_total[0]++;
						win_count_local[0]++;

					//multiple winners
					}else{
						for(int i = 0; i < number_players; i++){
							if(score[i] == bestOutcome){
								win_count_total[i+1]++;
								win_count_local[i+1]++;
							}
						}
					}
				}
			}

			//output depth 3: after each game
			if(output_depth >= TOMGAME_OUTPUT_DEPTH3){
				if(number_players == 1){
					gmp->Print("\nL3 game   | %d\t score %6.5f", game_repeat_index+1, score[0]);
				}else{
					gmp->Print("\nL3 game   | %d\t draws %4d\t wins", game_repeat_index+1, win_count_local[0]);
					for(int i = 0; i < number_players; i++)
						gmp->Print(" %4d", win_count_local[i+1]);
					gmp->Print("\t     scores");
					for(int i = 0; i < number_players; i++)
						gmp->Print(" %6.3f", score[i]);
				}
			}else if(output_depth == TOMGAME_OUTPUT_DEPTH1){
				if(batch_repeat_index*num_games+game_repeat_index >= next_output){
					gmp->Print(".");
					next_output += output_interval;
				}
			}

			if(score_output != NULL){	
				if(intermediate_output > 0){
					if(((score_output[return_score_player_num]->n) % intermediate_output) == 0){
						score_output[return_score_player_num]->Calc_AvgDev();
						score_output[return_score_player_num]->Calc_Confidence();
						gmp->Print("%8d  %6.2f  %6.2f  %6.2f  %9.3f\n",
							score_output[return_score_player_num]->n,
							score_output[return_score_player_num]->avg*100,
							score_output[return_score_player_num]->Calc_Confidence()*100,
							score_output[return_score_player_num]->dev*100,
							getCPUTime()-cpu_time
						);
						if((scr_out_time < 0) || (scr_out_time > TOMGAME_OUTPUT_EVALUATE_INTERMEDIATE_FLUSH_TIMEINTERVAL)){
							gmp->Flush();
						}
						scr_out_time = getCPUTime();
					}
				}
			}
		}
		// END - games

		//output depth 2: after series of games
		if(output_depth >= TOMGAME_OUTPUT_DEPTH2){
			if(number_players == 1){
				gmp->Print("\nL2 REPEAT | %d\t average score %6.5f", batch_repeat_index+1, score_count_local[0] / num_games);
			}else{
				gmp->Print("\nL2 REPEAT | %d\t draws %4d\t wins", batch_repeat_index+1, win_count_local[0]);
				for(int i = 0; i < number_players; i++)
					gmp->Print(" %4d", win_count_local[i+1]);
				gmp->Print("\t sum-scores");
				for(int i = 0; i < number_players; i++)
					gmp->Print(" %6.3f", score_count_local[i]);
				gmp->Print("\t relative");
				for(int i = 0; i < number_players+1; i++)
					gmp->Print(" %5.3f", (float)win_count_local[i]/num_games);
			}
		}
		
		//calculate avgerage and deviation in external score storing structure
		if(score_output != NULL){	
			score_output[0]->Calc_AvgDev();
			score_output[0]->Calc_Confidence();
			score_output[1]->Calc_AvgDev();
			score_output[1]->Calc_Confidence();
		}
	}
	// END - repeats

	//output depth 0: after all repeats, final results, players outputs
	if(output_depth >= TOMGAME_OUTPUT_DEPTH0){
		cpu_time = getCPUTime()-cpu_time;
		if(number_players == 1){
			gmp->Print("\naverage score %6.5f\t | Player: %s", score_count_total[0] / num_games / num_repeats, players[0]->player_name.c_str());
		}else{
			gmp->Print("\nPlayers: %s",players[0]->player_name.c_str());
			for(int i = 1; i < this->number_players; i++)
				gmp->Print(" vs %s", players[i]->player_name.c_str());
			gmp->Print(":\t draws %4d\t wins", win_count_total[0]);
			for(int i = 0; i < number_players; i++)
				gmp->Print(" %4d", win_count_total[i+1]);
			gmp->Print("\t sum-scores");
			for(int i = 0; i < number_players; i++)
				gmp->Print(" %6.3f", score_count_total[i]);
			gmp->Print("\t relative");
			for(int i = 0; i < number_players+1; i++)
				gmp->Print(" %5.3f", (float)win_count_total[i]/num_games/num_repeats);
			
		}
		gmp->Print("\t[%d repeats %d games]\t Runtime: %9.3f s", num_repeats, num_games, cpu_time);

		//output players info
		for(int i = 0; i < number_players; i++)
			if(players[i]->final_output_enabled)
				players[i]->Output();
	}

	//output average time per move
	if(measure_time_per_move){
		for(int i = 0; i < number_players; i++)
			gmp->Print("\nTIME P%d:   %6.1lf ms/game   %6.2lf ms/move  (games %d moves %d totalTime %lf s)",i,players_move_time_sum[i] / (double)(num_repeats*num_games) * 1000.0, players_move_time_sum[i] / (double)(num_repeats*num_games*players_move_count[i]) * 1000.0 , num_repeats*num_games, players_move_count[i],players_move_time_sum[i]);
		gmp->Print("\n\n");
	}

	//save return value
	double tmpReturn = score_count_total[return_score_player_num];

	//clean up
	delete[] win_count_total;
	delete[] win_count_local;
	delete[] score_count_total;
	delete[] score_count_local;
	delete[] players_move_time_sum;
	delete[] players_move_count;

	//return score
	return tmpReturn;
}
void Game_Engine::Evaluate_Two_Players(int num_repeats, int num_games, int output_depth, Player_Engine* player1, Player_Engine* player2)
{
	Player_Engine *tmpPlayers[] = {player1,player2};
	Evaluate_Players(num_repeats, num_games, output_depth, tmpPlayers);
}

/**
Checks if the players are correctly defined and linked to game
*/
Player_Engine** Game_Engine::Validate_Players(Player_Engine** specified_players)
{

	//players were not specified when calling Game_Engine procedure
	if(specified_players == NULL){
		//if players were binded to 
		if(players != NULL){
			specified_players = players;
		
		//players were not binded to game - invalid call
		}else{

			//print error and return
			gmp->Print("!!ERROR!! Game Engine: Validate_Players(): function called without specifying or binding players to game\n");
			return NULL;
		}

	//players argument was specified when calling Game_Engine procedure
	}else{

		//check if binded players are different from players in calling argument
		if(show_warnings)
			if((players != NULL)&&(players != specified_players))
				gmp->Print("!WARNING! Game Engine: Validate_Players(): binded players are different from specified players in argument\n");

	}

	//check if correct number of players
	try{
		for(int i = 0; i < number_players; i++){
			if(specified_players[i] != NULL){

				//check if players are binded to correct game
				if(show_warnings)
					if(specified_players[i]->game != this)
						gmp->Print("!WARNING! Validate_Players(): player with index %d binded to incorrect game\n",i);
				
			//player not defined: error
			}else{
				//print error and return
				gmp->Print("!!ERROR!! Game Engine: Validate_Players(): player with index %d undefined (NULL)\n",i);
				return NULL;
			}
		}
		//check if players are binded to correct game

	//catch memory access violation exception
	}catch(char *exc){
		gmp->Print("!!ERROR!! Game Engine: Validate_Players(): insufficent number of player objects or incorrect number of players definiton: %d, exception caught: %s\n", number_players, exc);
		return NULL;
	}

	return specified_players;

}

//
///**
//Benchmark function for game playing: measures execution time.
//
//@param num_games Number of games to simulate.
//@param moves_percent Number of plys per game - percentage of total board size, float value > 0.0
//@param printTime Enable/disable display of CPU execution time
//@return Duration of simulations in miliseconds.
//*/
//double Game_Engine::benchmark_playingTime(int num_games, float moves_percent, bool printTime){
//	double cpu_time = getCPUTime();
//
//	//run simulations
//	for(int i = 0; i < num_games; i++){
//		resetGame();
//		for(int j = 0; j < (int)(board_size*moves_percent); j++)
//			if(playMoveUnsafe(randomAction()) != 0)
//				j = (int)(board_size*moves_percent);
//	}
//
//	//return elapsed time
//	cpu_time = getCPUTime()-cpu_time;
//	if(printTime)
//		gmp->Print("PLAYED %d games, up to %d moves, %dx%d board, runtime: %9.3f s %9.1f us/game\n",
//			num_games,
//			(int)(board_size*moves_percent),
//			board_length,
//			board_length,
//			cpu_time,
//			cpu_time / num_games * 1000*1000
//		);
//	fflush(stdout);
//	return cpu_time;
//}
//
///**
//Debug benchmark function for game playing, measures outcome statistics: number of finished games, average score WITHOUT komi, percentage of each player wins and draws
//
//@param num_games Number of games to simulate.
//@param moves_percent Number of plys per game - percentage of total board size, float value > 0.0
//@return Duration of simulations in miliseconds.
//*/
//double Game_Engine::benchmark_playingStat(int num_games, float moves_percent){
//	int countFinished, countWin1, countWin2;
//	INT64 countMoves;
//	float tmpScore, avgScore;
//	double cpu_time, move_time;
//
//	move_time = 0;
//	cpu_time = getCPUTime();
//
//	countFinished = 0;
//	countWin1 = 0;
//	countWin2 = 0;
//	avgScore = 0.0f;
//	countMoves = 0;
//
//	//run simulations
//	for(int i = 0; i < num_games; i++){
//		resetGame();
//		move_time -= getCPUTime();
//		for(int j = 0; j < (int)(board_size*moves_percent); j++){
//			countMoves++;
//			if(playMoveUnsafe(randomAction()) != 0){
//				j = (int)(board_size*moves_percent);
//				//gmp->Print("GAME FINISHED: %d\n", number_of_plays); output_state_to_STD(false);	//DEBUG output
//				countFinished++;
//			}
//		}
//		move_time += getCPUTime();
//		tmpScore = computeScore(0.0);
//		avgScore += tmpScore;
//		if(score > 0)
//			countWin1++;
//		else if(score < 0)
//			countWin2++;
//	}
//
//	//average and output results
//	avgScore /= num_games;
//	
//	//output results and return elapsed time
//	cpu_time = getCPUTime()-cpu_time;
//	gmp->Print("\nRESULT %d games, up to %d moves, %dx%d board, runtime: %9.3f s %9.1f us/game %9.0f ns/move\n .finished games:\t%2.1f%%  [%d games]\n .avg moves per game:\t%.1f\n .average score:\t%3.3f\n .outcomes w1/d/w2:\t%2.2f%%  %2.2f%%  %2.2f%%\n\n",
//		num_games,
//		(int)(board_size*moves_percent),
//		board_length,
//		board_length,
//		cpu_time,
//		cpu_time / num_games * 1000*1000,
//		move_time / countMoves * 1000*1000*1000,
//		(float)countFinished/num_games*100.0f,
//		countFinished,
//		countMoves/(double)num_games,
//		avgScore, (float)countWin1/num_games*100.0f,
//		(float)(num_games-countWin1-countWin2)/num_games*100.0f,
//		(float)countWin2/num_games*100.0f
//	);
//	fflush(stdout);
//
//	return cpu_time;
//}
//
///**
//Benchmark function for game scoring: measures execution time.
//
//@param num_games Number of games to simulate.
//@param moves_percent Number of plys per game - percentage of total board size, float value > 0.0
//@param num_scores Number of score calculation repeats per game.
//@param printTime Enable/disable display of CPU execution time
//@return Duration of simulations in miliseconds.
//*/
//double Game_Engine::benchmark_scoring(int num_games, float moves_percent, int num_scores, bool printTime){
//	
//	double cpu_time, game_sim_time, score_time, move_time;
//	double tmpScore = 0;
//
//	game_sim_time = 0;
//	move_time = 0;
//	score_time = 0;
//
//	cpu_time = getCPUTime();
//
//	//run simulations
//	for(int i = 0; i < num_games; i++){
//		game_sim_time -= getCPUTime();
//		resetGame();
//		move_time -= getCPUTime();
//		for(int j = 0; j < (int)(board_size*moves_percent); j++){
//			playMoveUnsafe(randomAction());
//			//output_state_to_STD(false);	//DEBUG output
//		}
//		move_time += getCPUTime();
//		game_sim_time += getCPUTime();
//
//		score_time -= getCPUTime();
//		for(int j = 0; j < num_scores; j++){
//			//gmp->Print("%.1f %d\n",computeScore(), number_of_plays);	//DEBUG output
//			tmpScore += computeScore();
//			number_of_plays++;
//		}
//		score_time += getCPUTime();
//	}
//
//	//return elapsed time
//	cpu_time = getCPUTime()-cpu_time;
//	//gmp->Print("test: %3.3f %3.3f %3.3f %3.3f\n", cpu_time, (score_time+game_sim_time),game_sim_time,score_time);	//DEBUG output
//	if(printTime)
//		gmp->Print("SCORED %d games, %d moves, %d scores, %dx%d board, runtime: %9.3f s %9.1f us/game %9.0f ns/move %9.0f ns/score\n",
//			num_games,
//			(int)(board_size*moves_percent),
//			num_scores,
//			board_length,
//			board_length,
//			cpu_time,
//			game_sim_time / num_games * 1000*1000,
//			move_time / (num_games*(int)(board_size*moves_percent)) * 1000*1000*1000,
//			score_time / (num_games*num_scores) * 1000*1000*1000
//		);
//	fflush(stdout);
//
//	score = (float)tmpScore/num_games/num_scores;
//
//	return cpu_time;
//}
//
//
///**
//A series of benchmarks to measure execution time.
//*/
//void Game_Engine::benchmarkSeries(){
//	
//	srand((unsigned int)time(NULL));
//
//	Game_Engine game02(2);
//	Game_Engine game05(5);
//	Game_Engine game10(10);
//	Game_Engine game19(19);
//
//	game02.benchmark_playingTime(999);
//	game02.benchmark_playingTime(200000);
//	game02.benchmark_playingTime(200000);
//	game02.benchmark_playingTime(200000);
//	game05.benchmark_playingTime(999);
//	game05.benchmark_playingTime(200000);
//	game05.benchmark_playingTime(200000);
//	game05.benchmark_playingTime(200000);
//	game10.benchmark_playingTime(999);
//	game10.benchmark_playingTime(200000);
//	game10.benchmark_playingTime(200000);
//	game10.benchmark_playingTime(200000);
//	game19.benchmark_playingTime(999);
//	game19.benchmark_playingTime(200000);
//	game19.benchmark_playingTime(200000);
//	game19.benchmark_playingTime(200000);
//
//	game02.benchmark_scoring(999);
//	game02.benchmark_scoring(50000);
//	game02.benchmark_scoring(50000);
//	game02.benchmark_scoring(50000);
//	game05.benchmark_scoring(999);
//	game05.benchmark_scoring(50000);
//	game05.benchmark_scoring(50000);
//	game05.benchmark_scoring(50000);
//	game10.benchmark_scoring(999);
//	game10.benchmark_scoring(50000);
//	game10.benchmark_scoring(50000);
//	game10.benchmark_scoring(50000);
//	game19.benchmark_scoring(999);
//	game19.benchmark_scoring(50000);
//	game19.benchmark_scoring(50000);
//	game19.benchmark_scoring(50000);
//}
//
