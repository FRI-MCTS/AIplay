/**
AI player using UCT tree algorithm.

ISSUES: PRESERVING TREE CURRENTLY NOT IMPLEMENTED COMPLETLY
*/


#ifndef _TOM_PLAYER_AI_UCT_AMAF_
#define _TOM_PLAYER_AI_UCT_AMAF_

//includes
#include "Player_Engine.hpp"

//defines
#define TOMPLAYER_AI_UCT_AMAF_PARAM_NUMBER_ITERATIONS	1000
#define TOMPLAYER_AI_UCT_AMAF_PARAM_C					(1.0 / (2*sqrt(2)))
#define TOMPLAYER_AI_UCT_AMAF_PARAM_DEFAULT_MAX_PLAYS	(game->maximum_plys)
#define TOMPLAYER_AI_UCT_AMAF_PARAM_PRESERVE_TREE		false
#define TOMPLAYER_AI_UCT_AMAF_TREE_SIZE_LIMIT_GB			1.0
#define TOMPLAYER_AI_UCT_AMAF_BEST_CHILD_EXPLORATION_ENABLED		true
#define TOMPLAYER_AI_UCT_AMAF_OPTIMIZATION_INTERNAL_FORCE_COPY	false

#define TOMPLAYER_AI_UCT_AMAF_VARIANT					0	//valid values 0 (basic), 1 (UCT_explr without AMAF), 2 (separated AMAF + UCT)
															//variant 0 works best in game gomoku7x7 against vanilla UCT
#define TOMPLAYER_AI_UCT_AMAF_IGNORE_UNTIL_EXPANDED		1	//apply AMAF only to children of fully expanded parents (to prevent biasing the nodes that were randomly expanded first)
#define TOMPLAYER_AI_UCT_AMAF_DISABLE_AMAF				0	//set to 1 to disable AMAF (does not speed up computation time to vanilla UCT level)


//defines - DEBUG checking
#define TOMPLAYER_AI_UCT_AMAF_DEBUG_MEMORY_SIZE_COUNT	(1 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_AMAF_DEBUG_HISTORY_COPY_CHECK	(1 && TOM_DEBUG)

//defines - DEBUG behaviour
#define TOMPLAYER_AI_UCT_AMAF_DEBUG_DISABLE_RANDOM		((1 && TOM_DEBUG) || TOM_DISABLE_RANDOM)

//defines - DEBUG VISUALIZATION
#define TOMPLAYER_AI_UCT_AMAF_VISUALIZE_LEVEL			2		//set visualization depth
#define TOMPLAYER_AI_UCT_AMAF_VISUALIZE_LEVEL_UCT		((1 >= TOMPLAYER_AI_UCT_AMAF_VISUALIZE_LEVEL) && TOM_DEBUG)

#define TOMPLAYER_AI_UCT_AMAF_VISUALIZE_UCT_GETC_AFTER_MOVE	(0 && TOMPLAYER_AI_UCT_AMAF_VISUALIZE_LEVEL_UCT)
#define TOMPLAYER_AI_UCT_AMAF_VISUALIZE_UCT_ACTIONS_TREE	(0 && TOMPLAYER_AI_UCT_AMAF_VISUALIZE_LEVEL_UCT)
#define TOMPLAYER_AI_UCT_AMAF_VISUALIZE_UCT_ACTIONS_PLAYOUT	(0 && TOMPLAYER_AI_UCT_AMAF_VISUALIZE_LEVEL_UCT)

#define TOMPLAYER_AI_UCT_AMAF_DEBUG_ROOT_CHILDREN_VALUES	(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_AMAF_DEBUG_TRACE_ALL_UCT			(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_AMAF_DEBUG_TRACE_BEST_UCT			(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_AMAF_DEBUG_TREE_EXPANSION			(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_AMAF_DEBUG_TREE_EXPANSION_SIM		(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_AMAF_DEBUG_SIMULATED_GAMES_OUT		(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_AMAF_DEBUG_BACKUP_SHOW_AMAFTAGS	(0 && TOM_DEBUG)

/**
Basic AI UCT player class
*/
class Player_AI_UCT_AMAF : public Player_Engine
{

public:

