//include header
#include "Player_AI_UCT_Reinforce.hpp"

/**
Constructor for "AI UCT Reinforce" player module calling AI UCT player

@param game Pointer to a Game_Engine object (or derivated class)
*/
Player_AI_UCT_Reinforce::Player_AI_UCT_Reinforce(Game_Engine* game, int player_number) : Player_AI_UCT(game, player_number)
{
	//player definition
	player_name = "UCT_reinforce";

	//call initialization procedures
	if(game != NULL)
		Initialize();
	else
		is_initialized = false;
}

//destructor
Player_AI_UCT_Reinforce::~Player_AI_UCT_Reinforce(void)
{
	Clear_Memory();
}

void Player_AI_UCT_Reinforce::Allocate_Memory()
{
	//allocate resources
	UCTroot = UCT_Tree_Node_Init(-1, NULL);
	UCTroot_memory = UCTroot;
	internalGame = game->Create_Duplicate_Game();
	simulatedGame = game->Create_Duplicate_Game();

	//allocate rewards array
	online_rewards = new double[UCT_param_defaultPolicy_maxPlys];

}

void Player_AI_UCT_Reinforce::Clear_Memory()
{
	//clean up memory
	if(is_initialized){
		if(UCTtree_preserve)
			UCT_Tree_Delete(UCTroot_memory);
		else
			UCT_Tree_Delete(UCTroot);

		delete(internalGame);
		delete(simulatedGame);

		//rewards array
		delete(online_rewards);
	}
}

