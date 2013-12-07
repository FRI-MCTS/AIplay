/**
AI player using UCT tree algorithm.

ISSUES: PRESERVING TREE CURRENTLY NOT IMPLEMENTED COMPLETLY
*/


#ifndef _TOM_PLAYER_AI_UCT_TOMTEST_
#define _TOM_PLAYER_AI_UCT_TOMTEST_

//includes
#include "Player_Engine.hpp"
#include "Tom_Lrp.hpp"

//defines
#define TOMPLAYER_AI_UCT_TOMTEST_PARAM_NUMBER_ITERATIONS	1000
#define TOMPLAYER_AI_UCT_TOMTEST_PARAM_SIMULATIONS_PER_ITER	1
#define TOMPLAYER_AI_UCT_TOMTEST_PARAM_C					(1.0 / (2*sqrt(2)))
#define TOMPLAYER_AI_UCT_TOMTEST_PARAM_DEFAULT_MAX_PLAYS	(game->maximum_plys)
#define	TOMPLAYER_AI_UCT_TOMTEST_PARAM_MOV_AVG_ALPHA		0.001
#define TOMPLAYER_AI_UCT_TOMTEST_PARAM_PRESERVE_TREE		false
#define TOMPLAYER_AI_UCT_TOMTEST_TREE_SIZE_LIMIT_GB			1.0
#define TOMPLAYER_AI_UCT_TOMTEST_BEST_CHILD_EXPLORATION_ENABLED		true
#define TOMPLAYER_AI_UCT_TOMTEST_OPTIMIZATION_INTERNAL_FORCE_COPY	false

#define TOMPLAYER_AI_UCT_TOMTEST_SIMULATIONS_CALC_VARIANCE	false	//store individual results for multiple simulations per iteration and calculate variance

#define TOMPLAYER_AI_UCT_TOMTEST_FUNC_APPROX_NUM_PARAMS		2

//TESTING OPPONENT MODEL
#define TOMPLAYER_AI_UCT_TOMTEST_RANDOM_TREE_OPPONENT_THRESHOLD		0.0		//0.0 = disabled







//defines - DEBUG checking
#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_MEMORY_SIZE_COUNT	(1 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_HISTORY_COPY_CHECK	(1 && TOM_DEBUG)

//defines - DEBUG behaviour
#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_DISABLE_RANDOM		((1 && TOM_DEBUG) || TOM_DISABLE_RANDOM)

//defines - DEBUG VISUALIZATION
#define TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_LEVEL			2		//set visualization depth
#define TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_LEVEL_UCT		((1 >= TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_LEVEL) && TOM_DEBUG)



#define TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_UCT_GETC_AFTER_MOVE	(0 && TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_LEVEL_UCT)
#define TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_UCT_ACTIONS_TREE		(0 && TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_LEVEL_UCT)
#define TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_UCT_ACTIONS_PLAYOUT	(0 && TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_LEVEL_UCT)

#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_ROOT_CHILDREN_VALUES	(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_TRACE_BEST_UCT		(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_TREE_EXPANSION		(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_TREE_EXPANSION_SIM	(0 && TOM_DEBUG)
#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_SIMULATED_GAMES_OUT	(0 && TOM_DEBUG)

#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_DISABLE_RANDOM_MULTIPLE_BEST	0
#define TOMPLAYER_AI_UCT_TOMTEST_DEBUG_DISABLE_RANDOM_EXPAND		1

/**
Basic AI UCT player class
*/
class Player_AI_UCT_TomTest : public Player_Engine
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

		//memorized calculations
		double value;

		//new indicators
		int last_update_iteration;
		int	num_simulations;
		double num_plys_sim_phase;
		double num_plys_to_end;

		double computation_time;
		double knowledge_gain;

		double visits_in_last_UCT;		//number of visits in the last call of UCT() (since root was lastly changed and tree prunned)

		double moving_average_value;
		double moving_average_num_plys_sim_phase;
		double moving_average_num_plys_to_end;

	} UCTnode;

	//public procedures
	Player_AI_UCT_TomTest(Game_Engine* game = NULL, int player_number = TOMPLAYER_PLAYER_NUMBER);
	virtual ~Player_AI_UCT_TomTest();
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
	int		UCT_param_IterNum;					//number of iterations per move
	int		UCT_param_SimPerIter;				//number of simulations per iteration
	double	UCT_param_C;						//exploration factor
	int		UCT_param_defaultPolicy_maxPlys;	//maxium number of moves per simulated game
	int		UCTtree_maxMemorySize;				//maximum size of tree in memory

	double	UCT_param_mov_avg_alpha;			//weight factor of last sample when calculating moving average value of node in tree in UCT_Backup()

	//public variables - UCT learning parameters

	//public variables - UCT
	UCTnode* UCTroot;
	UCTnode* UCTroot_memory;

	//public variables - function approximation parameters
	Tom_Function_Approximator* Func_App_UCT_Params;
	int Func_App_UCT_num_params;

	//public variables - current state
	int UCT_num_plys;
	int MCTS_current_iterNum;
	int MCTS_current_simuNum;
	int MCTS_num_all_iterations;
	int MCTS_num_all_simulations;

	//public vairables - debug and visualization settings
	int  output_type;
	double	debug_dbl_cnt1, debug_dbl_cnt2;

protected:

	//private protected procedures
	virtual void Initialize();
	virtual void Init_Settings();
	virtual void Allocate_Memory();
	virtual void Clear_Memory();
	virtual void New_Game2();

	//private protected procedures - UCT
	virtual int			UCT();
	virtual UCTnode*	UCT_Tree_Policy(UCTnode* root, int MCTS_current_iterNum);
	virtual UCTnode*	UCT_Tree_Policy_Best_Child(UCTnode* parent, double exploration_weight, int MCTS_current_iterNum);
	virtual void		UCT_Simulate(int num_simulations, const bool calc_variance = TOMPLAYER_AI_UCT_TOMTEST_SIMULATIONS_CALC_VARIANCE);
	virtual void		UCT_Default_Policy();
	virtual UCTnode*	UCT_Expand(UCTnode* parent);
	virtual void		UCT_Backup(UCTnode* leaf);

	virtual void		UCT_Update_Internal_Game(int number_actions);
	virtual void		UCT_Tree_Change_Root(int number_actions);
	virtual void		UCT_Tree_Trim(UCTnode* branchRoot, int remaining_depth);
	virtual void		UCT_Tree_Delete(UCTnode* branchRoot);
	virtual void		UCT_Tree_Reset();
	virtual void		UCT_Tree_Preserve();

	virtual UCTnode*	UCT_Tree_Node_Init(int action, UCTnode* parent);	//node: initialize
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

	//feedback information from simulations
	UCTnode*	UCT_selected_leaf;
	Game_Engine* simulatedGame_startingState;
	int			sim_feedback_num_sims;		//should this be long long ?
	double*		sim_feedback_scores_avg;
	double*		sim_feedback_scores_var;
	double**	sim_feedback_scores;
	double		sim_feedback_plys_avg;
	double		sim_feedback_plys_var;
	double*		sim_feedback_plys;

	//private protected variables - UCT parameters
	bool UCTtree_preserve;

	//private protected variables - debug and visualization
	stringstream visualizeActionsTree;
	stringstream visualizeActionsPlayout;
};




#endif