#ifndef _TOM_GAME_TICTACTOE_
#define _TOM_GAME_TICTACTOE_

//includes
#include "Game_Gomoku.hpp"

//default values
#define TOMGAME_TICTACTOE_BOARD_LENGTH	3
#define TOMGAME_TICTACTOE_WIN_CONNECTED_PIECES 3
#define TOMGAME_TICTACTOE_SCORE_WIN		1.0
#define TOMGAME_TICTACTOE_SCORE_LOSE	0.0
#define TOMGAME_TICTACTOE_SCORE_DRAW	0.5

/**
Definiton of the Tic Tac Toe game engine.

Scoring: win 1, lose 0 (see defines)
*/
#pragma once
class Game_TicTacToe : public Game_Gomoku
{

public:
	
	//public procedures - engine
	Game_TicTacToe(Game_Engine* source_game = TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY);	//Constructor
	virtual ~Game_TicTacToe(void);							//Destructor

	//public procedures - support
	Game_Engine* Create_Duplicate_Game(bool copy_state = TOMGAME_ENGINE_COPY_GAME_STATE, const bool copy_history = TOMGAME_ENGINE_COPY_GAME_HISTORY);


protected:
	//private procedures
	void Init_Settings();
	bool Check_Game_Win(int selected_move);

};



#endif