//include header
#include "GoGameEngine.hpp"

//MACRO defines
#define GOENG_NORT(pos) (pos-board_length)
#define GOENG_SOUT(pos) (pos+board_length)
#define GOENG_WEST(pos) (pos-1)
#define GOENG_EAST(pos) (pos+1)

#define GOENG_VALID_NORT(pos)	(pos > board_length-1)
#define GOENG_VALID_SOUT(pos)	(pos < board_size-board_length)
#define GOENG_VALID_WEST(pos)	(boardState[pos].column > 0)
#define GOENG_VALID_EAST(pos)	(boardState[pos].column < board_length-1)

//default constructor
//GoGameEngine::GoGameEngine()
//{
//	GoGameEngine::initialize(DEFAULT_GOENG_boardSize);
//}

//constructor with parameter for board length
GoGameEngine::GoGameEngine(int board_width, bool reset)
{
	GoGameEngine::initialize(board_width, reset);
}

//destructor
GoGameEngine::~GoGameEngine(void)
{
	//release memory space
	delete(boardState);
	delete(moves[0]);
	delete(moves[1]);
	delete(chains);
}

//allocate memory and initialize all values
void GoGameEngine::initialize(int board_width, bool reset)
{
	//set global values
	board_length = board_width;
	board_size = board_length*board_length;
	all_actions_num = board_size+1;

	//set parameters to default values
	score_pass_point = DEFAULT_GOENG_score_pass_point;
	disable_warnings = 0;
	komi = DEFAULT_GOENG_KOMI;

	//allocate resources
	boardState = new single_Intersection[board_size];
	moves[0] = new bool[all_actions_num];
	moves[1] = new bool[all_actions_num];
	chains = new single_Chain[board_size];

	//label rows
	for(int i = 0; i < board_length; i++)
		for(int j = 0; j < board_length; j++)
			boardState[i*board_length + j].column = j;

	//set all initial values
	if(reset)
		resetGame();
}

/**
Reset to initial values (restart game)
*/
void GoGameEngine::resetGame(){

	ended = false;
	current_player = 0;
	number_of_plays = 0;

	num_moves[0] = all_actions_num; //including PASS move
	num_moves[1] = all_actions_num; //including PASS move

	num_captured[0] = 0;
	num_captured[1] = 0;
	score = 0;
	score_pass = 0;

	previous_move = -1;
	last_score_compute = -1;

	//initialize values in arrays
	for(int i = 0; i < board_size; i++){
		boardState[i].stone = 0;
		boardState[i].belonging_chain = NULL;
		boardState[i].nextStone = NULL;
		boardState[i].flag = -1;
		moves[0][i] = 1;
		moves[1][i] = 1;
		chains[i].flag2 = 0;
		chains[i].firstStone = NULL;
		chains[i].lastStone = NULL;
	}
	moves[0][all_actions_num-1] = 1;
	moves[1][all_actions_num-1] = 1;
}

/**
Check validity and play selected move. If two consecutive passes are played, then game ends.

@param move_number Selected action expressed as position+1 of new stone to be placed in 1D array, value 0 or less represents the PASS action.
@return 0 if move was successfully executed; 1 if game ended because of consecutive PASSes; -1 if illegal move was played or an error occured.
*/
int GoGameEngine::playMove(int move_number){

	//check move
	//if(move_number < 0) printf("DEBUG: move<0 \n");	//DEBUG

	if(validateAction(move_number)){
		return playMoveUnsafe(move_number);	//play legal move

	//error: illegal move
	}else{
		if(disable_warnings == 0)
			printf("!!ERROR!! Go Game Engine: wrong move, player %d, move %d, number of plays %d\n",current_player+1,move_number,number_of_plays);
		return -1;
	}
}

/**
Returns if inputted action is currently available i.e. legal.

@return TRUE if action is valid, FALSE if action is invalid.
*/
bool GoGameEngine::validateAction(int number){

	//action PASS is always valid
	if(number == 0){
		return true;

	//check if action falls out of array
	}else if((number < 0) || (number > board_size)){
		return false;

	//check if intersection is available to play
	}else
		return moves[current_player][number];
}

/**
UNSAFE: Play selected move without checking validity. If two consecutive passes are played, then game ends.

@param move_number Selected action expressed as position+1 of new stone to be placed in 1D array, value 0 or less represents the PASS action.
@return 0 if move was successfully executed; 1 if game ended because of consecutive PASSes.
*/
int GoGameEngine::playMoveUnsafe(int move_number){

	//local variables
	int feedback = 0;	//function return variable
	int cf,ce;			//counters
	const int move_corrected = move_number-1;	//move_number decreased by 1 (corrected to align in 1D array)

	//check if move is PASS
	if(move_number == 0){

		//use APA scoring, give a point to the opponent at each PASS
		if(score_pass_point){
			if(current_player == 0)
				score_pass--;
			else
				score_pass++;
		}

		//check for two consecutive passes - end game
		if(previous_move == 0){
			feedback = 1;
			this->ended = true;
		}

		//remember previous move
		previous_move = 0;

	//otherwise place new stone
	}else{

		//init counters
		cf = 0;			//number of nearby friendly chains
		ce = 0;			//number of nearby enemy chains

		//reset number of liberties
		num_liberties = 0;		

		//check adjacent intersections
		if(GOENG_VALID_NORT(move_corrected)) adjacentCheck(GOENG_NORT(move_corrected),&cf,&ce);	//executes if not in top row
		if(GOENG_VALID_SOUT(move_corrected)) adjacentCheck(GOENG_SOUT(move_corrected),&cf,&ce);	//executes if not in bottom row
		if(GOENG_VALID_WEST(move_corrected)) adjacentCheck(GOENG_WEST(move_corrected),&cf,&ce);	//executes if not in left column
		if(GOENG_VALID_EAST(move_corrected)) adjacentCheck(GOENG_EAST(move_corrected),&cf,&ce);	//executes if not in right column

		//clear flags on enemy chains
		for(int i = 0; i < ce; i++)
			adjacentChain[1][i]->flag = 0;

		//add new stone
		boardState[move_corrected].stone = current_player+1;

		//MERGE new stone with nearby chains or create a new chain; and recalculate liberties
		mergeChains(move_corrected, cf);

		//check if suicide
		if(boardState[move_corrected].belonging_chain->liberties == 0){
			//own chain captured - available game actions are already updated in captureChain procedure
			captureChain(boardState[move_corrected].belonging_chain, 1-current_player);
		}else{
			//update available game actions (index in moves[] is by 1 higher than in boardState[])
			moves[0][move_number] = 0;
			moves[1][move_number] = 0;
		}

		//update available game actions count (must be executed also when suicide occurs - because of the captureChain() algorithm)
		num_moves[0]--;
		num_moves[1]--;

		//TODO-- prevent KO and cycles and suicide (maybe here i will need liberties by intesrections, for KO and suicide), later also for triple and multiple ko, eternal life

		//remember previous move
		previous_move = move_number;
	}

	//change global variables
	current_player = 1 - current_player;
	number_of_plays++;

	return feedback;
}


