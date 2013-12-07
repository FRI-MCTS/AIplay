//include header
#include "Player_AI_UCT_TomTest.hpp"

/**
Constructor for AI UCT player module calling general game player constructor

@param game Pointer to a Game_Engine object (or derivated class)
*/
Player_AI_UCT_TomTest::Player_AI_UCT_TomTest(Game_Engine* game, int player_number) : Player_Engine(game, player_number)
{
	//player definition
	player_name = "UCT_TomTest";

	//call initialization procedures
	if(game != NULL)
		Initialize();
	else
		is_initialized = false;
}

//destructor
Player_AI_UCT_TomTest::~Player_AI_UCT_TomTest(void)
{
	Clear_Memory();
}

//allocate memory and initialize variables
void Player_AI_UCT_TomTest::Initialize()
{
	//set init flag
	is_initialized = true;

	//initialize settings
	Init_Settings();

	//allocate resources
	Allocate_Memory();
	
	//reset game state
	New_Game2();
}

void Player_AI_UCT_TomTest::Init_Settings()
{
	//init internal variables
	all_actions_num = game->maximum_allowed_moves;
	UCTtree_maxMemorySize = (int)(TOMPLAYER_AI_UCT_TOMTEST_TREE_SIZE_LIMIT_GB * (1<<30));
	UCTtree_num_nodes = 0;
	UCTtree_memorySize = 0;
	
	Func_App_UCT_num_params = TOMPLAYER_AI_UCT_TOMTEST_FUNC_APPROX_NUM_PARAMS;
	Func_App_UCT_Params = NULL;

	UCT_num_plys = 0;
	MCTS_current_iterNum = 0;
	MCTS_current_simuNum = 0;
	MCTS_num_all_iterations = 0;
	MCTS_num_all_simulations = 0;

	//init optimization settings
	internal_game_force_copy = TOMPLAYER_AI_UCT_TOMTEST_OPTIMIZATION_INTERNAL_FORCE_COPY;

	//init learning parameters
	UCT_param_C = TOMPLAYER_AI_UCT_TOMTEST_PARAM_C;
	UCT_param_IterNum = TOMPLAYER_AI_UCT_TOMTEST_PARAM_NUMBER_ITERATIONS;
	UCT_param_SimPerIter = TOMPLAYER_AI_UCT_TOMTEST_PARAM_SIMULATIONS_PER_ITER;
	UCT_param_defaultPolicy_maxPlys = TOMPLAYER_AI_UCT_TOMTEST_PARAM_DEFAULT_MAX_PLAYS;
	UCT_param_mov_avg_alpha = TOMPLAYER_AI_UCT_TOMTEST_PARAM_MOV_AVG_ALPHA;
	UCTtree_preserve = TOMPLAYER_AI_UCT_TOMTEST_PARAM_PRESERVE_TREE;
	
	//debug variables
	debug_dbl_cnt1 = 0;
	debug_dbl_cnt2 = 0;
}

void Player_AI_UCT_TomTest::Allocate_Memory()
{
	//allocate resources
	UCTroot = UCT_Tree_Node_Init(-1, NULL);
	UCTroot_memory = UCTroot;
	internalGame = game->Create_Duplicate_Game();
	simulatedGame = game->Create_Duplicate_Game();
	simulatedGame_startingState = game->Create_Duplicate_Game();

	//simulations feedback structures
	sim_feedback_scores_avg = new double[game->number_players];
	sim_feedback_scores_var = new double[game->number_players];
	sim_feedback_scores = new double*[UCT_param_SimPerIter];
	for(int i = 0; i < UCT_param_SimPerIter; i++)
		sim_feedback_scores[i] = new double[game->number_players];
	sim_feedback_plys = new double[UCT_param_SimPerIter];

}

void Player_AI_UCT_TomTest::Clear_Memory()
{
	//clean up memory
	if(is_initialized){
		if(UCTtree_preserve)
			UCT_Tree_Delete(UCTroot_memory);
		else
			UCT_Tree_Delete(UCTroot);

		delete(internalGame);
		delete(simulatedGame);

		//simulations feedback structures
		delete(simulatedGame_startingState);
		delete(sim_feedback_scores_avg);
		delete(sim_feedback_scores_var);
		for(int i = 0; i < UCT_param_SimPerIter; i++)
			delete(sim_feedback_scores[i]);
		delete(sim_feedback_scores);
		delete(sim_feedback_plys);
	}
}

void Player_AI_UCT_TomTest::New_Game2()
{

	//RESET internal game copy
	internalGame->Copy_Game_State_From(game);
	
	//null history-storing variables
	lastAction_node = NULL;
}

//public procedure: reset player (reset all learning values)
void Player_AI_UCT_TomTest::Reset()
{

	//delete/free entire tree except root
	UCT_Tree_Reset();

	//reset root
	UCTroot->parent = NULL;
	UCT_Tree_Node_Reset(UCTroot);

	return New_Game2();
}

//public procedure: signal player that new game has been started
void Player_AI_UCT_TomTest::New_Game()
{

	//reset root (and reset tree, optionally)
	if(UCTtree_preserve){
		UCTroot = UCTroot_memory;
		New_Game2();
	}else{
		Reset();		//calls reset1(), newGame(), newGame1()
	}

}

//public procedure: returns players next selected move
int Player_AI_UCT_TomTest::Get_Move()
{

	int moveNumber;
	int number_internal_moves;

	//debug: display tree state
	//STDprint_UCTtree_state();
	//STDprintTreeVal(root);

	//How many moves were made in external game since last Get_Move() call
	number_internal_moves = (game->current_plys - internalGame->current_plys);
	
	//Update internal game to match external game (replay moves in internal game)
	UCT_Update_Internal_Game(number_internal_moves);

	//Correct tree state: set new root and delete part of tree that is unrelevant
	UCT_Tree_Change_Root(number_internal_moves);

	//Execute UCT algortihm and select best action
	moveNumber = UCT();

	//DEBUG
#if(TOMPLAYER_AI_UCT_TOMTEST_DEBUG_ROOT_CHILDREN_VALUES)
	Output_UCT_Tree_Node_Children(UCTroot);		//show values of roots actions (children)
#endif
#if(TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_UCT_GETC_AFTER_MOVE)
	cin.get();
#endif

	//return selected move
	return moveNumber;
}

