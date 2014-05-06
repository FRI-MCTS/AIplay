/**
Lrp Learning automata
*/

//includes
#include "Tom_Lrp.hpp"


// -----------------------------------------------------------//
// GENERAL TEMPLATE FOR OBJECTS

Tom_Common_Object::Tom_Common_Object()
{
	//init variables
	flag_is_initialized = false;
	flag_is_allocated = false;
}

void Tom_Common_Object::Initialize(const int without_allocation)
{
	//allocate non-resettable memory and set constants (independent of settable parameters)
	Memory_Init_Constants();

	//set init flag (cannot be reset, unless object is deleted)
	flag_is_initialized = true;

	//allocate memory and initialize variables
	if(!without_allocation)
		Reinitialize(TOM_COMMON_OBJECT_INIT_SETTINGS_RESET);
}

void Tom_Common_Object::Reinitialize(const int apply_new_settings)
{
	//check init flag
	if(flag_is_allocated){
		Memory_Free();
	}

	//initialize settings
	if(apply_new_settings == TOM_COMMON_OBJECT_INIT_SETTINGS_APPLY_NEW){
		Settings_Apply_New_Private();
	}else{
		Settings_Reset_Private();
	}
		
	//allocate resources
	Memory_Allocate();
	flag_is_allocated = true;

	//reset game state
	State_Reset();
}

//allocate memory and initialize variables
void Tom_Common_Object::Settings_Reset()
{
	Reinitialize(TOM_COMMON_OBJECT_INIT_SETTINGS_RESET);
}

void Tom_Common_Object::Settings_Apply_New()
{
	Reinitialize(TOM_COMMON_OBJECT_INIT_SETTINGS_APPLY_NEW);
}

// -----------------------------------------------------------//
// Linear Reward Punishment (LRP) Learning Automaton

Tom_Lrp::~Tom_Lrp()
{
	if(flag_is_allocated)	Memory_Free();				//clear memory if allocated
	if(flag_is_initialized)	Memory_Free_Constants();	//clear constants if allocated
};

void Tom_Lrp::Memory_Init_Constants()
{
	//init constants
	//object_info[TOMLRP_INFO_NAME] =			"Lrp Learning Automata";
	//object_info[TOMLRP_INFO_NAME_SHORT] =		"LRP";
	//object_info[TOMLRP_INFO_DESCRIPTION] =	"Learning Automata with Reward-Penalty policy";
}

void Tom_Lrp::Settings_Reset_Private()
{
	num_actions = TOM_LRP_DEFAULT_NUM_ACTIONS;
	learn_param_a = TOM_LRP_DEFAULT_PARAM_A;
	learn_param_b = TOM_LRP_DEFAULT_PARAM_B;
}

void Tom_Lrp::Memory_Allocate()
{
	action_probabilities = new double[num_actions];
	evaluations_history = NULL;
	num_updates_since_alloc = 0;
	evaluations_history_num_samples = 0;
	evaluations_history_num_param_values = 0;
}

void Tom_Lrp::Memory_Free()
{
	delete(action_probabilities);
	if(evaluations_history != NULL){
		for(int i = 0; i < evaluations_history_max_samples; i++)
			delete(evaluations_history[i].param_values);
		delete(evaluations_history);
	}
}

void Tom_Lrp::State_Reset()
{
	for(int i = 0; i < num_actions; i++){
		action_probabilities[i] = 1.0 / (double)num_actions;
	}
	num_updates_since_reset = 0;
}