/**
Function for adjacency check for all four directions when adding a new stone
*/
void GoGameEngine::adjacentCheck(int adjacentPosition, int* counter_friendly, int* counter_enemy){
	
	single_Chain* current_chain;	//pointer to a currently analyzed chain

	if(boardState[adjacentPosition].stone != 0){
		current_chain = boardState[adjacentPosition].belonging_chain;	//pointer to belonging chain
		if(current_chain->flag == 0)									//was already visited?
			
			//-- friendly chain --
			if(boardState[adjacentPosition].stone - 1 == current_player){ 	
				adjacentChain[0][(*counter_friendly)] = current_chain;	//memorize in order to merge chains later
				current_chain->flag = 1;								//set to visited
				(*counter_friendly)++;									//increase counter of friendly chains nearby
			
			//-- enemy chain --	
			}else{								
				current_chain->liberties--;								//decrease liberties by 1
				if(current_chain->liberties == 0){						//if number of liberties is zero then capture
					captureChain(current_chain,current_player);
					num_liberties++;
				}else{													//otherwise:
					current_chain->flag = 1;							//set to visited
					adjacentChain[1][(*counter_enemy)] = current_chain;	//memorize in order to restore flag later
					(*counter_enemy)++;									
				}
			}
	}else{
		num_liberties++;	//increase number of liberties									
	}
}

/**
Captures and deletes from the board all stones belonging to a single chain.

Also resets the flags of that chain; increases the global counter of captured stones; and updates set of possible game actions.

@param chainPointer Pointer to chain to be captured.
@param capturingPlayer Which player is capturing the chain (value 0 or 1).
*/
void GoGameEngine::captureChain(single_Chain* chainPointer, int capturingPlayer){

	//local variables
	single_Intersection* currentStone;	//pointer to a currently analyzed stone
	int pos;	//holds position of a currently analysed stone
	int ce;		//counter

	//remove stone by stone
	currentStone = chainPointer->firstStone;	//get first stone in chain
	for(int i = 0; i < chainPointer->size; i++){

		//delete stone (reset atribute values at intersection)
		//currentStone->belonging_chain = NULL; //this may not be needed
		currentStone->stone = 0;

		//calculate position by offset from array start
		pos = currentStone - boardState;

		//update liberties of nearby enemy chains
		ce = 0;
		if(GOENG_VALID_NORT(pos)) adjacentCheckDel(GOENG_NORT(pos),&ce, capturingPlayer);	//executes if not in top row
		if(GOENG_VALID_SOUT(pos)) adjacentCheckDel(GOENG_SOUT(pos),&ce, capturingPlayer);	//executes if not in bottom row
		if(GOENG_VALID_WEST(pos)) adjacentCheckDel(GOENG_WEST(pos),&ce, capturingPlayer);	//executes if not in left column
		if(GOENG_VALID_EAST(pos)) adjacentCheckDel(GOENG_EAST(pos),&ce, capturingPlayer);	//executes if not in right column

		//clear flag of updated chains by single stone deletion
		for(int j = 0; j < ce; j++)
			adjacentChain[2][j]->flag2 = 0;

		//update available game actions
		moves[0][pos+1] = 1;
		moves[1][pos+1] = 1;

		//get next stone in chain
		currentStone = currentStone->nextStone;		
	}

	//increase global counter of captured stones
	num_captured[capturingPlayer] += chainPointer->size;

	//reset chain flag set by 
	chainPointer->flag = 0;
	//no need to clear other atributes as they are reset when a new stone is placed (a new chain created), all the link were already broken in the first part of this procedure

	//increase counter of avaliable game actions
	num_moves[0] += chainPointer->size;
	num_moves[1] += chainPointer->size;
}

/**
Function for adjacency check for all four directions when deleting a stone
*/
void GoGameEngine::adjacentCheckDel(int adjacentPosition, int* counter_enemy, int capturingPlayer){
	single_Chain* current_chain;			//pointer to a currently analyzed chain

	if(boardState[adjacentPosition].stone - 1 == capturingPlayer){
		current_chain = boardState[adjacentPosition].belonging_chain;	//pointer to belonging chain
		if(current_chain->flag2 == 0){									//was already visited?
			adjacentChain[2][(*counter_enemy)] = current_chain;			//memorize in order to clear flag later
			current_chain->flag2 = 1;									//set to visited
			(*counter_enemy)++;											//increase counter of enemy chains nearby															
			current_chain->liberties++;									//increase liberties by 1
		}
	}
}

