//include header
#include "Player_AI_Simple.hpp"

/**
Constructor for AI player module calling general game player constructor

@param game Pointer to a Game_Engine object (or derivated class)
*/
Player_AI_Simple::Player_AI_Simple(Game_Engine* game, int new_player_number) : Player_Engine(game, new_player_number)
{
	//player definition
	player_name = "SimpAI";

	//call initialization procedures
	if(game != NULL)
		Initialize();
	else
		is_initialized = false;
}


//destructor
Player_AI_Simple::~Player_AI_Simple(void)
{
	Clear_Memory();
}

void Player_AI_Simple::Init_Settings()
{
	//init player settings
	output_type =		TOMPlayer_AI_Simple_OUTPUT_TYPE_2D;

	//init internal variables
	all_actions_num =	game->maximum_allowed_moves;

	//init learning settings
	explorationFactor =	(int)(TOMPlayer_AI_Simple_PARAM_EXPLORE_FACTOR*(float)(RAND_MAX+1));
	selectMoveType =	TOMPlayer_AI_Simple_PARAM_MOVE_TYPE;
}

void Player_AI_Simple::Allocate_Memory()
{
	//allocate resources
	actionsWeight = new double	[all_actions_num];
	actionsNumWin = new int		[all_actions_num];
	actionsNumSel = new int		[all_actions_num];
	actionsPlayed = new bool	[all_actions_num];
}

void Player_AI_Simple::Clear_Memory()
{
	//clean up memory
	if(is_initialized){
		delete(actionsWeight);
		delete(actionsNumWin);
		delete(actionsNumSel);
		delete(actionsPlayed);
	}
}

/**
Reset to initial values
*/
void Player_AI_Simple::Reset()
{
	for(int i = 0; i < all_actions_num; i++){
		actionsWeight[i] = 1.0;		//exploring starts
		actionsNumWin[i] = 0;
		actionsNumSel[i] = 0;
	}

	for(int i = 0; i < all_actions_num; i++)
		actionsPlayed[i] = 0;
}

int Player_AI_Simple::Get_Move(){
	if(selectMoveType == TOMPlayer_AI_Simple_MOVE_TYPE_EXPLORE)
		return Play_Move();
	else if(selectMoveType == TOMPlayer_AI_Simple_MOVE_TYPE_BEST)
		return Play_Move_Best();
	else
		return -1;
}

int Player_AI_Simple::Play_Move(){

	int selected;
	double sumWeights, randThres;

	//choose random action
	if(rand() < explorationFactor){
		selected = game->Select_Move_Random();		//(rand()/RAND_MAX+1)* (currentGame->num_moves[currentGame->current_player]);

	//choose weighted action
	}else{

		sumWeights = 0.0;
		for(int i = 0; i < all_actions_num; i++){
			if(game->current_moves[game->current_player][i] == true){
				sumWeights += actionsWeight[i];
			}
		}

		randThres = rand()/(float)(RAND_MAX+1) * sumWeights;

		//if all actions are bad select a random one
		if(sumWeights == 0.0){
			selected = game->Select_Move_Random();		//(rand()/RAND_MAX+1)* (currentGame->num_moves[currentGame->current_player]);
		}else{
			
			sumWeights = 0.0;
			selected = -1;
			for(int i = 0; i < all_actions_num; i++){
				if(game->current_moves[game->current_player][i] == true){
					sumWeights += actionsWeight[i];
					if(sumWeights >= randThres){
						selected = i;
						i = all_actions_num;
					}
				}
			}
			
		}
		
	}

	actionsPlayed[selected] = 1;
	return selected;
}

int Player_AI_Simple::Play_Move_Best(){

	int selected, multiple_best, randThres;
	double bestWeight;

	//choose random action
	if(rand() < explorationFactor){
		selected = game->Select_Move_Random();		//(rand()/RAND_MAX+1)* (currentGame->num_moves[currentGame->current_player]);
	}else{

		//find best action
		multiple_best = 1;
		bestWeight = actionsWeight[0];
		selected = 0;
		for(int i = 1; i < all_actions_num; i++){
			if(game->current_moves[game->current_player][i] == 1){
				if(actionsWeight[i] > bestWeight){
					bestWeight = actionsWeight[i];
					selected = i;
					multiple_best = 1;
				}else if(bestWeight == actionsWeight[i]){
					multiple_best++;
				}
			}
		}

		//if multiple best actions, choose one at random
		if(multiple_best > 1){
			randThres = (int)(rand()/(float)(RAND_MAX+1) * multiple_best);
			if(actionsWeight[0] == bestWeight){
				if(randThres == 0)
					selected = 0;
				else
					randThres--;
			}

			for(int i = 1; i < all_actions_num; i++){
				if(game->current_moves[game->current_player][i] == 1){
					if(actionsWeight[i] == bestWeight){
						if(randThres <= 0){
							selected = i;
							i = all_actions_num;
						}else{
							randThres--;
						}
					}
				}
			}	
		}
	}

	//flag action as played
	actionsPlayed[selected] = 1;
	return selected;
}

void Player_AI_Simple::End_Game(){
	
	if(game->score[player_number] > 0.0){
		for(int i = 0; i < all_actions_num; i++){
			if(actionsPlayed[i] == 1){
				actionsNumWin[i]++;
				actionsNumSel[i]++;
				actionsWeight[i] = (double)(actionsNumWin[i])/actionsNumSel[i];
				actionsPlayed[i] = 0;
			}
		}
	}else if(game->score[player_number] <= 0.0){
		for(int i = 0; i < all_actions_num; i++){
			if(actionsPlayed[i] == 1){
				actionsNumSel[i]++;
				actionsWeight[i] = (double)(actionsNumWin[i])/actionsNumSel[i];
				actionsPlayed[i] = 0;
			}
		}
	}

}

void Player_AI_Simple::Output(){

	if(output_type == TOMPlayer_AI_Simple_OUTPUT_TYPE_1D){

		printf("OUTPUT AI BASIC, player %d:",player_number);
		for(int i = 0; i < all_actions_num; i++)
			printf(" %3.3f",actionsWeight[i]);
		printf("\n");

	}else if(output_type == TOMPlayer_AI_Simple_OUTPUT_TYPE_2D){

		printf("OUTPUT AI BASIC, player %d\n",player_number);
		for(int i = 0; i < game->board_length; i++){
			for(int j = 0; j < game->board_length; j++){
				printf("%3.3f ",actionsWeight[i*game->board_length+j]);
			}
			printf("\n");
		}
		printf("\n");

	}else if(output_type == TOMPlayer_AI_Simple_OUTPUT_TYPE_GO){

		printf("\nOUTPUT AI BASIC, player %d\nPASS: %3.3f\n",player_number,actionsWeight[0]);
		for(int i = 0; i < game->board_length; i++){
			for(int j = 0; j < game->board_length; j++){
				printf("%3.3f ",actionsWeight[i*game->board_length+j+1]);
			}
			printf("\n");
		}
		printf("\n");

	}

}