int Tom_Lrp::Select_Action(const int check_sum)
{
	int selected_action = 0;

	//set random threshold
	#if(!TOM_LRP_USE_SQUARE_RANDOM)
		double rand_val = (double)rand()/(RAND_MAX+1);
	#else
		double rand_val = rand()*rand()/(RAND_MAX*RAND_MAX+1);
	#endif

	//select action based on probability distribution and random threshold
	double sum = 0.0;
	int i = 0;
	while((i < num_actions)&&(sum <= rand_val)){
		sum += action_probabilities[i];
		i++;
	}
	selected_action = i-1;

	//optional: check if sum of actions is within bounds
	if(check_sum){

		//calculate sum of all probabilites (continue where the previous loop finished)
		for(; i < num_actions; i++)
			sum += action_probabilities[i];

		//check if error bound exceeded
		if(abs(sum - 1.0) > TOM_LRP_SUM_ERROR_BOUND){
			
			//correct probabilites
			for(i = 0; i < num_actions; i++)
				action_probabilities[i] /= sum;

			//show warning (numerical drift correction)
			//TODO
			printf("\nLRP WARNING: numerical drift, prob sum: %3.9f\n", sum);
		}

	}

	return selected_action;
}

//LRP equation for reward
void Tom_Lrp::Update_Probabilites_Reward(int action_index, double change_step)
{
	for(int i = 0; i < num_actions; i++)
		action_probabilities[i] *= (1 - change_step);
	action_probabilities[action_index] += change_step;

	num_updates_since_alloc++;
	num_updates_since_reset++;
}

//LRP equation for penalty
void Tom_Lrp::Update_Probabilites_Penalty(int action_index, double change_step)
{
	for(int i = 0; i < num_actions; i++)
		action_probabilities[i] = action_probabilities[i] * (1 - change_step) + (change_step/(num_actions-1));
	action_probabilities[action_index] -= (change_step/(num_actions-1));

	num_updates_since_alloc++;
	num_updates_since_reset++;
}

//Output current probabilites
void Tom_Lrp::Debug_Display_Probabilites(const int output_detail_level)
{
	if(output_detail_level >= 1){
		if(output_detail_level >= 2)
			printf("\n");

		printf("LRP: Action Probabilites");

		if(output_detail_level >= 2){
			printf("\n  Action   ");
			for(int i = 0; i < num_actions; i++)
				printf("     %2d",i);
			printf("     SUM");
			printf("\n  Prob [%%] ");
		}else{
			printf(" [%%]");
		}

	}

	double sum = 0.0;
	for(int i = 0; i < num_actions; i++){
		printf("\t %5.2f",action_probabilities[i]*100.0);
		sum += action_probabilities[i];
	}

	if(output_detail_level >= 1){
		printf("     %11.9f",sum*100);
		printf("\n");
	}
}

//Management of evaluations history array
void Tom_Lrp::Evaluations_History_Allocate(int max_num_samples, int num_parameter_weights)
{
	evaluations_history_max_samples = max_num_samples;
	evaluations_history_num_param_values = num_parameter_weights;

	evaluations_history = new LRP_sample[max_num_samples];
	for(int i = 0; i < evaluations_history_max_samples; i++)
		evaluations_history[i].param_values = new double[evaluations_history_num_param_values];
}
void Tom_Lrp::Evaluations_History_Resize(int new_max_num_samples, int new_num_parameter_weights)
{
	//allocate new array
	LRP_sample*	newArray = new LRP_sample[new_max_num_samples];
	for(int i = 0; i < new_max_num_samples; i++)
		newArray[i].param_values = new double[new_num_parameter_weights];

	//copy old to new and delete old
	for(int i = 0; i < evaluations_history_max_samples; i++){
		newArray[i].previous_action = evaluations_history[i].previous_action;
		newArray[i].num_repeats = evaluations_history[i].num_repeats;
		newArray[i].score = evaluations_history[i].score;
		newArray[i].stddev = evaluations_history[i].stddev;
		newArray[i].kNN_score = evaluations_history[i].kNN_score;
		
		for(int j = 0; j < evaluations_history_num_param_values; j++)
			newArray[i].param_values[j] = evaluations_history[i].param_values[j];

	}
	delete(evaluations_history);
	evaluations_history = newArray;

	//update global variables
	evaluations_history_max_samples = new_max_num_samples;
	evaluations_history_num_param_values = new_num_parameter_weights;
}
void Tom_Lrp::Evaluations_History_Add_Sample(int action, int num_repeats, double score, double stddev, double* parameter_weights)
{
	evaluations_history[evaluations_history_num_samples].previous_action = action;
	evaluations_history[evaluations_history_num_samples].num_repeats = num_repeats;
	evaluations_history[evaluations_history_num_samples].score = score;
	evaluations_history[evaluations_history_num_samples].stddev = stddev;
	evaluations_history[evaluations_history_num_samples].kNN_score = 0.0;	//TODO: create function Add_Sample_kNN to calculate kNN score online
	for(int j = 0; j < evaluations_history_num_param_values; j++)
		evaluations_history[evaluations_history_num_samples].param_values[j] = parameter_weights[j];
	evaluations_history_num_samples++;
}
void Tom_Lrp::Debug_Display_Evaluations_History()
{
	printf("Tom_Lrp::Debug_Display_Evaluations_History()\n");
	printf("  i \taction rep score    dev \t knnScr \t param_weights\n");
	for(int i = 0; i < evaluations_history_num_samples; i++){
		printf("%3d \t %2d  %4d  %5.2f%%  %5.2f%% \t %5.2f%% \t", i, evaluations_history[i].previous_action, evaluations_history[i].num_repeats, evaluations_history[i].score*100.0, evaluations_history[i].stddev*100.0, evaluations_history[i].kNN_score*100.0);
		for(int j = 0; j < evaluations_history_num_param_values; j++)
			printf(" %1.5f",evaluations_history[i].param_values[j]);
		printf("\n");
	}
}