//Updates internal game to match external game (replay moves in internal game)
void Player_AI_UCT_TomTest::UCT_Update_Internal_Game(int number_actions)
{

	//check if external game should be copied to or replayed in internal game
	if((game->is_deterministic)&&(internal_game_force_copy == false)){

		//play all players moves in internal copy of game (including own last move)
		for(int i = number_actions; i > 0; i--)
			internalGame->Play_Move(game->history_moves[game->current_plys-i+1]);

	//copy external game to internal game
	}else{
		internalGame->Copy_Game_State_From(game,true,internalGame->current_plys-1);
	}
	
	//DEBUG: check if move history of external and internal game is equal
#if(TOMPLAYER_AI_UCT_TOMTEST_DEBUG_HISTORY_COPY_CHECK)
	for(int i = 0; i <= game->current_plys; i++){
		if(game->history_moves[i] != internalGame->history_moves[i]){
			printf("PLAYER AI UCT: UCT_Update_Internal_Game(): game copy error - external and internal game history does not match at index %d: ext %d int %d ... RESETTING UCT PLAYER\n",i,game->history_moves[i],internalGame->history_moves[i]);
			i = game->current_plys+1;
			Reset();
		}
	}
#endif

}

int Player_AI_UCT_TomTest::UCT()
{

	//execute simulations with given computational budget
	for(MCTS_current_iterNum = 0; MCTS_current_iterNum < UCT_param_IterNum; MCTS_current_iterNum++){

		//RESET simulated game position to initial state
		simulatedGame->Copy_Game_State_From(internalGame,false);
		 
		//DEBUG
		//internalGame->output_chains_to_STD();
		//simulatedGame->output_chains_to_STD();
		//simulatedGame->output_state_to_STD();
		//printf("sim player %d\n",simulatedGame->current_player);

		//UCT algorithm
		UCT_selected_leaf = UCT_Tree_Policy(UCTroot, MCTS_current_iterNum);
		UCT_Simulate(UCT_param_SimPerIter);		//simulates game until end using UCT_Default_Policy() and sets global "sim_feedback..." values
		UCT_Backup(UCT_selected_leaf);			//reads global "sim_feedback..." values

		//check if memory limit was exceeded - TODO: this is a basic version that work well if only 1 node is added per iteration, otherwise the procedure must be implemented inside UCTtreePolicy, where new nodes are allocated
		if(UCTtree_memorySize >= UCTtree_maxMemorySize){
			MCTS_current_iterNum = UCT_param_IterNum;	//end the simulations loop
		}

		//increase global iteration counter
		MCTS_num_all_iterations++;

		//DEBUG
#if(TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_LEVEL_UCT)
		printf("\nSim num %2d     score",i);
		for(int t = 0; t < game->number_players; t++)
			printf("  %3.5f",final_rewards[t]);
#if(!TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_UCT_ACTIONS_TREE)
		printf("     root  %2d", UCTroot->action_index);
#else
		printf("     tree  %2d", UCTroot->action_index);
		printf("%s",visualizeActionsTree.str().c_str());
		visualizeActionsTree.str("");		//set stringstream to empty
		visualizeActionsTree.clear();		//clear flags
#endif
#if(TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_UCT_ACTIONS_PLAYOUT)
		printf("    \t playout:");
		printf("%s",visualizeActionsPlayout.str().c_str());
		visualizeActionsPlayout.str("");	//set stringstream to empty
		visualizeActionsPlayout.clear();	//clear flags
#endif
#endif

#if(TOMPLAYER_AI_UCT_TOMTEST_DEBUG_TREE_EXPANSION_SIM)
		Output_UCT_Tree();
#endif

	}

	//DEBUG
#if(TOMPLAYER_AI_UCT_TOMTEST_DEBUG_TREE_EXPANSION)
	Output_UCT_Tree();
#endif
	
	//return best action from root (without exploring factors) and remember node
	if(UCTroot->number_explored_children > 0){
		lastAction_node = UCT_Tree_Policy_Best_Child(UCTroot, 0.0, 0);
		return (lastAction_node->action_index);

	//no simulations run - return random action
	}else if(UCT_param_IterNum == 0){
		lastAction_node = NULL;
		return game->Select_Move_Random();

	//no valid action (no children explored), return error
	}else{
		printf("PLAYER AI UCT: UCT(): action selection error - no valid action available\n");
		lastAction_node = NULL;
		return -1;
	}

	//increase global counter of invoked UCT plys
	UCT_num_plys++;
}

Player_AI_UCT_TomTest::UCTnode* Player_AI_UCT_TomTest::UCT_Tree_Policy(UCTnode* root, int MCTS_current_iterNum)
{

	UCTnode* currentNode = root;
	int terminalNode = (int)(simulatedGame->game_ended);

	//move through the tree and game
	while(terminalNode == 0){
		
		//check expanding condition and descend tree according to tree policy
		if(currentNode->number_explored_children >= simulatedGame->current_number_moves[simulatedGame->current_player]){
	
			//select best child node/action
			currentNode = UCT_Tree_Policy_Best_Child(currentNode, 1.0, MCTS_current_iterNum);

			//play simulated game (if game not ended yet playMove() returns value 0)
			terminalNode = simulatedGame->Play_Move(currentNode->action_index);
		
			//DEBUG
#if(TOMPLAYER_AI_UCT_TOMTEST_DEBUG_TRACE_BEST_UCT)
			printf("Best node: %d  %4.1f %3.0f  %3.0f  %3.3f\n", currentNode->action_index, currentNode->rewards, currentNode->parent->visits, currentNode->visits, currentNode->value);
#endif

		//expand if not fully expanded
		}else{

			//select a nontried action
			currentNode = UCT_Expand(currentNode);

			//play simulated game
			simulatedGame->Play_Move(currentNode->action_index);

			//TODO: DEBUG_ID001 why this doesnt work? see other DEBUG_ID001
			//currentNode->number_allowed_actions = simulatedGame->current_number_moves[simulatedGame->current_player];

			//is last node in tree
			terminalNode = 1;
		}

#if(TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_UCT_ACTIONS_TREE)
		//sprintf(visualizeActionsTree, "  > %2d", currentNode->action_index);
		if(currentNode->action_index >= 10)
			visualizeActionsTree << "  > " << currentNode->action_index;
		else
			visualizeActionsTree << "  >  " << currentNode->action_index;
		//printf("  > %2d", currentNode->action_index);
#endif

		//DEBUG
		//simulatedGame->output_state_to_STD();
		//simulatedGame->output_chains_to_STD();
#if(TOMPLAYER_AI_UCT_TOMTEST_AMAF_DEBUG_SIMULATED_GAMES_OUT)
		printf("-tree-\n");
		simulatedGame->Output_Board_State();
		//simulatedGame->output_state_to_STD();
		//simulatedGame->output_chains_to_STD();
		//printf("Moves: %d %d\n",simulatedGame->num_moves[0], simulatedGame->num_moves[1]);
#endif
	}

	//save number of actions from root to selected leaf (search depth)
	UCTtree_treePolicy_lastDepth = simulatedGame->current_plys - internalGame->current_plys;

	//return selected leaf
	return currentNode;

};

