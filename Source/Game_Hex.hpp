#ifndef _TOM_GAME_HEX_
#define _TOM_GAME_HEX_

//#includes
#include "Game_Engine.hpp"

//default values
#define TOMGAME_HEX_BOARD_LENGTH			11
#define TOMGAME_HEX_BOARD_HEIGHT			TOMGAME_HEX_BOARD_LENGTH
#define TOMGAME_HEX_BIG_BOARD_LENGTH		TOMGAME_HEX_BOARD_LENGTH + 2
#define TOMGAME_HEX_BIG_BOARD_HEIGHT		TOMGAME_HEX_BIG_BOARD_LENGTH
//#define TOMGAME_HEX_WIN_CONNECTED_PIECES 5
#define TOMGAME_HEX_SCORE_WIN			1.0
#define TOMGAME_HEX_SCORE_LOSE			0.0
//#define TOMGAME_HEX_SCORE_DRAW			0.5

/**
Definiton of the Hex game engine.

Scoring: win 1, lose 0 (see defines)
*/
#pragma once
class Game_Hex : public Game_Engine
{

public:
	
	//public procedures - engine
	Game_Hex(Game_Engine* source_game = TOMGAME_ENGINE_CONSTRUCTOR_NO_COPY);	//Constructor
	virtual ~Game_Hex(void);							//Destructor
	void Game_Reset();

	//public procedures - support
	void Copy_Game_State_From(Game_Engine* source, bool copy_history = TOMGAME_ENGINE_COPY_GAME_HISTORY, int history_copy_start_index = 0);
	Game_Engine* Create_Duplicate_Game(bool copy_state = TOMGAME_ENGINE_COPY_GAME_STATE, bool copy_history = TOMGAME_ENGINE_COPY_GAME_HISTORY);
	using Game_Engine::Get_Next_Player;
	int Get_Next_Player(int);

	//public procedures - debug and visualization

	//public variables - game settings
	int win_connected_pieces;

	//public variables - game state

	//public variables - game history

	//public variables - debug


protected:
	//private procedures
	void Init_Settings();
	void Allocate_Memory();
	void Clear_Memory();
	void Clear_Constants();
	void Copy_Settings(Game_Engine* source_game);
	int  Game_Dynamics(int selected_move);
	bool Check_Game_Win(int position);
	void Flood(int position,int oldValue,int newValue);

	//private procedures - support
	int  Human_Move_Translate(int human_move);

	//private variables - game state
	int*	sosedje;
};



#endif