/**
Return index of sample with highest score in evaluations_history array
If multiple best scores are found, the most recent one is selected (the latest in the array)

@return Index of sample with best score
*/
int Tom_Lrp::Evaluations_History_Best_Single(const int output_detail_level)
{
	int best_index = -1;
	double best_score = -DBL_MAX;

	//loop through all samples
	for(int s = 0; s < evaluations_history_num_samples; s++){
		if(evaluations_history[s].score >= best_score){
			best_score = evaluations_history[s].score;
			best_index = s;
		}
	}

	//debug output
	if(output_detail_level > 0){
		printf("Best value: ind %d \t scr %5.2f%% \t weights ", best_index, best_score*100.0);
		for(int j = 0; j < evaluations_history_num_param_values; j++)
			printf(" %1.5f",evaluations_history[best_index].param_values[j]);
		printf("\n");
	}

	//return index of best 
	return best_index;
}

/**
Search for the highest kNN-averaged score in the evaluations_history array
Current implementation is CPU-inefficient (slow): if an array of k-nearest neighbours would be kept for each sample, then the neighoburs could be more easily set when adding each new sample
If multiple best scores are found, the most recent one is selected (the latest in the array)

@return Index of sample with best average score according to num_neighbours nearest neighbours
*/
int Tom_Lrp::Evaluations_History_Best_KNN_Average_Slow(int num_neighbours, const bool weighted_by_repeats, const double central_weight_factor, const bool manhattan_distance, const int output_detail_level)
{
	//check if there is enough neighbours
	if(num_neighbours > (evaluations_history_num_samples - 1))
		num_neighbours = evaluations_history_num_samples - 1;	//set new value to number of all samples, excluding self

	//check if number of neighbours higher than zero
	if(num_neighbours < 1)
		return Evaluations_History_Best_Single(output_detail_level);
	
	//kNN algorithm based on distance between parameter weights
	double current_score, current_distance, sum_weights;
	double* neighbours_distance = new double[num_neighbours];	//sorted list of nearest neigbours, nearest is first
	int* neighbours_index = new int[num_neighbours];

	int best_index = -1;
	double best_score = -DBL_MAX;

	//loop through all samples
	for(int s1 = 0; s1 < evaluations_history_num_samples; s1++){

		//clear temp array
		for(int n = 0; n < num_neighbours; n++){
			neighbours_distance[n] = DBL_MAX;
		}

		//find nearest neighbours
		for(int s2 = 0; s2 < evaluations_history_num_samples; s2++){
			//ignore self
			if(s2 != s1){

				//calculate distance
				current_distance = 0;
				if(manhattan_distance){	//manhattan distance
					for(int p = 0; p < evaluations_history_num_param_values; p++)
						current_distance += abs(evaluations_history[s1].param_values[p] - evaluations_history[s2].param_values[p]);
				}else{					//squared geometrical distance
					for(int p = 0; p < evaluations_history_num_param_values; p++)
						current_distance += ((evaluations_history[s1].param_values[p] - evaluations_history[s2].param_values[p])*(evaluations_history[s1].param_values[p] - evaluations_history[s2].param_values[p]));
				}

				//check if s2 is k-nearest
				for(int n = 0; n < num_neighbours; n++){
					if(current_distance < neighbours_distance[n]){
						//shift array by one element
						for(int t = num_neighbours-1; t > n; t--){
							neighbours_distance[t] = neighbours_distance[t-1];
							neighbours_index[t] = neighbours_index[t-1];
						}

						//insert new
						neighbours_distance[n] = current_distance;
						neighbours_index[n] = s2;

						//break the "check k-nearest" loop
						n = num_neighbours;
					}
				}

			}
		}
		//END - find nearest neighbours

		//--average score with nearest neighbours
		
		//non-weighted kNN
		if(!weighted_by_repeats){

			current_score = evaluations_history[s1].score * central_weight_factor;	//score of observed sample, with applied scalar weigth (if weight is 1.0 then it is equal to all others)
			for(int n = 0; n < num_neighbours; n++)
				current_score += evaluations_history[neighbours_index[n]].score;
			current_score /= (num_neighbours + central_weight_factor);

		//kNN weighted by num of repeats per sample
		}else if(weighted_by_repeats){
			current_score = evaluations_history[s1].score * evaluations_history[s1].num_repeats * central_weight_factor;
			sum_weights = evaluations_history[s1].num_repeats * central_weight_factor;
			for(int n = 0; n < num_neighbours; n++){
				current_score += (evaluations_history[neighbours_index[n]].score * evaluations_history[neighbours_index[n]].num_repeats);
				sum_weights += evaluations_history[neighbours_index[n]].num_repeats;
			}
			current_score /= sum_weights;
		}

		//-- END - average score with nearest neighbours

		//save
		evaluations_history[s1].kNN_score = current_score;

		//DEBUG
		//if(current_score < 0.01) printf("KNN WARNING: %1.6f at %d\n",current_score,s1);

		//set best
		if(current_score >= best_score){
			best_score = current_score;
			best_index = s1;
		}

	}

	//debug output
	if(output_detail_level > 0){
		printf("Best %2d-NN: ind %d \t scr %5.2f%% \t weights ", num_neighbours, best_index, best_score*100.0);		
		for(int j = 0; j < evaluations_history_num_param_values; j++)
			printf(" %1.5f",evaluations_history[best_index].param_values[j]);
		printf(" \t no-avg %5.2f%% \t manh_dist %d \n", evaluations_history[best_index].score*100.0, (int)manhattan_distance);
	}

	//free memory
	delete(neighbours_distance);
	delete(neighbours_index);

	//return index of best 
	return best_index;
}