/**
Select next child from given tree node and with given UCT exploration weight C.
*/
Player_AI_UCT_TomTest::UCTnode* Player_AI_UCT_TomTest::UCT_Tree_Policy_Best_Child(UCTnode* parent, double exploration_weight, int MCTS_current_iterNum){

	double bestValue;
	int multiple_best, randAction;
	UCTnode* selectedChild;

	bestValue = -DBL_MAX;
	selectedChild = NULL;

	//OPPONENT RANDOM MOVES
	if(((simulatedGame->current_plys - internalGame->current_plys) % 2 == 1) && ((rand()/(float)(RAND_MAX+1)) < TOMPLAYER_AI_UCT_TOMTEST_RANDOM_TREE_OPPONENT_THRESHOLD)){
		
		multiple_best = parent->number_allowed_actions;
	
	}else{
	
		//find best-valued children node
		multiple_best = 1;
		for(int i = 0; i < parent->number_allowed_actions; i++){
		
			//check if children was already allocated (for safety)
			if(parent->children[i] != NULL){

				//note: param_C renamed exploration_weight

				//Param_C dynamic change
				//-- number of plys since game start
				//param_C = param_C * ( 2 - (double)simulatedGame->current_plys / simulatedGame->maximum_plys);
				//param_C = param_C * ( 1 - (double)simulatedGame->current_plys / simulatedGame->maximum_plys);
				//param_C = param_C * ( 1.3 - 1.0*(double)simulatedGame->current_plys / simulatedGame->maximum_plys);
				//param_C = param_C * ( 1.0 - 0.7*(double)simulatedGame->current_plys / simulatedGame->maximum_plys);
				//param_C = param_C * ( 0 + (double)simulatedGame->current_plys / simulatedGame->maximum_plys);
				//param_C = param_C * ( 1 + (double)simulatedGame->current_plys / simulatedGame->maximum_plys);
				//param_C = param_C * ( 0.5 + (double)simulatedGame->current_plys / simulatedGame->maximum_plys);

				//-- number of plys of tree root since game start
				//param_C = param_C * ( 1.3 - 1.0*(double)internalGame->current_plys / internalGame->maximum_plys);
				//param_C = param_C * ( 1.0 - 0.7*(double)internalGame->current_plys / internalGame->maximum_plys);
				
				//-- depth of node in the tree
				//param_C = param_C * ( 0.3 + 1.0*(double)(simulatedGame->current_plys - internalGame->current_plys) / (internalGame->maximum_plys - internalGame->current_plys));
				//param_C = param_C * ( 0.5 + 1.0*(double)(simulatedGame->current_plys - internalGame->current_plys) / (internalGame->maximum_plys - internalGame->current_plys));
				//param_C = param_C * ( 1.0 + 0.3*(double)(simulatedGame->current_plys - internalGame->current_plys) / (internalGame->maximum_plys - internalGame->current_plys));
				//param_C = param_C * ( 0.3 + 0.7*(double)(simulatedGame->current_plys - internalGame->current_plys) / (internalGame->maximum_plys - internalGame->current_plys));
				
				//param_C = param_C;


// ---- moving average instead of normal average ---- //
				//double app_val_C = UCT_param_C * exploration_weight;	//neccessary for select best child (when C must be 0.0)
				//parent->children[i]->value = 
				//	(parent->children[i]->moving_average_value) +
				//	//((parent->children[i]->rewards / parent->children[i]->visits)) +
				//	2.0 * app_val_C * sqrt(2*log(parent->visits) / parent->children[i]->visits);

// ---- USE THIS ONE FOR   LRP_improved_v1() and LRP_test_wrapper() ---- //
				//UCB equation with Function Apporximator for parameter C
				//- divided into intervals by MCTS_current_iterNum

				const double MCTS_simNum_interval_threshold = 0.0;
				const double default_cp_val = 0.0;

				double app_val_C = UCT_param_C;

				Func_App_UCT_Params->Calculate_Results();
				//change C based on number of current simulation
				//if(MCTS_current_iterNum >= (int)(UCT_param_IterNum*MCTS_simNum_interval_threshold))
					//app_val_C = Func_App_UCT_Params->results[0];
				//app_val_C = Func_App_UCT_Params->results[0]+Func_App_UCT_Params->results[1];
				//else
				//	app_val_C = Func_App_UCT_Params->results[1];

				//-- single NN: weighted output --//
				//WARNING: THE NUMBER OF Func_App_UCT_Params must be set in Player_AI_UCT_TomTest.hpp with the constant TOMPLAYER_AI_UCT_TOMTEST_FUNC_APPROX_NUM_PARAMS
				double sumNet =
					//Func_App_UCT_Params->results[0] * 1.0 +	//bias
					//Func_App_UCT_Params->results[0] * (parent->number_allowed_actions / game->maximum_allowed_moves) +	//num allowed actions
					//Func_App_UCT_Params->results[0] * (MCTS_current_iterNum / UCT_param_IterNum) +		//number of current simulation
					//Func_App_UCT_Params->results[0] * (game->current_plys / game->maximum_plys) +	//external game current ply
					//Func_App_UCT_Params->results[0] * (simulatedGame->current_plys / simulatedGame->maximum_plys) +	//simulated game current ply
					
					//-- tests "C single NN search" 2013_11_08 --//

					//test 1
					Func_App_UCT_Params->results[0] * ((double)MCTS_current_iterNum / UCT_param_IterNum) +	//MCTS current iteration number / max iterations per move
					Func_App_UCT_Params->results[1] * (parent->children[i]->num_plys_sim_phase / parent->children[i]->visits / simulatedGame->maximum_plys) +	//avg. plys in the sim phase / maximum game plys
					//test 2
					//Func_App_UCT_Params->results[2] * (parent->children[i]->visits_in_last_UCT / UCT_param_IterNum) +	//visits in last MCTS iteration batch (since last external move) / max iterations per move
					//Func_App_UCT_Params->results[3] * (parent->number_allowed_actions / game->maximum_allowed_moves) +	//parent num allowed action / max possible actions
					//test 3
					//Func_App_UCT_Params->results[4] * (game->current_plys / game->maximum_plys) +
					//Func_App_UCT_Params->results[5] * (simulatedGame->current_plys / simulatedGame->maximum_plys) +
					//bias
					Func_App_UCT_Params->results[1] * 1.0 +	//bias

					0;
					
				app_val_C = 1.0 / (1.0 + exp(-sumNet));
				app_val_C = (app_val_C - 0.5) * 2 + UCT_param_C;
				//-- END - single NN: weighted output --//


				//-- DEBUG PRINT --
				//printf("%3.3f\t%3.3f\n", (double)MCTS_current_iterNum / UCT_param_IterNum, (parent->children[i]->num_plys_sim_phase / parent->children[i]->visits / simulatedGame->maximum_plys));
				//if(parent->children[i]->number_allowed_actions != 7)
				//printf("%3.3f\t%d\t%d\n",parent->children[i]->visits_in_last_UCT / UCT_param_IterNum, parent->children[i]->number_allowed_actions,  game->maximum_allowed_moves);
				//printf("%3.3f\t%3.3f\n", (double)game->current_plys / game->maximum_plys, (double)simulatedGame->current_plys / simulatedGame->maximum_plys);
				//-- END - DEBUG PRINT --


				//-- direct C search --//
				//app_val_C = Func_App_UCT_Params->results[0];
				//-- END - direct C search --//

				//-- two C intervals, depending on different indicators --//
				////if((parent->children[i]->num_plys_sim_phase / parent->children[i]->visits) * 4.0 < simulatedGame->maximum_plys){	//first and second half of UCT iterations, ratio approximately 45%
				////if(MCTS_current_iterNum*2 < UCT_param_IterNum){	//first and second half of UCT iterations, ratio should be 50% (not tested)
				//if(parent->number_allowed_actions < game->maximum_allowed_moves){ //based on number of actions in node
				//	app_val_C = Func_App_UCT_Params->results[0];
				//	//app_val_C = UCT_param_C;
				//	//app_val_C = 0.5;
				//	debug_dbl_cnt1 += 1.0;
				//}else{
				//	app_val_C = Func_App_UCT_Params->results[1];
				//	//app_val_C = UCT_param_C;
				//	//app_val_C = 0.5;
				//}
				//debug_dbl_cnt2 += 1.0;
				//-- END - two C intervals, depending on different indicators --//


				//app_val_C = -0.5;
				app_val_C *= exploration_weight;	//neccessary for select best child (when C must be 0.0)
				parent->children[i]->value = 
					(parent->children[i]->rewards / parent->children[i]->visits) +
					2.0 * app_val_C * sqrt(2*log(parent->visits) / parent->children[i]->visits);



// ---- USE THIS ONE FOR   Param_Impact_Testing_v06() ---- //
				//const double MCTS_simNum_interval_threshold = 0.5;
				//const double default_cp_val = -0.1;

				//double app_val_C;
				//if(MCTS_current_iterNum >= (int)(UCT_param_IterNum*MCTS_simNum_interval_threshold))
				//	app_val_C = param_C;
				//else
				//	app_val_C = default_cp_val;

				//parent->children[i]->value = 
				//	(parent->children[i]->rewards / parent->children[i]->visits) +
				//	2.0 * app_val_C * sqrt(2*log(parent->visits) / parent->children[i]->visits);


				//UCB equation with Function Apporximator for parameter C
				//Func_App_UCT_Params->Calculate_Results(param_C);
				//double app_val_C = Func_App_UCT_Params->results[0];
				//parent->children[i]->value = 
				//	(parent->children[i]->rewards / parent->children[i]->visits) +
				//	2.0 * app_val_C * sqrt(2*log(parent->visits) / parent->children[i]->visits);


// ---- plain UCB equation ---- //
				//double app_val_C = UCT_param_C * exploration_weight;	//neccessary for select best child (when C must be 0.0)
				//parent->children[i]->value = 
				//	(parent->children[i]->rewards / parent->children[i]->visits) +
				//	2.0 * app_val_C * sqrt(2*log(parent->visits) / parent->children[i]->visits);



				//printf("DEBUG VAL: %d %3.1f\n", i, value);

				//remember best children and count number of equal best children
				if(parent->children[i]->value == bestValue){
					#if((!TOMPLAYER_AI_UCT_TOMTEST_DEBUG_DISABLE_RANDOM)&&(!TOMPLAYER_AI_UCT_TOMTEST_DEBUG_DISABLE_RANDOM_MULTIPLE_BEST))
					multiple_best++;
					#endif
				}else if(parent->children[i]->value > bestValue){
					selectedChild = parent->children[i];
					bestValue = parent->children[i]->value;
					multiple_best = 1;
				}
			}
		}

	}

	//if multiple best actions/children, break ties uniformly random
	if(multiple_best > 1){
		randAction = (int)( (rand()/(float)(RAND_MAX+1)) * multiple_best );
		for(int i = 0; i < parent->number_allowed_actions; i++){
			if(parent->children[i] != NULL){
				if(parent->children[i]->value >= bestValue){					
					if(randAction <= 0){
						selectedChild = parent->children[i];
						i = parent->number_allowed_actions;	//break loop
					}else{
						randAction--;
					}
				}
			}
		}
	}

	//return selected untried action
	return selectedChild;
}