/**
Merge multiple chains and last added stone, or crate new chain if there are no existing chains to merge.

@param chainPointers Array of pointers to chains to be merged.
@param newStone Location of lastly placed stone (in 1D cooridnates, equals to selected move)
@param num_chains How many chain are to be merged (maximum 4)
@param newStoneLiberties The number of liberties of the new stone
*/
void GoGameEngine::mergeChains(int newStone, int num_chains){

	//local variables
	int largest, maxSize;					//the largest of chains to be merged; and its size
	single_Intersection* currentStone;		//pointer to a currently analyzed stone

	//check if a new chain must be created
	if(num_chains == 0){
		boardState[newStone].belonging_chain = &chains[newStone];	//set pointer to new chain root for new stone
		chains[newStone].firstStone = &boardState[newStone];		//set pointer to first stone in chain for new chain
		chains[newStone].lastStone = &boardState[newStone];			//set pointer to last stone
		chains[newStone].size = 1;									//set size of new chain
		chains[newStone].liberties = num_liberties;					//set number of liberties of new chain (equals to liberties of new stone)
		chains[newStone].flag = 0;									//set flag to zero

	//no new chain will be created - nearby chains are present 
	}else{
		//liberties must be recalculated from merged chains
		num_liberties = 0;

		//only one adjacent chain
		if(num_chains == 1){

			//erase flag
			adjacentChain[0][0]->flag = 0;	

			//1. add new stone to nearby chain
			boardState[newStone].belonging_chain = adjacentChain[0][0];			//set pointer to extisting chain root for new stone
			adjacentChain[0][0]->size++;										//increase size of chain
			adjacentChain[0][0]->lastStone->nextStone = &boardState[newStone];	//link last stone in chain to new stone
			adjacentChain[0][0]->lastStone = &boardState[newStone];				//set pointer to new last stone								

			//2. recalculate liberties
			liberties_markChain(adjacentChain[0][0]);	//TODO: remove this line and uncomment the one below
			//TODO: liberties_addOneStone(newStone);
			adjacentChain[0][0]->liberties = num_liberties;

		//some chains need to be merged
		}else{

			//add new stone and merge two chains
			if(num_chains == 2){

				//1. figure out the largest chain
				if(adjacentChain[0][0]->size >= adjacentChain[0][1]->size)
					largest = 0;
				else
					largest = 1;

				//erase flag
				adjacentChain[0][largest]->flag = 0;

				//2. add new stone to largest chain
				boardState[newStone].belonging_chain = adjacentChain[0][largest];				//set pointer to extisting chain root for new stone
				adjacentChain[0][largest]->lastStone->nextStone = &boardState[newStone];		//link last stone in chain to new stone
				adjacentChain[0][largest]->size++;												//needed for liberties_markChain() to work correctly

				//3. iterate through larger chain and mark liberties
				liberties_markChain(adjacentChain[0][largest]);

				//4. add smaller chain to the largest
				adjacentChain[0][largest]->lastStone = adjacentChain[0][1-largest]->lastStone;	//set pointer to new last stone
				adjacentChain[0][largest]->size += adjacentChain[0][1-largest]->size;			//sum sizes (new stone + smaller chain)

				//4.1 iterate through smaller chain: for each element set pointer to larger chain
				currentStone = adjacentChain[0][1-largest]->firstStone;							//first stone in smaller chain
				boardState[newStone].nextStone = currentStone;									//link from largest chain to smaller
				for(int i = 0; i < adjacentChain[0][1-largest]->size; i++){

					//set pointer to new chain
					currentStone->belonging_chain = adjacentChain[0][largest];
						
					//mark liberties
					liberties_markStone(currentStone);

					//get next stone in chain
					currentStone = currentStone->nextStone;
				}

			//add new stone and merge multiple chains (three or four)
			}else{

				//1. figure out largest chain
				maxSize = 0;
				for(int i = 0; i < num_chains; i++)
					if(adjacentChain[0][i]->size > maxSize){
						maxSize = adjacentChain[0][i]->size;
						largest = i;
					}

				//erase flag
				adjacentChain[0][largest]->flag = 0;

				//2. add new stone to largest chain
				boardState[newStone].belonging_chain = adjacentChain[0][largest];				//set pointer to extisting chain root for new stone
				adjacentChain[0][largest]->lastStone->nextStone = &boardState[newStone];		//link last stone in chain to new stone
				adjacentChain[0][largest]->size++;												//increase size by one

				//3. iterate through larger chain and mark liberties
				liberties_markChain(adjacentChain[0][largest]);

				//4. iterate through chains
				currentStone = &boardState[newStone];		//lastly played stone is to be linked to first smaller chain
				for(int i = 0; i < num_chains; i++){

					//if not largest chain, then add
					if(i != largest){

						adjacentChain[0][largest]->size += adjacentChain[0][i]->size;	//increase size of merged chain
						currentStone->nextStone = adjacentChain[0][i]->firstStone;		//link last stone of previous chain to first stone of current chain
						currentStone = adjacentChain[0][i]->firstStone;					//set new first stone

						currentStone->belonging_chain = adjacentChain[0][largest];		//set pointer to new chain for first stone
						liberties_markStone(currentStone);								//mark liberties of first stone
						
						//4.1 iterate through stones
						for(int j = 0; j < adjacentChain[0][i]->size-1; j++){
							currentStone = currentStone->nextStone;						//get next stone in chain
							currentStone->belonging_chain = adjacentChain[0][largest];	//set pointer to new chain
							liberties_markStone(currentStone);							//mark liberties of current stone
						}
					}

				}	//END - iterate through chains

				//4. set pointer of largest chain to new last stone
				adjacentChain[0][largest]->lastStone = currentStone;

			}	//END - add new stone and merge multiple chains (three or four)

			//update liberties of merged chain
			adjacentChain[0][largest]->liberties = num_liberties;

		}	//END - some chains need to be merged

	}	//END - no new chain will be created - nearby chains are present
	
}	//END of PROCEDURE

