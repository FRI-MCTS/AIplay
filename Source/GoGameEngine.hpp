#ifndef _TOM_GAME_GO_
#define _TOM_GAME_GO_

//includes - C libraries
//#include <fstream>
//#include <cmath>
//#include <ctime>
//#include <cstring>
//#include <iostream>
//#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <ctime>
#include "Support_GetCPUTime.hpp"

using namespace std;

//default values
#define DEFAULT_GOENG_KOMI				6.5		//bonus to player 2
#define DEFAULT_GOENG_boardSize			5		//length of board by one axis
#define DEFAULT_GOENG_score_pass_point	1		//enable or disable giving a point to the opponent when passing

/**
	The Data structure of chains and stones on intersections
	is a one way linked list from the root (single_Chain) to
	the lastly added stone,	where each stone is linked to the
	root and the root holds the pointer to the last stone (in
	order to simplify merging of chains)
*/
typedef struct single_Intersection single_Intersection;
typedef struct single_Chain single_Chain;

//single board position - intersection
struct single_Intersection {
	int column;				//number of column, to avoid division/modulo in search procedures
	char stone;				//0 - clear, 1 - player1, 2 - player2
	int flag;				//support variable for search algorithms
	single_Chain * belonging_chain;		//pointer to which chain the stone belongs to (index in array), initialized to NULL
	single_Intersection * nextStone;	//pointer to next stone in same chain, initialized to NULL
};

//single chain of stones
struct single_Chain {
	int liberties;			//sum of liberties of all stones in the chain
	int size;				//number of stones in chain
	char flag;				//support variable for search algorithms - adding stone, merging/deleting chains
	char flag2;				//support variable for search algorithms - deleting single stones
	single_Intersection * firstStone;		//pointer to first stone in chain
	single_Intersection * lastStone;		//pointer to last stone in chain
};

//prototype defines
class PlayerGo;
class GoGameEngine;

//definiton of the Go game engine
#pragma once
class GoGameEngine
{

//public procedures and variables
public:
	
	GoGameEngine(int board_length = DEFAULT_GOENG_boardSize, bool reset = true);		//Constructor with parameter for board length
	virtual ~GoGameEngine(void);				//Destructor
	void	resetGame();						//Reset to initial values (restart game)
	int		playMove(int move_number);			//Check validity and play selected move (value 0 equals to action PASS, value 1 equals to intersection 1-1,...)
	int		playMoveUnsafe(int move_number);	//Play selected move without checking validity (value 0 equals to action PASS, value 1 equals to intersection 1-1,...)
	bool	validateAction(int number);			//Check if action is valid
	int		selectAction(int serial_number);		//Support function for selecting an action from the set of available actions, safe with validation
	int		selectActionUnsafe(int serial_number);	//Support function for selecting an action from the set of available actions, unsafe without validation
	int		randomAction();						//Support function for selecting a random action	
	float	computeScore();	//Calculate the score at current game state and add default number of bonus points to player 2 (komi)
	float	computeScore(float bonus_player2);	//Calculate the score at current game state and add number of bonus points to player 2 (komi)
	void	output_state_to_STD(bool showScore = false);			//Output current game state to standard output
	double	benchmark_playingTime(int num_games = 10000, float moves_percent = 2.0f, bool printTime = true);					//Benchmark function for game playing execution time 
	double	benchmark_playingStat(int num_games = 10000, float moves_percent = 2.0f);											//Benchmark function for game playing outcome statistics
	double	benchmark_scoring(int num_games = 10000, float moves_percent = 0.75f, int num_scores = 100, bool printTime = true);	//Benchmark function for game scoring execution time

	static GoGameEngine* createDuplicateGame(GoGameEngine* sourceGame);
	static void copyGameState(GoGameEngine* sourceGame, GoGameEngine* targetGame);

	//play multiple repeats of multiple games and track statistics about each player
	void evaluatePlayersPerformance(
		PlayerGo** players,
		int num_repeats = 1000,
		int num_games = 100000,
		float new_komi = 1.5,	//bonus points to P2
		bool print_wait = 0,
		float max_moves_factor = 2.0
	);
	void playOutputGame(PlayerGo** players, bool showCurrentScore = true);	//Show and play a single game on standard output.