/**
Expand the tree
*/
Player_AI_UCT_TomTest::UCTnode* Player_AI_UCT_TomTest::UCT_Expand(UCTnode* parent){

	int randAction, cntAvailable;
	UCTnode* selectedChild;

#if(TOM_DEBUG)
	if((parent->children == NULL)&&(parent->number_explored_children > 0)){
		printf("WARNING: PLAYER AI UCT: UCT_Expand(): node children list NOT allocated but number of explored children is %d\n",parent->number_explored_children);
	}else if((parent->children != NULL)&&(parent->number_explored_children <= 0)){
		printf("WARNING: PLAYER AI UCT: UCT_Expand(): node children list allocated but number of explored children is %d ... ALLOCATING LIST (causing memory leak)\n",parent->number_explored_children);
		UCT_Tree_Node_AllocateList(parent, simulatedGame->current_number_moves[simulatedGame->current_player]);
	}
#endif

	//allocate memory resources when exploring first action from node
	if(parent->children == NULL)
		UCT_Tree_Node_AllocateList(parent, simulatedGame->current_number_moves[simulatedGame->current_player]);
	
	//printf("DEBUG: parent p %d %d\n", parent, parent->allowedActionNum);
	//for(int i = 0; i < parent->allowedActionNum; i++)
	//	printf("DEBUG: child p %d %d\n", parent->children[i], i);

	//choose random untried action

#if((!TOMPLAYER_AI_UCT_TOMTEST_DEBUG_DISABLE_RANDOM)&&(!TOMPLAYER_AI_UCT_TOMTEST_DEBUG_DISABLE_RANDOM_EXPAND))
	randAction = (int)((rand()/(float)(RAND_MAX+1))*(parent->number_allowed_actions - parent->number_explored_children));
#else
	randAction = 0;
#endif

	//find nontried action, cycle through all actions
	cntAvailable = 0;
	selectedChild = NULL;
	for(int i = 0; i < all_actions_num; i++){

		//check available actions
		if(simulatedGame->current_moves[simulatedGame->current_player][i]){
			
			//check if action was already tried / already exists in the tree
			if((parent->children[cntAvailable]) == NULL){
				if(randAction <= 0){

					//allocate and init new children
					parent->children[cntAvailable] = UCT_Tree_Node_Init(i, parent);

					//mark as selected child/action
					selectedChild = parent->children[cntAvailable];

					//increase number of explored children actions/nodes
					parent->number_explored_children++;

					//break the loop
					i = all_actions_num;
				}else{
					randAction--;
				}
			}
			//END - check if action was already tried
			cntAvailable++;
			
		}
		//END - check available actions
	}

	//return selected untried action
	return selectedChild;
}