/**
Count liberties when adding one stone to chain (requirement: chain pointer must be stored in adjacentChain[0][0])
*/
void GoGameEngine::liberties_addOneStone(int addedStone){
	//TODO must check if any liberties of the marked stone (up to 4) are
	//already adjacent to the one chain being merged (up to 4x4 if comparisons)
	//and discard them (not take into account when adding number of liberties of new stone to the number of liberties of whole chain)
}

/**
Mark all liberties around a chain.

*/
void GoGameEngine::liberties_markChain(single_Chain* chainPointer){

	single_Intersection* currentStone = chainPointer->firstStone;	//get first stone in chain
	liberties_markStone(currentStone);								//mark liberties for first stone

	//iterate through all stones in chain
	for(int i = 0; i < chainPointer->size-1; i++){					
		currentStone = currentStone->nextStone;						//get next stone in chain
		liberties_markStone(currentStone);							//mark liberties of each stone
	}
}

/**
Mark all liberties around a stone.
*/
void GoGameEngine::liberties_markStone(single_Intersection*  stonePointer){
	
	int pos = stonePointer - boardState;		//position of stone in array

	//check adjacent intersections
	if(GOENG_VALID_NORT(pos)) adjacentLiberty(GOENG_NORT(pos));	//executes if not in top row
	if(GOENG_VALID_SOUT(pos)) adjacentLiberty(GOENG_SOUT(pos));	//executes if not in bottom row
	if(GOENG_VALID_WEST(pos)) adjacentLiberty(GOENG_WEST(pos));	//executes if not in left column
	if(GOENG_VALID_EAST(pos)) adjacentLiberty(GOENG_EAST(pos));	//executes if not in right column
	
}

/**
Function for adjacency check for all four directions when calculating number of liberties of a single stone
*/
void GoGameEngine::adjacentLiberty(int adjacentPosition){
	if((boardState[adjacentPosition].stone == 0)&&(boardState[adjacentPosition].flag != number_of_plays)){	//check if intersection is clear and not yet marked
		boardState[adjacentPosition].flag = number_of_plays;		//mark as visited (with incremental number_of_plays there is no need to erase flags after every move)
		num_liberties++;										//increase liberty count
	}
}

/**
Calculate the score at current game state. Uses territory scoring (empty zone + captures), penalty when passing and komi.

@param bonus_player2 Ammount of bonus points (komi) added to player 2 (white)
@return Positive value indicates player 1 wins (black) and the difference in points
*/
float GoGameEngine::computeScore(float bonus_player2){

	int scoreFirst, retVal;

	//check if score was already calculated for this game position
	if(number_of_plays == last_score_compute)
		return score;

	//iterate through entire board
	scoreFirst = 0;
	for(int i = 0; i < board_size; i++){

		//Check if intersection is empty and if was not yet visited: iterate through empty zones
		//with number_of_plays+2 there is no need to reset the flag after each count
		//+2 is needed not to mislead the gameEngine which also uses the same technique to update the game state
		if((boardState[i].stone == 0) && (boardState[i].flag != number_of_plays+2)){
			
			//reset adjacency flags for current empty zone
			adjacentOne = 0;
			adjacentTwo = 0;

			//recursive count of each empty zone and update score sum
			retVal = computeScoreFlood(i);
			if((adjacentOne == 1)&&(adjacentTwo == 0))
				scoreFirst += retVal;
			else if ((adjacentOne == 0)&&(adjacentTwo == 1))
				scoreFirst -= retVal;
		}
	}

	//save score: apply number of captures, number of passes, and bonus to player two (komi)
	score = scoreFirst + num_captured[0] - num_captured[1] + score_pass - bonus_player2;

	//remember time step
	last_score_compute = number_of_plays;

	//return score
	return score;
}

/**
Call computeScore() with preset komi value
*/
float GoGameEngine::computeScore(){
	return computeScore(this->komi);
}

/**
Recursive count of connected empty intersections and neighbouring players. Flags adjacentOne and adjacentTwo must be preset to zero.

@param intersection Current position in count
@return Number of connected empty intersections, starting from given intersection, also sets flags adjacentOne and adjacentTwo
*/
int	GoGameEngine::computeScoreFlood(int iIndex){

	int countVal;

	//flag as visited
	boardState[iIndex].flag = number_of_plays+2;

	//check neighbours
	countVal = 1;
	if GOENG_VALID_NORT(iIndex)
		computeScoreFlood_dir(GOENG_NORT(iIndex), &countVal);
	if GOENG_VALID_SOUT(iIndex)
		computeScoreFlood_dir(GOENG_SOUT(iIndex), &countVal);
	if GOENG_VALID_WEST(iIndex)
		computeScoreFlood_dir(GOENG_WEST(iIndex), &countVal);
	if GOENG_VALID_EAST(iIndex)
		computeScoreFlood_dir(GOENG_EAST(iIndex), &countVal);

	return countVal;
}