int Player_AI_UCT_Reinforce::UCT()
{

	UCTnode* selected_leaf;
	//double* final_rewards;

	//execute simulations with given computational budget
	for(int i = 0; i < UCT_param_IterNum; i++){

		//RESET simulated game position to initial state
		simulatedGame->Copy_Game_State_From(internalGame,false);

		//reset number of moves from root (also used as index for rewards array)
		UCTtree_treePolicy_lastDepth = 0;

		//DEBUG
		//internalGame->output_chains_to_STD();
		//simulatedGame->output_chains_to_STD();
		//simulatedGame->output_state_to_STD();
		//printf("sim player %d\n",simulatedGame->current_player);

		//UCT algorithm
		selected_leaf = UCT_Tree_Policy(UCTroot);
		UCT_Default_Policy();
		UCT_Backup(selected_leaf, NULL);
		//final_rewards = UCT_Default_Policy();
		//UCT_Backup(selected_leaf, final_rewards);

		//UCTbackup(UCTtreePolicy(root), UCTdefaultPolicy());	//compact version

		//check if memory limit was exceeded - TODO: this is a basic version that work well if only 1 node is added per iteration, otherwise the procedure must be implemented inside UCTtreePolicy, where new nodes are allocated
		if(UCTtree_memorySize >= UCTtree_maxMemorySize){
			i = UCT_param_IterNum;	//end the simulations loop
		}

		//DEBUG
#if(TOMPLAYER_AI_UCT_VISUALIZE_LEVEL_UCT)
		printf("\nSim num %2d     score",i);
		for(int t = 0; t < game->number_players; t++)
			printf("  %3.5f",internalGame->score[t]);
#if(!TOMPLAYER_AI_UCT_VISUALIZE_UCT_ACTIONS_TREE)
		printf("     root  %2d", UCTroot->action_index);
#else
		printf("     tree  %2d", UCTroot->action_index);
		printf("%s",visualizeActionsTree.str().c_str());
		visualizeActionsTree.str("");		//set stringstream to empty
		visualizeActionsTree.clear();		//clear flags
#endif
#if(TOMPLAYER_AI_UCT_VISUALIZE_UCT_ACTIONS_PLAYOUT)
		printf("    \t playout:");
		printf("%s",visualizeActionsPlayout.str().c_str());
		visualizeActionsPlayout.str("");	//set stringstream to empty
		visualizeActionsPlayout.clear();	//clear flags
#endif
#endif

#if(TOMPLAYER_AI_UCT_DEBUG_TREE_EXPANSION_SIM)
		Output_UCT_Tree();
#endif

	}

	//DEBUG
#if(TOMPlayer_AI_UCT_DEBUG_TREE_EXPANSION)
	Output_UCT_Tree();
#endif
	
	//return best action from root (without exploring factors) and remember node
	if(UCTroot->number_explored_children > 0){
		lastAction_node = UCT_Tree_Policy_Best_Child(UCTroot, 0.0);
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
}

Player_AI_UCT_Reinforce::UCTnode* Player_AI_UCT_Reinforce::UCT_Tree_Policy(UCTnode* root)
{

	UCTnode* currentNode = root;
	int terminalNode = (int)(simulatedGame->game_ended);

	//move through the tree and game
	while(terminalNode == 0){
		
		//check expanding condition and descend tree according to tree policy
		if(currentNode->number_explored_children >= simulatedGame->current_number_moves[simulatedGame->current_player]){
	
			//select best child node/action
			currentNode = UCT_Tree_Policy_Best_Child(currentNode, UCT_param_C);

			//play simulated game (if game not ended yet playMove() returns value 0)
			terminalNode = simulatedGame->Play_Move(currentNode->action_index);
		
			//DEBUG
#if(TOMPLAYER_AI_UCT_DEBUG_TRACE_BEST_UCT)
			printf("%d  %4.1f %3.0f  %3.0f  %3.3f\n", currentNode->action_index, currentNode->rewards, currentNode->parent->visits, currentNode->visits, currentNode->value);
#endif

		//expand if not fully expanded
		}else{

			//select a nontried action
			currentNode = UCT_Expand(currentNode);

			//play simulated game
			simulatedGame->Play_Move(currentNode->action_index);
			
			//is last node in tree
			terminalNode = 1;
		}

		//get immediate reward after last move
		simulatedGame->Calculate_Score();
		online_rewards[UCTtree_treePolicy_lastDepth] = simulatedGame->score[0];		//WARNING: TODO: CURRENTLY WORKS ONLY FOR SINGLE-PLAYER GAMES

		//increment distance from root
		UCTtree_treePolicy_lastDepth++;


#if(TOMPLAYER_AI_UCT_VISUALIZE_UCT_ACTIONS_TREE)
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
	}

	//save number of actions from root to selected leaf (search depth) - not needed anymore, increments in above loop
	//UCTtree_treePolicy_lastDepth = simulatedGame->current_plys - internalGame->current_plys;

	//return selected leaf
	return currentNode;

};

/**
Play default policy from current game position untill game end
*/
double* Player_AI_UCT_Reinforce::UCT_Default_Policy(){
	
	int gameStatus, lastMove;

	//reset reward sum in the playout phase (to collect all rewards)
	online_rewards[UCTtree_treePolicy_lastDepth] = 0.0;

	//simulate game until game-end
	gameStatus = (int)(simulatedGame->game_ended);
	while((gameStatus == 0)&&( (simulatedGame->current_plys - internalGame->current_plys) < UCT_param_defaultPolicy_maxPlys)){
		
		//DEBUG
		//if(simulatedGame->number_of_plays == 38) 
		//	printf("DEBUG: plys == %d", simulatedGame->number_of_plays);	

		//random policy
		#if(!TOMPLAYER_AI_UCT_DEBUG_DISABLE_RANDOM)
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

		//get immediate reward after last move - collect all rewards in the playout phase
		simulatedGame->Calculate_Score();
		online_rewards[UCTtree_treePolicy_lastDepth] += simulatedGame->score[0];		//WARNING: TODO: CURRENTLY WORKS ONLY FOR SINGLE-PLAYER GAMES
		
		//DEBUG
#if((TOMPLAYER_AI_UCT_VISUALIZE_LEVEL_UCT)&&TOMPlayer_AI_UCT_Reinforce_VISUALIZE_UCT_ACTIONS_TREE)
		//sprintf(visualizeActionsTree, "  > %2d", currentNode->action_index);
		visualizeActionsPlayout << "  > " << lastMove;
		//printf("  > %2d", currentNode->action_index);
#endif
#if(TOMPLAYER_AI_UCT_DEBUG_SIMULATED_GAMES_OUT)
		simulatedGame->Output_Board_State();
		//simulatedGame->output_state_to_STD();
		//simulatedGame->output_chains_to_STD();
		//printf("Moves: %d %d\n",simulatedGame->num_moves[0], simulatedGame->num_moves[1]);
#endif
	}

	//calculate score
	//simulatedGame->Calculate_Score();

	//return final score
	//return (simulatedGame->score);
	return NULL;

}

/**
Single-player backup reward through the tree
*/
void Player_AI_UCT_Reinforce::UCT_Backup(UCTnode* leaf, double* rewards){

	UCTnode* currentNode = leaf;
	double sumRewards;
	double lastReward;
	int numRewards;

	//int belonging_player;

	//TODO IMPROVE: NOW TEMPORARY SOLUTION FOR SINGLEPLAYER games
#if(TOM_DEBUG)
	if(game->number_players > 1)
		printf("PLAYER AI UCT Reinforce: UCT_Backup(): ERROR: non singleplayer games currently not supported!\n");
#endif

	//propagate reward up through the tree (do not update root yet)
	sumRewards = online_rewards[UCTtree_treePolicy_lastDepth];
	for(int reward_counter = UCTtree_treePolicy_lastDepth-1; reward_counter >= 0; reward_counter--){

		//update sum of online rewards
		sumRewards += online_rewards[reward_counter];

		//update node value, normalize by number of steps (number of rewards) in the simulation game
		numRewards = (simulatedGame->current_plys - internalGame->current_plys - reward_counter);
		lastReward = (sumRewards / numRewards);
		currentNode->rewards += lastReward;

		currentNode->visits += 1.0;
		currentNode = currentNode->parent;

	}
	//update root
	currentNode->rewards += lastReward;
	currentNode->visits += 1.0;

	//DEBUG
#if(TOM_DEBUG)
	if(currentNode != UCTroot)
		printf("PLAYER AI UCT Reinforce: UCT_Backup(): ERROR: backup count incorrect by more than 1\n");
#endif


}

// ------------------------------

/**
Constructor for "AI UCT Reinforce" player module calling AI UCT player

@param game Pointer to a Game_Engine object (or derivated class)
*/
Player_AI_UCT_Reinforce_DPrepeat::Player_AI_UCT_Reinforce_DPrepeat(Game_Engine* game, int player_number) : Player_AI_UCT_Reinforce(game, player_number)
{
	//player definition
	player_name = "UCT_reinforce_DPrepeat";

	//call initialization procedures
	if(game != NULL)
		Initialize();
	else
		is_initialized = false;
}

//destructor
Player_AI_UCT_Reinforce_DPrepeat::~Player_AI_UCT_Reinforce_DPrepeat(void)
{
	Clear_Memory();
}

/**
Modified default policy: repeats last move in tree
*/
double* Player_AI_UCT_Reinforce_DPrepeat::UCT_Default_Policy(){
	
	int gameStatus, lastMove;

	//reset reward sum in the playout phase (to collect all rewards)
	online_rewards[UCTtree_treePolicy_lastDepth] = 0.0;

	//last tree move policy
	lastMove = simulatedGame->history_moves[simulatedGame->current_plys];

	//simulate game until game-end
	gameStatus = (int)(simulatedGame->game_ended);
	while((gameStatus == 0)&&( (simulatedGame->current_plys - internalGame->current_plys) < UCT_param_defaultPolicy_maxPlys)){
		
		//DEBUG
		//if(simulatedGame->number_of_plays == 38) 
		//	printf("DEBUG: plys == %d", simulatedGame->number_of_plays);	

		//play move in internal simulated game
#if(TOM_DEBUG)
		gameStatus = simulatedGame->Play_Move(lastMove);
#else
		gameStatus = simulatedGame->Play_Move_Unsafe(lastMove);
#endif

		//get immediate reward after last move - collect all rewards in the playout phase
		simulatedGame->Calculate_Score();
		online_rewards[UCTtree_treePolicy_lastDepth] += simulatedGame->score[0];		//WARNING: TODO: CURRENTLY WORKS ONLY FOR SINGLE-PLAYER GAMES
		
		//DEBUG
#if((TOMPLAYER_AI_UCT_VISUALIZE_LEVEL_UCT)&&TOMPlayer_AI_UCT_Reinforce_VISUALIZE_UCT_ACTIONS_TREE)
		//sprintf(visualizeActionsTree, "  > %2d", currentNode->action_index);
		visualizeActionsPlayout << "  > " << lastMove;
		//printf("  > %2d", currentNode->action_index);
#endif
#if(TOMPLAYER_AI_UCT_DEBUG_SIMULATED_GAMES_OUT)
		simulatedGame->Output_Board_State();
		//simulatedGame->output_state_to_STD();
		//simulatedGame->output_chains_to_STD();
		//printf("Moves: %d %d\n",simulatedGame->num_moves[0], simulatedGame->num_moves[1]);
#endif
	}

	//calculate score
	//simulatedGame->Calculate_Score();

	//return final score
	//return (simulatedGame->score);
	return NULL;

}

// ------------------------------

/**
Constructor for "AI UCT Reinforce" player module calling AI UCT player

@param game Pointer to a Game_Engine object (or derivated class)
*/
Player_AI_UCT_Reinforce_DPpassive::Player_AI_UCT_Reinforce_DPpassive(Game_Engine* game, int player_number) : Player_AI_UCT_Reinforce(game, player_number)
{
	//player definition
	player_name = "UCT_reinforce_DPpassive";

	//call initialization procedures
	if(game != NULL)
		Initialize();
	else
		is_initialized = false;
}

//destructor
Player_AI_UCT_Reinforce_DPpassive::~Player_AI_UCT_Reinforce_DPpassive(void)
{
	Clear_Memory();
}

/**
Modified default policy: non-changing passive move policy
*/
double* Player_AI_UCT_Reinforce_DPpassive::UCT_Default_Policy(){
	
	int gameStatus, lastMove;

	//reset reward sum in the playout phase (to collect all rewards)
	online_rewards[UCTtree_treePolicy_lastDepth] = 0.0;

	//non-changing passive move policy
	lastMove = simulatedGame->Select_Move_Passive();

	//simulate game until game-end
	gameStatus = (int)(simulatedGame->game_ended);
	while((gameStatus == 0)&&( (simulatedGame->current_plys - internalGame->current_plys) < UCT_param_defaultPolicy_maxPlys)){
		
		//DEBUG
		//if(simulatedGame->number_of_plays == 38) 
		//	printf("DEBUG: plys == %d", simulatedGame->number_of_plays);	

		//play move in internal simulated game
#if(TOM_DEBUG)
		gameStatus = simulatedGame->Play_Move(lastMove);
#else
		gameStatus = simulatedGame->Play_Move_Unsafe(lastMove);
#endif

		//get immediate reward after last move - collect all rewards in the playout phase
		simulatedGame->Calculate_Score();
		online_rewards[UCTtree_treePolicy_lastDepth] += simulatedGame->score[0];		//WARNING: TODO: CURRENTLY WORKS ONLY FOR SINGLE-PLAYER GAMES
		
		//DEBUG
#if((TOMPLAYER_AI_UCT_VISUALIZE_LEVEL_UCT)&&TOMPlayer_AI_UCT_Reinforce_VISUALIZE_UCT_ACTIONS_TREE)
		//sprintf(visualizeActionsTree, "  > %2d", currentNode->action_index);
		visualizeActionsPlayout << "  > " << lastMove;
		//printf("  > %2d", currentNode->action_index);
#endif
#if(TOMPLAYER_AI_UCT_DEBUG_SIMULATED_GAMES_OUT)
		simulatedGame->Output_Board_State();
		//simulatedGame->output_state_to_STD();
		//simulatedGame->output_chains_to_STD();
		//printf("Moves: %d %d\n",simulatedGame->num_moves[0], simulatedGame->num_moves[1]);
#endif
	}

	//calculate score
	//simulatedGame->Calculate_Score();

	//return final score
	//return (simulatedGame->score);
	return NULL;

}