/**
Run simulations from the UCT-tree leaf by using the Default Policy

Updates MCTS_num_all_simulations counter, however not online but in BATCH at the end of all requested simulations
*/
void Player_AI_UCT_TomTest::UCT_Simulate(int num_simulations, const bool calc_variance){
	
	int plys_at_tree_leaf;

	//-- store values before playout phase begins --//
	plys_at_tree_leaf = simulatedGame->current_plys;	//remember number of moves at beginning of playout
	if(num_simulations > 1){
		simulatedGame_startingState->Copy_Game_State_From(simulatedGame,false);		//save game state at beginning of playout phase
	}

	//-- compute first simulation and calculate score --//
	UCT_Default_Policy();		//score is stored in simulatedGame->score

	//-- save feedback/results from first simulation --//

	//feedback: scores for each player
	for(int i = 0; i < simulatedGame->number_players; i++){

		sim_feedback_scores_avg[i] = simulatedGame->score[i];
		if(calc_variance)
			sim_feedback_scores[0][i] = sim_feedback_scores_avg[i];
	}

	//feedback: duration (number of plys) of the playout phase
	sim_feedback_plys_avg = (simulatedGame->current_plys) - plys_at_tree_leaf;
	if(calc_variance)
		sim_feedback_plys[0] = sim_feedback_plys_avg;

	//-- compute the rest of the simulations --//
	if(num_simulations == 1){
		for(MCTS_current_simuNum = 1; MCTS_current_simuNum < num_simulations; MCTS_current_simuNum++){

			//reset simulated game state to match state at beginning of playout phase
			simulatedGame->Copy_Game_State_From(simulatedGame_startingState,false);

			//compute simulation
			UCT_Default_Policy();

			//save feedback/results
			for(int i = 0; i < simulatedGame->number_players; i++){
				sim_feedback_scores_avg[i] += simulatedGame->score[i];
				if(calc_variance)
					sim_feedback_scores[MCTS_current_simuNum][i] = simulatedGame->score[i];
			}
			sim_feedback_plys_avg += ((simulatedGame->current_plys) - plys_at_tree_leaf);
			sim_feedback_plys[MCTS_current_simuNum] = (simulatedGame->current_plys) - (simulatedGame_startingState->current_plys);

		}

		//average results
		for(int i = 0; i < simulatedGame->number_players; i++){
			sim_feedback_scores_avg[i] /= num_simulations;
		}
		sim_feedback_plys_avg /= num_simulations;

		//calculate variance
		//TODO
	}

	//store number of computed simulations
	sim_feedback_num_sims = num_simulations;

	//BATCH update global simulation counter
	MCTS_num_all_simulations += num_simulations;
}

/**
Play default policy from current game position untill game end
*/
void Player_AI_UCT_TomTest::UCT_Default_Policy(){
	
	int gameStatus, lastMove;

	//simulate game until game-end
	gameStatus = (int)(simulatedGame->game_ended);
	while((gameStatus == 0)&&( (simulatedGame->current_plys - internalGame->current_plys) < UCT_param_defaultPolicy_maxPlys)){
		
		//DEBUG
		//if(simulatedGame->number_of_plays == 38) 
		//	printf("DEBUG: plys == %d", simulatedGame->number_of_plays);	

		//random policy
#if(!TOMPLAYER_AI_UCT_TOMTEST_DEBUG_DISABLE_RANDOM)
		lastMove = simulatedGame->Select_Move_Random();
#else
		lastMove = simulatedGame->Select_Move(0);
		//lastMove = simulatedGame->Select_Move_Random();
#endif

		//play move in internal simulated game
#if(TOM_DEBUG)
		gameStatus = simulatedGame->Play_Move(lastMove);
#else
		gameStatus = simulatedGame->Play_Move_Unsafe(lastMove);
#endif
		
		//DEBUG
#if((TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_LEVEL_UCT)&&TOMPLAYER_AI_UCT_TOMTEST_VISUALIZE_UCT_ACTIONS_TREE)
		//sprintf(visualizeActionsTree, "  > %2d", currentNode->action_index);
		visualizeActionsPlayout << "  > " << lastMove;
		//printf("  > %2d", currentNode->action_index);
#endif
#if(TOMPLAYER_AI_UCT_TOMTEST_DEBUG_SIMULATED_GAMES_OUT)
		simulatedGame->Output_Board_State();
		//simulatedGame->output_state_to_STD();
		//simulatedGame->output_chains_to_STD();
		//printf("Moves: %d %d\n",simulatedGame->num_moves[0], simulatedGame->num_moves[1]);
#endif
	}

	//calculate score at game end
	simulatedGame->Calculate_Score();
}