// support function for computeScoreFlood
void GoGameEngine::computeScoreFlood_dir(int dirIndex, int* countVal){
	if(boardState[dirIndex].flag != number_of_plays+2){
		if (boardState[dirIndex].stone == 0)
			*countVal += computeScoreFlood(dirIndex);
		else if (boardState[dirIndex].stone == 1)
			adjacentOne = 1;
		else
			adjacentTwo = 1;
	}
}

/**
Support function for selecting an action from the set of available actions, with validity check

@param serial_number Which action to pick from the set of available actions starting from lowest
@return Number of selected action, including PASS (value 0), returns -1 if error
*/
int GoGameEngine::selectAction(int serial_number){
	
	//check if action falls out of array
	if((serial_number < 0)||(serial_number > num_moves[current_player])){
		if(disable_warnings == 0)
			printf("!!ERROR!! Go Game Engine: move serial label invalid, player %d, move %d, number of plays %d\n",current_player+1,serial_number,number_of_plays);
		return -1;

	//return action
	}else
		return selectActionUnsafe(serial_number);
}

/**
UNSAFE: Support function for selecting an action from the set of available actions, without validity check

@param serial_number Which action to pick from the set of available actions starting from lowest
@return Number of selected action, including PASS (value 0), returns -1 if error
*/
int GoGameEngine::selectActionUnsafe(int serial_number){
	
	//find available action
	int selected = -1;
	for(int i = 0; i < all_actions_num; i++){
		if(moves[current_player][i] == 1){
			if(serial_number == 0){
				selected = i;
				i = all_actions_num;
			}
			serial_number--;
		}
	}

	//check for errors - DEBUG
	//if(selected == -1){
	//	if(disable_warnings == 0){
	//		printf("! ERROR: Go Game Engine: serial number to large by %d, player %d, number of plays since game start %d, num moves %d\n",serial_number,current_player+1,number_of_plays,num_moves[current_player]);
	//		output_state_to_STD();
	//		for(int i = 0; i < board_length; i++){
	//			for(int j = 0; j < board_length; j++)
	//				printf("%d ",moves[current_player][i*board_length+j+1]);
	//			printf("\n");
	//		}

	//		getc(stdin);
	//	}
	//}
	//printf("sel: %d, p: %d\n",selected, current_player+1);	//DEBUG

	return selected;
}

/**
Support function for selecting a random action. Notice that srand(time(NULL)) should be called at main program start.

@return Number of selected action, including PASS (from 0 to board_size)
*/
int GoGameEngine::randomAction(){
	return selectActionUnsafe( ((int)((rand()/(float)(RAND_MAX+1))*(num_moves[current_player]))));
	//return selectAction(num_moves[current_player]);	//DEBUG: always choose last action
}

/**
Output current game state to standard output.

@param showScore Calculate and display current score
*/
void GoGameEngine::output_state_to_STD(bool showScore){
	
	printf("\n   ");
	for(int i = 0; i < board_length; i++)
		printf(" %d",i+1);
	printf("\n  ");
	for(int i = 0, k = 0; i < board_length; i++, k += board_length){
		printf("\n%2d ",i+1);
		for(int j = 0; j < board_length; j++){
			if(k+j != previous_move-1)
				printf(" %d", boardState[k+j].stone);
			else{
				//printf("*%d", boardState[k+j].stone);
				if(j < board_length-1){
					printf("+%d+%d", boardState[k+j].stone, boardState[k+j+1].stone);
					j++;
				}else
					printf("+%d+", boardState[k+j].stone);
			}
		}
	}
	printf("\n");
	if(previous_move == 0)
		printf("P%d passed",(1-current_player)+1);
	printf("\nPLY: %2d",number_of_plays);
	if(showScore)
		printf(" \t SCORE: %.1f",computeScore());
	printf("\n\n");
	fflush(stdout);

}

/**
Output current chains to standard output.

@param showScore Calculate and display current score
*/
void GoGameEngine::output_chains_to_STD(){
	
	printf("\n   ");
	for(int i = 0; i < board_length; i++)
		printf(" %3d",i+1);
	printf("\nCHAINS  ");
	for(int i = 0, k = 0; i < board_length; i++, k += board_length){
		printf("\n%2d ",i+1);
		for(int j = 0; j < board_length; j++){
			if(boardState[k+j].belonging_chain == NULL){
				printf(" ---");
			}else{
				if(boardState[k+j].stone == 0){
					printf(" %2de", (boardState[k+j].belonging_chain - chains));
				}else if(boardState[k+j].stone == 1){
					printf(" %2db", (boardState[k+j].belonging_chain - chains));
				}else if(boardState[k+j].stone == 2){
					printf(" %2dw", (boardState[k+j].belonging_chain - chains));
				}else{
					printf(" err");
				}
			}
		}
	}
	printf("\n");
	if(previous_move == 0)
		printf("P%d passed",(1-current_player)+1);
	printf("\nPLY: %2d\n",number_of_plays);
	fflush(stdout);

}

/**
Benchmark function for game playing: measures execution time.

@param num_games Number of games to simulate.
@param moves_percent Number of plys per game - percentage of total board size, float value > 0.0
@param printTime Enable/disable display of CPU execution time
@return Duration of simulations in miliseconds.
*/
double GoGameEngine::benchmark_playingTime(int num_games, float moves_percent, bool printTime){
	double cpu_time = getCPUTime();

	//run simulations
	for(int i = 0; i < num_games; i++){
		resetGame();
		for(int j = 0; j < (int)(board_size*moves_percent); j++)
			if(playMoveUnsafe(randomAction()) != 0)
				j = (int)(board_size*moves_percent);
	}

	//return elapsed time
	cpu_time = getCPUTime()-cpu_time;
	if(printTime)
		printf("PLAYED %d games, up to %d moves, %dx%d board, runtime: %9.3f s %9.1f us/game\n",
			num_games,
			(int)(board_size*moves_percent),
			board_length,
			board_length,
			cpu_time,
			cpu_time / num_games * 1000*1000
		);
	fflush(stdout);
	return cpu_time;
}