	//public structures
	typedef struct UCTnode {

		//node-action link
		int action_index;

		//tree structure variables
		int number_allowed_actions;
		int number_explored_children;
		UCTnode* parent;
		UCTnode** children;

		//learning values
		double visits;
		double rewards;

		//AMAF heuristic
		double AMAF_visits;
		double AMAF_rewards;

		//memorized calculations
		double value;

	} UCTnode;

	//public procedures
	Player_AI_UCT_AMAF(Game_Engine* game = NULL, int player_number = TOMPLAYER_PLAYER_NUMBER);
	virtual ~Player_AI_UCT_AMAF();
	virtual void Reset();
	virtual void New_Game();
	virtual int  Get_Move();
	//void Before_Move(int move);
	//void End_Game();

	//public procedures - debug and visualization	
	virtual void Output_UCT_Tree();
	virtual void Output_UCT_Tree_Branch(UCTnode*);
	virtual void Output_UCT_Tree_Node_Children(UCTnode*);
	virtual void Debug_UCT_Tree_MemoryState();
	virtual int  Debug_UCT_Tree_Size(UCTnode*);

	//void Output();
	//void STDprint_UCTtree_state();	//debug procedure for printing current tree info
	//int  STDprintNumChildren(UCTnode* branchRoot);	//prints number of children of each node to standard output
	//int  STDprintTreeVal(UCTnode* branchRoot);	//prints number of children of each node to standard output

	//public procedures - UCT algorithm
	//void UCTperserveTree_enable();
	//void UCTperserveTree_disable();

	//public variables - player settings

	//public variables - optimization settings
	bool	internal_game_force_copy;

	//public variables - UCT learning parameters
	int		UCT_param_IterNum;			//number of simulation per move
	double	UCT_param_C;					//exploration factor
	int		UCT_param_defaultPolicy_maxPlys;	//maxium number of moves per simulated game
	int		UCTtree_maxMemorySize;		//maximum size of tree in memory

	//public variables - UCT
	UCTnode* UCTroot;
	UCTnode* UCTroot_memory;

	//public vairables - debug and visualization settings
	int  output_type;

protected:

	//private protected procedures
	virtual void Initialize();
	virtual void Init_Settings();
	virtual void Allocate_Memory();
	virtual void Clear_Memory();
	virtual void New_Game2();

	//private protected procedures - UCT
	virtual int			UCT_AMAF();
	virtual UCTnode*	UCT_AMAF_Tree_Policy(UCTnode* root, int simulation_number);
	virtual UCTnode*	UCT_AMAF_Tree_Policy_Best_Child(UCTnode* parent, double param_C);
	virtual double*		UCT_AMAF_Default_Policy(int simulation_number);
	virtual UCTnode*	UCT_Expand(UCTnode* parent);
	virtual void		UCT_AMAF_Backup(UCTnode* leaf, double* rewards, int simulation_number);

	virtual void		UCT_Update_Internal_Game(int number_actions);
	virtual void		UCT_Tree_Change_Root(int number_actions);
	virtual void		UCT_Tree_Trim(UCTnode* branchRoot, int remaining_depth);
	virtual void		UCT_Tree_Delete(UCTnode* branchRoot);
	virtual void		UCT_Tree_Reset();
	virtual void		UCT_Tree_Preserve();

	virtual UCTnode*	UCT_Tree_Node_Init(int action, UCTnode* parent, const bool rootNode = 0);	//node: initialize
	virtual void		UCT_Tree_Node_AllocateList(UCTnode* node, int list_length);	//node: allocate children list
	virtual void		UCT_Tree_Node_Reset(UCTnode* node);		//node: reset values to initial

	//private protected procedures - debug and visualization	


	//private protected variables
	//double* actionsWeight;
	//int* actionsNumWin;
	//int* actionsNumSel;
	//bool* actionsPlayed;
	int all_actions_num;

	//private protected variables - UCT
	int UCTtree_treePolicy_lastDepth;
	int UCTtree_num_nodes;
	int UCTtree_memorySize;
	Game_Engine* internalGame;
	Game_Engine* simulatedGame;
	UCTnode* lastAction_node;		//last played move
	UCTnode* UCTtree_newRoot;		//next UCT root

	int* AMAF_flagList;

	//private protected variables - UCT parameters
	bool UCTtree_preserve;

	//private protected variables - debug and visualization
	stringstream visualizeActionsTree;
	stringstream visualizeActionsPlayout;
};




#endif