/**
Single- or two-player backup reward through the tree
*/
void Player_AI_UCT_TomTest::UCT_Backup(UCTnode* leaf){

	UCTnode* currentNode = leaf;
	int belonging_player;
	double mov_avg_alpha;
	double tree_up_path_length;

	//select correct players' reward
	//if(UCTtree_treePolicy_lastDepth % 2 == 0)
	//	if(internalGame->current_plys % 2 == 0)
	//		belonging_player = 1;
	//	else
	//		belonging_player = 0;
	//else
	//	if(internalGame->current_plys % 2 == 0)
	//		belonging_player = 0;
	//	else
	//		belonging_player = 1;

	//TODO IMPROVE: NOW TEMPORARY SOLUTION FOR SINGLEPLAYER and TWOPLAYER games
	if(game->number_players > 1)
		//select correct players' reward: bitwise replacement for upper if-sentences (instead of modulo 2)
		belonging_player = (UCTtree_treePolicy_lastDepth & 1) == (internalGame->current_plys & 1);
	else
		belonging_player = 0;
	
	//propagate reward up through the tree
	//while(currentNode != NULL){	//this was the old implementation (new one below should be better for optimization)
	tree_up_path_length = 0;
	for(int i = 0; i < UCTtree_treePolicy_lastDepth+1; i++){

		//-- update current node --//
		//basic UCT values
		currentNode->visits += 1.0;
		currentNode->rewards += sim_feedback_scores_avg[belonging_player];

		//count node visits since last UCT() call (since last external game ply and UCT tree change)
		if(currentNode->last_update_iteration < MCTS_num_all_iterations - MCTS_current_iterNum)
			currentNode->visits_in_last_UCT = 1.0;
		else
			currentNode->visits_in_last_UCT += 1.0;

		//new indicators
		currentNode->last_update_iteration = MCTS_num_all_iterations;
		currentNode->num_simulations += sim_feedback_num_sims;			//IS NOT AVERAGED OVER NUMBER OF VISITS!
		currentNode->num_plys_sim_phase += sim_feedback_plys_avg;		//IS NOT AVERAGED OVER NUMBER OF VISITS!
		currentNode->num_plys_to_end += (sim_feedback_plys_avg + tree_up_path_length);	//IS NOT AVERAGED OVER NUMBER OF VISITS!


		//moving average
		//if(currentNode->visits <= 1.0){		//not needed if (mov_avg_alpha = 1.0 / currentNode->visits)
		//	currentNode->moving_average_value = sim_feedback_scores_avg[belonging_player];
			//currentNode->moving_average_num_plys_sim_phase = sim_feedback_plys_avg;
			//currentNode->moving_average_num_plys_to_end = (sim_feedback_plys_avg + tree_up_path_length);
		//}else{
			//set alpha value - different options (TODO: test all of them)
			mov_avg_alpha = 1.0 / currentNode->visits;	//test: this should be the same as normal average (not moving)
			//mov_avg_alpha = UCT_param_mov_avg_alpha;	//option 1
			//mov_avg_alpha = UCT_param_mov_avg_alpha * 1.0 / currentNode->visits;	//option 2
			//mov_avg_alpha = UCT_param_mov_avg_alpha * current_episode / currentNode->visits;							//original equation from on-policy monte carlo example improvement - not applicable here because episodes do not exist
			//mov_avg_alpha = UCT_param_mov_avg_alpha * (MCTS_num_all_iterations+1.0) / currentNode->visits;			//option 3: original equation translated to entire game duration
			//mov_avg_alpha = UCT_param_mov_avg_alpha * (MCTS_current_iterNum+1.0) / currentNode->visits_in_last_UCT;	//option 4.1: original equation translated to single move UCT evaluation
			//mov_avg_alpha = UCT_param_mov_avg_alpha * (MCTS_current_iterNum+1.0) / (currentNode->visits / ((double)UCT_num_plys+1.0));	//option 4.2: original equation translated to single move UCT evaluation, with visits averaged over entire game duration (to normalize upper and bottom part of fraction in equation)
			
			//limit mov_avg_alpha value
			//if(mov_avg_alpha > 1.0) mov_avg_alpha = 1.0;			//option1: threshold (drawback: non-continuous)
			//mov_avg_alpha = 1.0 / (1.0 + exp(-mov_avg_alpha));	//option2: sigmoid (drawback: computationally heavy)

			//update moving average according to alpha value
			currentNode->moving_average_value = (1.0-mov_avg_alpha) * (currentNode->moving_average_value) + (mov_avg_alpha * sim_feedback_scores_avg[belonging_player]);
			currentNode->moving_average_num_plys_sim_phase = (1.0-mov_avg_alpha) * (currentNode->moving_average_num_plys_sim_phase) + (mov_avg_alpha * sim_feedback_plys_avg);
			currentNode->moving_average_num_plys_to_end = (1.0-mov_avg_alpha) * (currentNode->moving_average_num_plys_to_end) + (mov_avg_alpha * (sim_feedback_plys_avg + tree_up_path_length));
		//}
		
		//DEBUG
		//if(abs(currentNode->rewards / currentNode->visits - currentNode->moving_average_value) > 0.001)
		//	printf("%3.3f %3.3f\n",currentNode->rewards / currentNode->visits, currentNode->moving_average_value);

		//-- move to parent --/
		currentNode = currentNode->parent;
		tree_up_path_length += 1.0;

		//-- change active player (to get correct reward) --//
		belonging_player = internalGame->Get_Previous_Player(belonging_player);
	}
}

/**
UCT Tree Node - initialization
*/
Player_AI_UCT_TomTest::UCTnode* Player_AI_UCT_TomTest::UCT_Tree_Node_Init(int action, UCTnode* parent)
{
	UCTnode* node;

	//allocate and init
	node = new UCTnode;
	node->action_index = action;
	node->parent = parent;
	node->children = NULL;


	//reset
	UCT_Tree_Node_Reset(node);

	//increase counter of nodes and memory consumption (needed for allocation)
	UCTtree_num_nodes++;					//this is for debug only
	UCTtree_memorySize += sizeof(UCTnode);

	return node;
}