// -----------------------------------------------------------//
// Sigmoid function with weighted parameters

Tom_Function_Approximator::~Tom_Function_Approximator()
{
	if(flag_is_allocated)	Memory_Free();				//clear memory if allocated
	if(flag_is_initialized)	Memory_Free_Constants();	//clear constants if allocated
};

void Tom_Function_Approximator::Memory_Init_Constants()
{
	//by definition
	num_actions_per_parameter = 2;	//IMPLEMENTATION FOR 2 ACTIONS PER PARAMETER weight: INCREASE and DECREASE 

}

void Tom_Function_Approximator::Settings_Reset_Private()
{
	num_results = TOM_FUNC_APPROX_DEFAULT_NUM_RESULTS;
	num_parameters_per_result = TOM_FUNC_APPROX_DEFAULT_NUM_PARAM_PER_RESULT;
}

void Tom_Function_Approximator::Memory_Allocate()
{
	
	num_actions = num_results*num_parameters_per_result*num_actions_per_parameter;
	num_params = num_results*num_parameters_per_result;

	results = new double[num_results];
	results_scalar_factors = new double[num_results];

	for(int i = 0; i < num_results; i++){
		results_scalar_factors[i] = TOM_FUNC_APPROX_DEFAULT_RESULTS_SCALAR_FACT;
	}

	//num_parameters counts BIAS as parameter
	parameter_weights = new double[num_params];
	parameter_weights_change_step = new double[num_params];


	for(int i = 0; i < num_params; i++){
		parameter_weights[i] = TOM_FUNC_APPROX_DEFAULT_INIT_PARAM_WEIGHTS;
		parameter_weights_change_step[i] = TOM_FUNC_APPROX_DEFAULT_PARAM_CHANGE_STEP;
	}
}

