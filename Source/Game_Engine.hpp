#ifndef _TOM_GAME_ENGINE_
#define _TOM_GAME_ENGINE_

//includes
#include "Game_Interface.hpp"
#include "Support_MultiPrint.hpp"

//defines
#define TOMGAME_ENGINE_EVALUATE_NUM_REPEATS		100
#define TOMGAME_ENGINE_EVALUATE_NUM_GAMES		1000
#define TOMGAME_ENGINE_EVALUATE_OUTPUT_DEPTH	1
#define TOMGAME_ENGINE_LEARNING_OUTPUT_DEPTH	0
#define TOMGAME_ENGINE_EVALUATE_ROTATE_PLAYERS	false

#define TOMGAME_ENGINE_COPY_GAME_HISTORY		1
#define TOMGAME_ENGINE_COPY_GAME_STATE			1
#define TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY		NULL

#define TOMGAME_OUTPUT_EVALUATE_INTERMEDIATE_FLUSH_TIMEINTERVAL		5.0			//the time interval in seconds between flushing output to file

//defines - DEBUG
#define TOMGAME_ENGINE_DISABLE_RANDOM			(TOM_DISABLE_RANDOM)

//external global variables
extern MultiPrinter * gmp;	//multiprinter object defined in main.hpp
extern int experiment_settings_id;

//forward declarations
class Player_Engine;

//class definition
#pragma once
class Game_Engine
{

//public procedures and variables
public:

	//virtual public procedures - engine
	virtual void Game_Reset()			= 0;
	virtual void Game_New()				{ Game_Reset(); };
	virtual bool Validate_Move(int);
	virtual int Play_Move(int);
	virtual int Play_Move_Unsafe(int);
	virtual void Calculate_Score()		{};

	//virtual public procedures - support
	virtual int  Human_Move_Input();
	virtual void Copy_Game_State_From(Game_Engine* source, const bool copy_history = TOMGAME_ENGINE_COPY_GAME_HISTORY, int history_copy_start_index = 0) {};
	virtual Game_Engine* Create_Duplicate_Game(bool copy_state = TOMGAME_ENGINE_COPY_GAME_STATE, const bool copy_history = TOMGAME_ENGINE_COPY_GAME_HISTORY) { return NULL; };
	virtual int Get_Next_Player(int p)		{return Get_Next_Player_TwoPlayer(p);};					//return next player on move: default set as for two-player game
	virtual int Get_Previous_Player(int p)	{return Get_Next_Player(p);};	//return previous player: default set to work correctly for single-player and two-player games (same rule as next player)
	int Get_Next_Player_SinglePlayer(int p);
	int Get_Next_Player_TwoPlayer(int p);
	int Get_Next_Player_MultiPlayer(int p);
	int Get_Previous_Player_MultiPlayer(int p);

	//virtual public procedures - debug and visualization
	virtual void Output_Board_State();
	virtual void Output_Board_State_Raw();

	//public procedures
	void Settings_Apply_Changes();
	void Settings_Reset();
	void Make_Moves_List();
	int  Select_Move(int serial_number);
	int  Select_Move_Unsafe(int serial_number);
	int  Select_Move_Random();
	virtual int Select_Move_Passive();
	virtual int Get_Next_Player()			{return Get_Next_Player(current_player);};		//return next player on move
	virtual int Get_Previous_Player()		{return Get_Previous_Player(current_player);};	//return previous player

	//--- many (if not all) of the following procedures and variables should be put in Game_Interface

	int experiment_repeat_index;
	int	batch_repeat_index;
	int game_repeat_index;

	//public procedures - debug and visualization
	void Debug_Test_Sequence();
	void Debug_Random_Move_Output(int number_moves = 1);
	void Output_Moves_History();
	void Simulate_Human_Game();							//Play a 1v1 human game by standard input/output	
	void Simulate_Output_Game(Player_Engine** = NULL);	//Show and play a single game on standard output.
	
	//public procedures - banchmarking, learning and evaluation