/**
Debug benchmark function for game playing, measures outcome statistics: number of finished games, average score WITHOUT komi, percentage of each player wins and draws

@param num_games Number of games to simulate.
@param moves_percent Number of plys per game - percentage of total board size, float value > 0.0
@return Duration of simulations in miliseconds.
*/
double GoGameEngine::benchmark_playingStat(int num_games, float moves_percent){
	int countFinished, countWin1, countWin2;
	INT64 countMoves;
	float tmpScore, avgScore;
	double cpu_time, move_time;

	move_time = 0;
	cpu_time = getCPUTime();

	countFinished = 0;
	countWin1 = 0;
	countWin2 = 0;
	avgScore = 0.0f;
	countMoves = 0;

	//run simulations
	for(int i = 0; i < num_games; i++){
		resetGame();
		move_time -= getCPUTime();
		for(int j = 0; j < (int)(board_size*moves_percent); j++){
			countMoves++;
			if(playMoveUnsafe(randomAction()) != 0){
				j = (int)(board_size*moves_percent);
				//printf("GAME FINISHED: %d\n", number_of_plays); output_state_to_STD(false);	//DEBUG output
				countFinished++;
			}
		}
		move_time += getCPUTime();
		tmpScore = computeScore(0.0);
		avgScore += tmpScore;
		if(score > 0)
			countWin1++;
		else if(score < 0)
			countWin2++;
	}

	//average and output results
	avgScore /= num_games;
	
	//output results and return elapsed time
	cpu_time = getCPUTime()-cpu_time;
	printf("\nRESULT %d games, up to %d moves, %dx%d board, runtime: %9.3f s %9.1f us/game %9.0f ns/move\n .finished games:\t%2.1f%%  [%d games]\n .avg moves per game:\t%.1f\n .average score:\t%3.3f\n .outcomes w1/d/w2:\t%2.2f%%  %2.2f%%  %2.2f%%\n\n",
		num_games,
		(int)(board_size*moves_percent),
		board_length,
		board_length,
		cpu_time,
		cpu_time / num_games * 1000*1000,
		move_time / countMoves * 1000*1000*1000,
		(float)countFinished/num_games*100.0f,
		countFinished,
		countMoves/(double)num_games,
		avgScore, (float)countWin1/num_games*100.0f,
		(float)(num_games-countWin1-countWin2)/num_games*100.0f,
		(float)countWin2/num_games*100.0f
	);
	fflush(stdout);

	return cpu_time;
}

/**
Benchmark function for game scoring: measures execution time.

@param num_games Number of games to simulate.
@param moves_percent Number of plys per game - percentage of total board size, float value > 0.0
@param num_scores Number of score calculation repeats per game.
@param printTime Enable/disable display of CPU execution time
@return Duration of simulations in miliseconds.
*/
double GoGameEngine::benchmark_scoring(int num_games, float moves_percent, int num_scores, bool printTime){
	
	double cpu_time, game_sim_time, score_time, move_time;
	double tmpScore = 0;

	game_sim_time = 0;
	move_time = 0;
	score_time = 0;

	cpu_time = getCPUTime();

	//run simulations
	for(int i = 0; i < num_games; i++){
		game_sim_time -= getCPUTime();
		resetGame();
		move_time -= getCPUTime();
		for(int j = 0; j < (int)(board_size*moves_percent); j++){
			playMoveUnsafe(randomAction());
			//output_state_to_STD(false);	//DEBUG output
		}
		move_time += getCPUTime();
		game_sim_time += getCPUTime();

		score_time -= getCPUTime();
		for(int j = 0; j < num_scores; j++){
			//printf("%.1f %d\n",computeScore(), number_of_plays);	//DEBUG output
			tmpScore += computeScore();
			number_of_plays++;
		}
		score_time += getCPUTime();
	}

	//return elapsed time
	cpu_time = getCPUTime()-cpu_time;
	//printf("test: %3.3f %3.3f %3.3f %3.3f\n", cpu_time, (score_time+game_sim_time),game_sim_time,score_time);	//DEBUG output
	if(printTime)
		printf("SCORED %d games, %d moves, %d scores, %dx%d board, runtime: %9.3f s %9.1f us/game %9.0f ns/move %9.0f ns/score\n",
			num_games,
			(int)(board_size*moves_percent),
			num_scores,
			board_length,
			board_length,
			cpu_time,
			game_sim_time / num_games * 1000*1000,
			move_time / (num_games*(int)(board_size*moves_percent)) * 1000*1000*1000,
			score_time / (num_games*num_scores) * 1000*1000*1000
		);
	fflush(stdout);

	score = (float)tmpScore/num_games/num_scores;

	return cpu_time;
}