void Tom_Function_Approximator::Memory_Free()
{
	delete(parameter_weights);
	delete(parameter_weights_change_step);
	delete(results);
	delete(results_scalar_factors);
}

void Tom_Function_Approximator::Action_Update_Weights(int action, const int output_weights, const bool safety_check)
{
	int param_index, do_increase;

	if(safety_check)
		if(!Validate_Action(action))
			return;

	//IMPLEMENTATION FOR 2 ACTIONS PER PARAMETER weight: INCREASE and DECREASE 

	do_increase = action % 2;

	//increase weight
	if(do_increase){

		param_index = (int)(action/2);
		parameter_weights[param_index] += parameter_weights_change_step[param_index];

	//decrease weight
	}else{

		param_index = (int)((action-1)/2);
		parameter_weights[param_index] -= parameter_weights_change_step[param_index];

	}

	//optional: output weights
	if(output_weights > 0){
		Debug_Display_Weights(output_weights);
	}

}

void Tom_Function_Approximator::Action_Update_Weights(int action, double delta_weight, const int output_weights, const bool safety_check)
{
	int param_index, do_increase;

	if(safety_check)
		if(!Validate_Action(action))
			return;

	//IMPLEMENTATION FOR 2 ACTIONS PER PARAMETER weight: INCREASE and DECREASE 

	do_increase = action % 2;

	//increase weight
	if(do_increase){

		param_index = (int)((action-1)/2);
		parameter_weights[param_index] += delta_weight;

	//decrease weight
	}else{

		param_index = (int)((action)/2);
		parameter_weights[param_index] -= delta_weight;

	}

	//optional: output weights
	if(output_weights > 0){
		Debug_Display_Weights(output_weights);
	}

}

bool Tom_Function_Approximator::Validate_Action(int action)
{
	if((action<0)||(action > num_actions)){
		printf("ERROR: Tom_Function_Approximator:Validate_Action(): Invalid action %d, max %d\n", action, num_actions);
		return false;
	}else{
		return true;
	}
}

void Tom_Function_Approximator::Set_Weights_Change_Step(double new_change_step)
{
	for(int i = 0; i < num_parameters_per_result*num_results; i++){
		parameter_weights_change_step[i] = new_change_step;
	}
}


void Tom_Function_Approximator::Debug_Display_Weights(const int output_detail_level)
{
	if(output_detail_level >= 1){
		if(output_detail_level >= 2)
			printf("\n");

		printf("Function Appox: Weights");

		if(output_detail_level >= 2){
			printf("\n  Param  ");
			for(int i = 0; i < num_parameters_per_result*num_results; i++)
				printf("      P%d",i);
			//printf("    BIAS");
			printf("\n  Weight ");
		}

	}

	for(int i = 0; i < num_parameters_per_result*num_results; i++){
		printf("  %6.2f",parameter_weights[i]);
	}

	if(output_detail_level >= 1){
		printf("\n");
	}
}

void Tom_Function_Approximator::Calculate_Results()
{
	//currently not in use
	//current implementation: just sum the parameter weight to the output result
	for(int i = 0; i < num_results; i++){
		results[i] = 0.0;
		for(int j = 0; j < num_parameters_per_result; j++){
			results[i] += parameter_weights[i*num_parameters_per_result + j];
		}
	}

}