/**
UCT Tree Node - reset values
*/
void Player_AI_UCT_TomTest::UCT_Tree_Node_Reset(UCTnode* node)
{
	node->visits = 0.0;
	node->rewards = 0.0;
	node->number_explored_children = 0;

	node->last_update_iteration = 0;
	node->num_simulations = 0;
	node->num_plys_sim_phase = 0.0;
	node->num_plys_to_end = 0.0;

	node->computation_time = 0.0;
	node->knowledge_gain = 0.0;

	node->moving_average_value = 0.0;
	node->moving_average_num_plys_sim_phase = 0.0;
	node->moving_average_num_plys_sim_phase = 0.0;

	node->visits_in_last_UCT = 0.0;
}

/**
UCT Tree Node - allocate children (actions) list
*/
void Player_AI_UCT_TomTest::UCT_Tree_Node_AllocateList(UCTnode* node, int list_length)
{

	//TODO: DEBUG_ID001 why this is not equal? see other DEBUG_ID001
	//if((node->number_allowed_actions > 0)&&(list_length != node->number_allowed_actions))
	//	printf("%d %d\n",node->number_allowed_actions, list_length);

	//remember list length
	node->number_allowed_actions = list_length;

	//allocate and init list
	node->children = new UCTnode*[list_length];
	for(int i = 0; i < list_length; i++)
		node->children[i] = NULL;

	//increase memory consumption counter
	UCTtree_memorySize += list_length*sizeof(UCTnode*);
}

void Player_AI_UCT_TomTest::UCT_Tree_Change_Root(int number_actions)
{
	//is root change needed?
	if(number_actions > 0){

		//trim tree, delete unrelevant part of tree
		if(!UCTtree_preserve){

			UCTtree_newRoot = NULL;
			UCT_Tree_Trim(UCTroot, number_actions);

			//new root has been found
			if(UCTtree_newRoot != NULL){

				//delete old root and set new root
				if(UCTroot != UCTtree_newRoot){		//redundant check, but may be needed in some special cases
					delete(UCTroot);
					UCTroot = UCTtree_newRoot;
					UCTroot->parent = NULL;
				}

				//these two mutualy exclude:
				//decrease tree memory size counter for old UCTroot as recursive call did not decrease it
				//UCTtree_num_nodes--;
				//UCTtree_memorySize -= sizeof(UCTnode);
				//increase tree memory size counter (because in recursion we assumed that no root was found)				
				//UCTtree_num_nodes++;
				//UCTtree_memorySize += sizeof(UCTnode);

			//new root not found, selected actions were not present in tree
			}else{

				//decrease tree memory size counter for old UCTroot as recursive call did not decrease it
				//UCTtree_num_nodes--;
				//UCTtree_memorySize -= sizeof(UCTnode);

				//reset root node
				UCTroot->action_index = internalGame->history_moves[internalGame->current_plys];
				UCTroot->children = NULL;
				UCT_Tree_Node_Reset(UCTroot);

				//DEBUG
#if(TOMPLAYER_AI_UCT_TOMTEST_DEBUG_MEMORY_SIZE_COUNT)
				if((UCTtree_num_nodes != 1)||(UCTtree_memorySize != sizeof(UCTnode)))
					printf("PLAYER AI UCT: Tree_Change_Root(): tree error - tree incorrectly deleted, remaining nodes %d\n",UCTtree_num_nodes);
#endif

			}

		//preserve complete tree structure in memory, only move root pointer to different node
		}else{

			UCT_Tree_Preserve();

		}

	}

}


/**
Select new tree root for current game state and trim tree if UCTtree_preserve is not set: delete all nodes (and free memory) except active branch
WARNING: does not delete first call branchRoot node
*/
void Player_AI_UCT_TomTest::UCT_Tree_Trim(UCTnode* branchRoot, int remaining_depth)
{

	//new root has not yet been found
	if(remaining_depth > 0){

		//search deeper in the tree if branchRoot is not a leaf - if has children
		if(branchRoot->children != NULL){

			//decrease tree memory size counter (must be done before loop, because number_explored_children is decreased to 0)
			UCTtree_num_nodes -= ( branchRoot->number_explored_children );
			UCTtree_memorySize -= ( (branchRoot->number_allowed_actions*sizeof(UCTnode*) + branchRoot->number_explored_children*sizeof(UCTnode)) );

			//loop through all children
			for(int i = 0; i < branchRoot->number_allowed_actions, branchRoot->number_explored_children > 0; i++){

				//children is valid
				if(branchRoot->children[i] != NULL){

					//selected action found in tree
					if(branchRoot->children[i]->action_index == internalGame->history_moves[internalGame->current_plys-remaining_depth+1]){

						UCT_Tree_Trim(branchRoot->children[i],remaining_depth-1);

						//delete child if it is not new found root
						if(remaining_depth-1 > 0)
							delete(branchRoot->children[i]);

					//delete non-selected tree branch
					}else{

						UCT_Tree_Delete(branchRoot->children[i]);

					}

					//decrease counter of children
					branchRoot->number_explored_children--;

					//delete child
					//delete(branchRoot->children[i]);
				}
			}

			//delete root node list of children
			delete(branchRoot->children);

		}

	//new root has been found
	}else{		

		UCTtree_newRoot = branchRoot;

	}

}


/**
Recursively delete tree from selected node, including root node
WARNING: does not delete first call branchRoot node
*/
void Player_AI_UCT_TomTest::UCT_Tree_Delete(UCTnode* branchRoot)
{

	//search deeper in the tree if branchRoot is not a leaf - if has children
	if(branchRoot->children != NULL){

		//decrease tree memory size counter (must be done before loop, because number_explored_children is decreased to 0)
		UCTtree_num_nodes -= ( branchRoot->number_explored_children );
		UCTtree_memorySize -= ( (branchRoot->number_allowed_actions*sizeof(UCTnode*) + (branchRoot->number_explored_children)*sizeof(UCTnode)) );	//size decreased by number of children nodes and by size of children list of root node

		//loop through all children
		for(int i = 0; i < branchRoot->number_allowed_actions, branchRoot->number_explored_children > 0; i++){
			if(branchRoot->children[i] != NULL){
				UCT_Tree_Delete(branchRoot->children[i]);
				branchRoot->number_explored_children--;
			}
		}

		//delete root node list of children
		delete(branchRoot->children);

	}

	//delete self
	delete(branchRoot);

}