/**
A series of benchmarks to measure execution time.
*/
void GoGameEngine::benchmarkSeries(){
	
	srand((unsigned int)time(NULL));

	GoGameEngine game02(2);
	GoGameEngine game05(5);
	GoGameEngine game10(10);
	GoGameEngine game19(19);

	game02.benchmark_playingTime(999);
	game02.benchmark_playingTime(200000);
	game02.benchmark_playingTime(200000);
	game02.benchmark_playingTime(200000);
	game05.benchmark_playingTime(999);
	game05.benchmark_playingTime(200000);
	game05.benchmark_playingTime(200000);
	game05.benchmark_playingTime(200000);
	game10.benchmark_playingTime(999);
	game10.benchmark_playingTime(200000);
	game10.benchmark_playingTime(200000);
	game10.benchmark_playingTime(200000);
	game19.benchmark_playingTime(999);
	game19.benchmark_playingTime(200000);
	game19.benchmark_playingTime(200000);
	game19.benchmark_playingTime(200000);

	game02.benchmark_scoring(999);
	game02.benchmark_scoring(50000);
	game02.benchmark_scoring(50000);
	game02.benchmark_scoring(50000);
	game05.benchmark_scoring(999);
	game05.benchmark_scoring(50000);
	game05.benchmark_scoring(50000);
	game05.benchmark_scoring(50000);
	game10.benchmark_scoring(999);
	game10.benchmark_scoring(50000);
	game10.benchmark_scoring(50000);
	game10.benchmark_scoring(50000);
	game19.benchmark_scoring(999);
	game19.benchmark_scoring(50000);
	game19.benchmark_scoring(50000);
	game19.benchmark_scoring(50000);
}

/**
A sample sequence of moves to check GoEngine corectness
*/
void GoGameEngine::debugTestSequence(int board_width){
	GoGameEngine game(board_width);
	
	//DEBUG: test sequence
	game.output_state_to_STD(true); 
	game.playMove(6); game.output_state_to_STD(true);
	game.playMove(7); game.output_state_to_STD(true);
	game.playMove(1); game.output_state_to_STD(true);
	game.playMove(11); game.output_state_to_STD(true);
	game.playMove(2); game.output_state_to_STD(true);
	game.playMove(3); game.output_state_to_STD(true);
	game.playMove(1); game.output_state_to_STD(true);
	game.playMove(2); game.output_state_to_STD(true);
	game.playMove(4); game.output_state_to_STD(true);
	game.playMove(0); game.output_state_to_STD(true);
	game.playMove(16); game.output_state_to_STD(true);
	game.playMove(0); game.output_state_to_STD(true);
	game.playMove(12); game.output_state_to_STD(true);
	game.playMove(0); game.output_state_to_STD(true);
	game.playMove(8); game.output_state_to_STD(true);
	game.playMove(6); game.output_state_to_STD(true);
	game.playMove(0); game.output_state_to_STD(true);
	game.playMove(1); game.output_state_to_STD(true);
	game.playMove(25); game.output_state_to_STD(true);
}

/**
Constructor for general Go player module

@param currentGame Pointer to instance of GoGameEngine object
*/
PlayerGo::PlayerGo(GoGameEngine* currentGame){
	this->game = currentGame;
	resetAfterSeries = false;
	isHuman = false;
	player_number = 0;
}

/**
Constructor for human Go player module calling general Go player constructor
*/
PlayerGoHuman::PlayerGoHuman(GoGameEngine* currentGame) : PlayerGo(currentGame){
	isHuman = true;
}

/**

*/
int PlayerGoHuman::getMove(){

	int move_num, error, inVal1, inVal2;

	//repeat until selected move is valid
	move_num = -1;
	while(move_num == -1){
		cout << "Waiting for player" << game->current_player+1 << " input (Y X): ";
		error = scanf("%d %d",&inVal1, &inVal2);
		if(error == 0){
			cin.clear();
			cin.ignore();
			cout << "invalid input!" << endl;
		}else{
			if((inVal1 == 0)&&(inVal2 == 0)){
				move_num = 0;
			}else if((inVal1 <= 0)||(inVal1 > game->board_length)||(inVal2 <= 0)||(inVal2 > game->board_length))
				cout << "invalid move!" << endl;
			else{
				move_num = (inVal1-1) * game->board_length + (inVal2-1)+ 1;
				if( !game->moves[game->current_player][move_num] ){
					cout << "invalid move!" << endl;
					move_num = -1;
				}
			}
		}
	}

	return move_num;

};

/**
Play a 1v1 human game by standard input/output.
For each move the player inputs the horizontal and vertical coordinate, e.g.: 4 6
PASS action is played with: 0 0

@param board_width Size of game board
@param showCurrentScore Display score after each play
*/
void GoGameEngine::humanGame(int board_width, bool showCurrentScore, float komi){

	//instantiate game
	GoGameEngine* game = new GoGameEngine(board_width);
	game->komi = komi;

	//instantiate players
	PlayerGo* players[] = {new PlayerGoHuman(game), new PlayerGoHuman(game)};

	//play game
	game->playOutputGame(players, showCurrentScore);

	//clean up
	delete players[0];
	delete players[1];
	//delete *players;	//not sure if needed
	delete game;
}

/**
Show and play a single game on standard output.

@param game Instance of GoGameEngine object
@param players Array of two pointers to PlayerGo objects
@param showCurrentScore Display score after each play
*/
void GoGameEngine::playOutputGame(PlayerGo** players, bool showCurrentScore){

	int feedback;
	float outcome;

	//start game
	this->resetGame();
	
	//play game until end is signaled
	feedback = 0;
	while(feedback == 0){
		
		//output game state
		this->output_state_to_STD(showCurrentScore);

		//play next move
		//-human player
		if(players[this->current_player]->isHuman){
			feedback = this->playMoveUnsafe(players[this->current_player]->getMove());
		//-computer player
		}else{
			feedback = this->playMove(players[this->current_player]->getMove());
		}

	}

	//game ended
	this->output_state_to_STD(true);
	outcome = this->computeScore();
	cout << "GAME ENDED: ";
	if(outcome > 0)
		cout << "player ONE wins";
	else if(outcome < 0)
		cout << "player TWO wins";
	else
		cout << "draw";

	cout << endl << endl << "Press any key to exit.";
	cin.ignore();
	cin.get();

}

