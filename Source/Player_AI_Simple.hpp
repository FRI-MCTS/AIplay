/**
AI player statisticaly tracink outcomes for single actions only, based on averaging.

ISSUES: PRESERVING TREE CURRENTLY NOT IMPLEMENTED COMPLETLY
*/


#ifndef _TOM_PLAYER_AI_SIMPLE_
#define _TOM_PLAYER_AI_SIMPLE_

//includes
#include "Player_Engine.hpp"

//defines
enum{
	TOMPlayer_AI_Simple_MOVE_TYPE_EXPLORE,
	TOMPlayer_AI_Simple_MOVE_TYPE_BEST
};
#define TOMPlayer_AI_Simple_PARAM_EXPLORE_FACTOR	0.1
#define TOMPlayer_AI_Simple_PARAM_MOVE_TYPE		TOMPlayer_AI_Simple_MOVE_TYPE_EXPLORE
enum{
	TOMPlayer_AI_Simple_OUTPUT_TYPE_1D,
	TOMPlayer_AI_Simple_OUTPUT_TYPE_2D,
	TOMPlayer_AI_Simple_OUTPUT_TYPE_GO
};

/**
Basic AI player class
*/
class Player_AI_Simple : public Player_Engine
{

public:

	//public procedures
	Player_AI_Simple(Game_Engine* game = NULL, int new_player_number = TOMPLAYER_PLAYER_NUMBER);
	virtual ~Player_AI_Simple();
	void Reset();
	int  Get_Move();
	void End_Game();

	//public procedures - debug and visualization
	void Output();

	//public variables - player settings
	int selectMoveType;			//default set to 0 - playSingleMove(), value 1 - playSingleMoveBest()

	//public variables - learning parameters
	double explorationFactor;	//percentage [0,1] of exploratory moves

	//public vairables - debug and visualization settings
	int output_type;

private:

	//private procedures
	void Init_Settings();
	void Allocate_Memory();
	void Clear_Memory();

	int Play_Move();
	int Play_Move_Best();

	//private variables
	double* actionsWeight;
	int* actionsNumWin;
	int* actionsNumSel;
	bool* actionsPlayed;
	int all_actions_num;

};


#endif