/**
Recursively delete selected tree branch, without root node
*/
void Player_AI_UCT_TomTest::UCT_Tree_Reset()
{

	UCTnode* branchRoot;

	//from which node to reset the tree
	if(!UCTtree_preserve)
		branchRoot = UCTroot;
	else
		branchRoot = UCTroot_memory;

	//node has children
	if(branchRoot->children != NULL){

		//find and delete children nodes
		for(int i = 0; i < branchRoot->number_allowed_actions, branchRoot->number_explored_children > 0; i++){
			if(branchRoot->children[i] != NULL){
				UCT_Tree_Delete(branchRoot->children[i]);
				branchRoot->number_explored_children--;
			}
		}

		//delete root node list of children
		delete(branchRoot->children);
		branchRoot->children = NULL;
	}

	//reset tree memory size counter
	UCTtree_num_nodes = 1;
	UCTtree_memorySize = sizeof(UCTnode);

	branchRoot->action_index = game->history_moves[game->current_plys];
	UCT_Tree_Node_Reset(branchRoot);
}

//TODO
void Player_AI_UCT_TomTest::UCT_Tree_Preserve()
{
}
//OLD CODE
////enable tree preservation
//void PlayerGoAI_UCT::UCTperserveTree_enable(){
//	memoryUCTroot = root;
//	UCTtree_preserve = true;
//}
//
////disable tree preservation - INCOMPLETE, TODO
//void PlayerGoAI_UCT::UCTperserveTree_disable(){
//	//delete tree except active root (for current game)
//	UCTtree_preserve = false;
//	trimUCTtree(memoryUCTroot,root->actionNum);
//}
//

/**
Output entire tree with all information (see implementation code for details about output format)
*/
void Player_AI_UCT_TomTest::Output_UCT_Tree()
{
	printf("\n========== BEGIN OUTPUT_UCT_TREE ==========\n\nTREE SIZE: %d nodes, memory: %d B (%.3f MB)\n\n", UCTtree_num_nodes, UCTtree_memorySize, UCTtree_memorySize/1024.0/1024.0);
	Output_UCT_Tree_Branch(UCTroot);
	printf("\n========== END OUTPUT_UCT_TREE ==========\n\n");
}

void Player_AI_UCT_TomTest::Output_UCT_Tree_Node_Children(UCTnode* parent)
{
	printf("\n========== BEGIN OUTPUT_UCT_TREE_NODE_CHILDREN ==========\naction: visits rewards  \tvalue:\n");
	for(int i = 0; i < UCTroot->number_allowed_actions; i++){
		if(UCTroot->children[i] != NULL){
			printf("   %3d:  %5.0f  %6.1f  \t%3.3f\n",
				UCTroot->children[i]->action_index, 
				UCTroot->children[i]->visits, 
				UCTroot->children[i]->rewards, 
				UCTroot->children[i]->value
			);
		}else{
			printf("Untried action: parent->children[%d]\n", i);
		}
	}
	printf("\n========== END OUTPUT_UCT_TREE_NODE_CHILDREN ==========\n\n");
}

/**
Output part of tree starting from branchRoot
*/
void Player_AI_UCT_TomTest::Output_UCT_Tree_Branch(UCTnode* branchRoot)
{
	UCTnode* tmpNode;
	
	//print current node and its parents
	printf("%d",branchRoot->action_index);
	tmpNode = branchRoot->parent;
	while(tmpNode != NULL){
		printf(" < %d",tmpNode->action_index);
		tmpNode = tmpNode ->parent;
	}

	//print current nodes' values
	printf("\t :\t %2.1f %2.1f",branchRoot->visits,branchRoot->rewards);

	//search deeper in the tree if branchRoot is not a leaf - if has children
	if(branchRoot->children != NULL){

		//print children info
		printf("\t %d/%d :",branchRoot->number_explored_children,branchRoot->number_allowed_actions);

		//print current nodes' children list
		for(int i = 0, c = 0; i < branchRoot->number_allowed_actions, c < branchRoot->number_explored_children; i++){
			if(branchRoot->children[i] != NULL){
				printf(" %d",branchRoot->children[i]->action_index);
				c++;
			}
		}

		printf("\n");

		//recursively call children nodes' output
		for(int i = 0, c = 0; i < branchRoot->number_allowed_actions, c < branchRoot->number_explored_children; i++){
			if(branchRoot->children[i] != NULL){
				Output_UCT_Tree_Branch(branchRoot->children[i]);
				c++;
			}
		}

	}else{
		printf("\n");
	}

}

void Player_AI_UCT_TomTest::Debug_UCT_Tree_MemoryState()
{

	int size1, size2;

	printf("\nTREE MEMORY STATE\n");
	printf("Current tree: ");
	size1 = Debug_UCT_Tree_Size(UCTroot);
	printf("\n");
	printf("Memory tree:  ");
	if(this->UCTtree_preserve)
		size2 = Debug_UCT_Tree_Size(UCTroot_memory);
	else
		size2 = -1;
	printf("\n");
	printf("Tree info: num nodes: %d size %d\n", UCTtree_num_nodes, UCTtree_memorySize);
	printf("Size: current: %d memory: %d\n\n", size1, size2);
}

//TODO

int Player_AI_UCT_TomTest::Debug_UCT_Tree_Size(UCTnode*)
{
	return 0;
}


//
///**
//return number of nodes in tree from branchRoot
//*/
//int PlayerGoAI_UCT::STDprintNumChildren(UCTnode* branchRoot){
//
//	int numNodes = 1;
//
//	if(branchRoot->explored_children > 0){
//
//		printf("%d[",branchRoot->explored_children);
//
//		for(int i = 0; i < branchRoot->allowedActionNum; i++){
//			if(branchRoot->children[i] != NULL){
//				numNodes += STDprintNumChildren(branchRoot->children[i]);
//			}
//		}
//
//		printf("]");
//
//	}else{
//
//		printf(".");
//
//	}
//
//	return numNodes;
//
//}
//
///**
//return number of nodes in tree from branchRoot
//*/
//int PlayerGoAI_UCT::STDprintTreeVal(UCTnode* branchRoot){
//
//	printf("%3.0f[",(branchRoot->rewards/branchRoot->visits)*100);
//
//	if(branchRoot->explored_children > 0){
//
//		for(int i = 0; i < branchRoot->allowedActionNum; i++){
//			if(branchRoot->children[i] != NULL){
//				STDprintTreeVal(branchRoot->children[i]);
//			}
//		}
//	}
//
//	printf("]");
//
//	return 0;
//
//}