void GoGameEngine::evaluatePlayersPerformance(PlayerGo** players, int num_repeats, int num_games, float new_komi, bool print_wait, float max_moves_factor){

	int game_status, winP1, winP2, draws, max_moves, totalWinP1;

	max_moves = (int)(max_moves_factor * this->board_length*this->board_length);
	totalWinP1 = 0;

	if(print_wait)
		printf(" win P1 | win P2 | draws \n");
	else
		printf(" %% win P1 | win P1 | win P2 | draws \n");

	//execute games
	srand((unsigned int)time(NULL));
	for(int j = 0; j < num_repeats; j++){
		
		winP1 = 0;
		winP2 = 0;
		draws = 0;
				
		if(players[0]->resetAfterSeries)
			players[0]->reset();
		if(players[1]->resetAfterSeries)
			players[1]->reset();

		for(int i = 0; i < num_games; i++){

			game_status = 0;
			while((game_status == 0)&&(this->number_of_plays < max_moves)){		
				game_status = this->playMove(players[this->current_player]->getMove());
				
				//if(print_wait)
				//	this->output_state_to_STD(true);
			}
			this->computeScore(new_komi);
			if(this->score > 0)
				winP1++;
			else if(this->score < 0)
				winP2++;
			else
				draws++;

			if(print_wait)
				printf("%5d %5d %5d \t game %5d score %3.1f\n", winP1, winP2, draws, i, this->score);

			//game must not be resetted before this call
			players[0]->endGame();
			players[1]->endGame();

			//game must be reset before players->newGame is called
			this->resetGame();

			//game must be resetted before this call
			players[0]->newGame();
			players[1]->newGame();

		}
		printf("%3.0f %% %5d %5d %5d \t game %5d score %3.1f\n", (double)winP1/num_games*100, winP1, winP2, draws, (j+1)*num_games, this->score);

		totalWinP1 += winP1;
	}

	printf("\nAverage P1 win rate: %2.3f\n\n",(double)totalWinP1/(num_games*num_repeats));
	players[0]->output();
	players[1]->output();

}

void GoGameEngine::copyGameState(GoGameEngine* sourceGame, GoGameEngine* targetGame){

	//copy parameters
	targetGame->score_pass_point = sourceGame->score_pass_point;
	targetGame->disable_warnings = sourceGame->disable_warnings;
	targetGame->komi = sourceGame->komi;

	//copy variables
	targetGame->ended = sourceGame->ended;
	targetGame->current_player = sourceGame->current_player;
	targetGame->number_of_plays = sourceGame->number_of_plays;

	targetGame->num_moves[0] = sourceGame->num_moves[0];
	targetGame->num_moves[1] = sourceGame->num_moves[1];

	targetGame->num_captured[0] = sourceGame->num_captured[0];
	targetGame->num_captured[1] = sourceGame->num_captured[1];
	targetGame->score = sourceGame->score;
	targetGame->score_pass = sourceGame->score_pass;

	targetGame->previous_move = sourceGame->previous_move;
	targetGame->last_score_compute = sourceGame->last_score_compute;

	//copy values in arrays: MUST PAY ATENTION TO POINTERS! (that is why if-sentences are needed)
	for(int i = 0; i < targetGame->board_size; i++){
		targetGame->boardState[i].stone = sourceGame->boardState[i].stone;
		targetGame->boardState[i].flag = sourceGame->boardState[i].flag;
		if(sourceGame->boardState[i].belonging_chain == NULL)
			targetGame->boardState[i].belonging_chain = NULL;
		else
			targetGame->boardState[i].belonging_chain = (sourceGame->boardState[i].belonging_chain - sourceGame->chains) + targetGame->chains;
		if(sourceGame->boardState[i].nextStone == NULL)
			sourceGame->boardState[i].nextStone = NULL;
		else
			targetGame->boardState[i].nextStone = (sourceGame->boardState[i].nextStone - sourceGame->boardState) + targetGame->boardState;

		targetGame->chains[i].size = sourceGame->chains[i].size;
		targetGame->chains[i].liberties = sourceGame->chains[i].liberties;
		targetGame->chains[i].flag = sourceGame->chains[i].flag;
		targetGame->chains[i].flag2 = sourceGame->chains[i].flag2;
		if(sourceGame->chains[i].firstStone == NULL)
			targetGame->chains[i].firstStone = NULL;
		else
			targetGame->chains[i].firstStone = (sourceGame->chains[i].firstStone - sourceGame->boardState) + targetGame->boardState;
		if(sourceGame->chains[i].lastStone == NULL)
			targetGame->chains[i].lastStone = NULL;
		else
			targetGame->chains[i].lastStone = (sourceGame->chains[i].lastStone - sourceGame->boardState) + targetGame->boardState;

		targetGame->moves[0][i] = sourceGame->moves[0][i];
		targetGame->moves[1][i] = sourceGame->moves[1][i];
	}
	targetGame->moves[0][targetGame->all_actions_num-1] = sourceGame->moves[0][sourceGame->all_actions_num-1];
	targetGame->moves[1][targetGame->all_actions_num-1] = sourceGame->moves[1][sourceGame->all_actions_num-1];

}

GoGameEngine* GoGameEngine::createDuplicateGame(GoGameEngine* sourceGame){
	GoGameEngine* newGame = new GoGameEngine(sourceGame->board_length, false);
	copyGameState(sourceGame, newGame);
	return newGame;
}

