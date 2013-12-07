/**
AI player using UCT tree algorithm with online reinforcement after each simulated move

ISSUES: PRESERVING TREE CURRENTLY NOT IMPLEMENTED COMPLETLY
*/


#ifndef _TOM_PLAYER_AI_UCT_REINFORCE_
#define _TOM_PLAYER_AI_UCT_REINFORCE_

//includes
#include "Player_AI_UCT.hpp"

/**
Basic player class
*/
class Player_AI_UCT_Reinforce : public Player_AI_UCT
{

public:
	//public procedures
	Player_AI_UCT_Reinforce(Game_Engine* game = NULL, int player_number = TOMPLAYER_PLAYER_NUMBER);
	virtual ~Player_AI_UCT_Reinforce();

protected:

	//private protected procedures
	virtual void Allocate_Memory();
	virtual void Clear_Memory();

	//private protected procedures - UCT
	virtual int			UCT();
	virtual UCTnode*	UCT_Tree_Policy(UCTnode* root);
	virtual double*		UCT_Default_Policy();
	virtual void		UCT_Backup(UCTnode* leaf, double* rewards);

	//private protected variables
	double*		online_rewards;

};

class Player_AI_UCT_Reinforce_DPrepeat : public Player_AI_UCT_Reinforce
{

public:
	//public procedures
	Player_AI_UCT_Reinforce_DPrepeat(Game_Engine* game = NULL, int player_number = TOMPLAYER_PLAYER_NUMBER);
	virtual ~Player_AI_UCT_Reinforce_DPrepeat();

protected:

	virtual double*		UCT_Default_Policy();

};

class Player_AI_UCT_Reinforce_DPpassive : public Player_AI_UCT_Reinforce
{

public:
	//public procedures
	Player_AI_UCT_Reinforce_DPpassive(Game_Engine* game = NULL, int player_number = TOMPLAYER_PLAYER_NUMBER);
	virtual ~Player_AI_UCT_Reinforce_DPpassive();

protected:

	virtual double*		UCT_Default_Policy();

};


#endif