	static void	humanGame(int board_width = DEFAULT_GOENG_boardSize, bool showCurrentScore = true, float komi = DEFAULT_GOENG_KOMI);		//Play a 1v1 human game by standard input/output	
	static void	benchmarkSeries();																		//Run a set of execution time test
	static void	debugTestSequence(int board_width = DEFAULT_GOENG_boardSize);							//Check GoEngine corectness

	single_Intersection * boardState;	//current board (game) state
	bool* moves[2];			//currently available moves for each player (respectively by index), including PASS move as first: moves[][0]
	int	num_moves[2];		//number of currently available moves for each player (respectively by index), including PASS move

	int board_length;		//length of playing board
	int board_size;			//square of board length
	int all_actions_num;	//number of all possible actions, including PASS (board_size + 1)
	int current_player;		//current player on move (0 - first player, 1 - second player)
	int number_of_plays;	//number of played moves up to current state (used also as tag for search algorithms)
	float score;			//current game score (positive in favour of player one), is calculated only after each call of computeScore()
	bool ended;				//hold flag whether the has game ended
	int previous_move;		//remembers last move (the move of the previous player)

//parameters
	float komi;				//bonus points to player 2 when computing score
	bool score_pass_point;	//give a point to the opponent when passing (from AGA scoring system)
	bool disable_warnings;	//enable/disable error feedback

//debug procedures
	void output_chains_to_STD();	//show current chains on game board

//private procedures and variables
private:

	void initialize(int board_width, bool reset = true);			//initialize game engine
	void captureChain(single_Chain* chainPointer, int capturingPlayer);	//capture all stones in chain
	void mergeChains(int newStone, int num_chains);							//merge multiple chains and last added stone, or crate a new chain
	void liberties_markStone(single_Intersection* stonePointer);			//mark liberties around a single stone
	void liberties_markChain(single_Chain* chainPointer);					//mark liberties around a chain
	void liberties_addOneStone(int addedStone);								//count liberties when adding single stone to chain
	int	computeScoreFlood(int iIndex);										//count empty intersections by recursive flooding
	void computeScoreFlood_dir(int iIndex, int* countVal);	// support function for computeScoreFlood

	//support functions for checking adjacent cells
	void adjacentCheck(int adjacentPosition, int* counter_friendly, int* counter_enemy);
	void adjacentCheckDel(int adjacentPosition, int* counter_chains, int capturingPlayer);
	void adjacentLiberty(int adjacentPosition);

	single_Chain* chains;	//list of current chains

	int num_captured[2];	//number of captured stones BY each player (respectively by index)
	int score_pass;			//counts points given by passing

	int last_score_compute;	//remember previous score compute time step

	int num_liberties;		//temporary global variable for number of liberties of last added/merged chain
	single_Chain* adjacentChain[3][4];	//temporary global array for storing pointers to adjacent chains of a single stone
	bool adjacentOne;		//temporary global variable for calculating score points: current empty zone adjacent to player one
	bool adjacentTwo;		//temporary global variable for calculating score points: current empty zone adjacent to player two
};

/**
Abstract class which can be used to implement a GO playing algorithm
*/
class PlayerGo{

public:
	PlayerGo(GoGameEngine* currentGame);
	//~PlayerGo();
	virtual int reset() {return 0;};
	virtual int newGame() {return 0;};
	virtual int getMove() = 0;
	virtual int endGame() {return 0;};
	virtual int output() {return 0;};
	
	bool isHuman;
	bool resetAfterSeries;
	int player_number;
	GoGameEngine* game;

protected:
	//virtual int init() = 0;
	
};

/**
Class that implements a human GO player
*/
class PlayerGoHuman : public PlayerGo{
public:
	PlayerGoHuman(GoGameEngine* currentGame);	//Constructor for human Go player module calling general Go player constructor
	int getMove();
};

//TODO: check if modulo % is slowing down:
// replace by adding another variable to single_intersection with type of location (NW,N,NE,...) with a byte number: this option is more complicated as a lot of IF clauses are needed but O(n) = n^2 +11n + 10
// this would also ease the count of liberties - or just add the row number (this way 4 IF clauses are needed per intersection, slower but simple, O(n) = n^2 * 4
//TODO: in addition to above, add to the array a -1 row and column on all sides (must recalculate all offsets appropriately) ... then you can simplify all checks from single intersection

//if we want to implement UNDO: at each UNDO all boardState flags must be reset to -1, same applies to last_score_compute, and score_pass/num_moves/num_captured must be checked (and maybe some other things too? current_player, number_of_plays)

#endif