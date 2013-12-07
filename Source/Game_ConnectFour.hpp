#ifndef _TOM_GAME_CONNECTFOUR_
#define _TOM_GAME_CONNECTFOUR_

//includes
#include "Game_Gomoku.hpp"

//default values
#define TOMGAME_FOURROW_BOARD_LENGTH			7
#define TOMGAME_FOURROW_BOARD_HEIGHT			6
#define TOMGAME_FOURROW_WIN_CONNECTED_PIECES	4
#define TOMGAME_FOURROW_SCORE_WIN				1.0
#define TOMGAME_FOURROW_SCORE_LOSE				0.0
#define TOMGAME_FOURROW_SCORE_DRAW				0.5

/**
Definiton of the Four in a row game engine.

Scoring: win 1, lose 0 (see defines)
*/
#pragma once
class Game_ConnectFour : public Game_Gomoku
{

public:
	
	//public procedures - engine
	Game_ConnectFour(Game_Engine* source_game = TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY);	//Constructor
	virtual ~Game_ConnectFour(void);							//Destructor

	//public procedures - support
	Game_Engine* Create_Duplicate_Game(bool copy_state = TOMGAME_ENGINE_COPY_GAME_STATE, const bool copy_history = TOMGAME_ENGINE_COPY_GAME_HISTORY);
	int	 Human_Move_Input();
	int  Human_Move_Translate(int human_move);
	
	//public procedures - debug and visualization
	void Output_Board_State();

protected:
	//private procedures
	void Init_Settings();
	void Copy_Settings(Game_Engine* source_game);
	int  Game_Dynamics(int selected_move);

	//private variables - game state

};




#endif