	//play multiple games without tracking statistics, only to enable player learning
	void Learn_Players(
		int num_games = TOMGAME_ENGINE_EVALUATE_NUM_GAMES,
		int output_depth = TOMGAME_ENGINE_LEARNING_OUTPUT_DEPTH,
		Player_Engine** players = NULL
	);
	void Learn_Two_Players(
		int num_games = TOMGAME_ENGINE_EVALUATE_NUM_GAMES,
		int output_depth = TOMGAME_ENGINE_LEARNING_OUTPUT_DEPTH,
		Player_Engine* player1 = NULL,
		Player_Engine* player2 = NULL
	);

	//play multiple repeats of multiple games and track statistics about each player
	double Evaluate_Players(
		int	num_repeats = TOMGAME_ENGINE_EVALUATE_NUM_REPEATS,
		int num_games = TOMGAME_ENGINE_EVALUATE_NUM_GAMES,
		int output_depth = TOMGAME_ENGINE_EVALUATE_OUTPUT_DEPTH,
			//-2 intermediate output every 1000
			//-1 only final output
			//>=0 level of output detail
		Player_Engine** players = NULL,
		bool rotate_starting_player = TOMGAME_ENGINE_EVALUATE_ROTATE_PLAYERS,		//CURRENTLY IMPLEMENTED ONLY FOR 2 PLAYERS
		int return_score_player_num = 0,		//defines which players score will be returned
		Tom_Sample_Storage<double>** score_output = NULL,	//output data structure (must be preallocated externally)
		int intermediate_output = 0,		//output results every "intermediate_output" repeats (0 = disabled)
		const int measure_time_per_move = 0		//measure time per move and output at the end
	);
	void Evaluate_Two_Players(
		int	num_repeats = TOMGAME_ENGINE_EVALUATE_NUM_REPEATS,
		int num_games = TOMGAME_ENGINE_EVALUATE_NUM_GAMES,
		int output_depth = TOMGAME_ENGINE_EVALUATE_OUTPUT_DEPTH,
		Player_Engine* player1 = NULL,
		Player_Engine* player2 = NULL
	);

	//--- TO REDEFINE/CHECK ---
	//double	benchmark_playingTime(int num_games = 10000, float moves_percent = 2.0f, bool printTime = true);					//Benchmark function for game playing execution time 
	//double	benchmark_playingStat(int num_games = 10000, float moves_percent = 2.0f);											//Benchmark function for game playing outcome statistics
	//double	benchmark_scoring(int num_games = 10000, float moves_percent = 0.75f, int num_scores = 100, bool printTime = true);	//Benchmark function for game scoring execution time
	//static void	benchmarkSeries();																		//Run a set of execution time test
	//_TO CHECK_ END

	//public variables - game definition
	string game_name;
	bool is_deterministic;

	//public variables - external links
	Player_Engine** players;

	//public variables - game settings
	int board_length;
	int board_height;
	int board_size;
	int number_players;
	int maximum_allowed_moves;
	int maximum_plys;
	double param_score_win;
	double param_score_lose;
	double param_score_draw;

	//public variables - visualization
	char output_board_last_move_char;
	char* output_board_lookup_char;

	//public variables - debug settings
	bool show_warnings;

	//public variables - game state
	char*	board_state;
	int*	current_number_moves;
	bool**	current_moves;
	int**	current_moves_list;
	double* score;
	int current_player;
	bool game_ended;

	//public variables - game history
	int current_plys;
	int* history_moves;

protected:

	//virtual protected procedures
	virtual void Init_Settings()	= 0;
	virtual void Allocate_Memory()	{};
	virtual void Clear_Memory()		{};
	virtual void Copy_Settings(Game_Engine* source_game)	{};
	virtual int  Game_Dynamics(int) = 0;	//rules for changing the game state, to be implemented for each game class
	virtual bool Check_Game_End(int pos) {return Check_Game_End_TwoPlayer(pos);};	//default set to two-player game
	bool Check_Game_End_SinglePlayer(int);
	bool Check_Game_End_TwoPlayer(int);
	bool Check_Game_End_MultiPlayer(int);
	virtual bool Check_Game_Win(int) = 0;	//combinations for winning positions, to be implemented for each game class
	virtual int  Human_Move_Translate(int);

	//protected procedures
	void Initialize();
	void Initialize_Common();
	void Copy_Initialize(Game_Engine* source_game);
	Player_Engine** Validate_Players(Player_Engine** specified_players);

	//protected variables - debug and visualizaton

};


#include "Player_Engine.hpp"

#endif