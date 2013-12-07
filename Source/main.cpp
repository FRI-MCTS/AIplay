//include header
#include "main.hpp"

//prototypes
void Hex_Testing();
void Main_Testing();
void ConnectFour_Testing();
void Gomoku_Testing();
void TicTacToe_Testing();
void TicTacToe_Implementation_Test1();
void Go_Testing();
void Benchmark();
void DiffSim_Testing(Game_Engine* diffSim, Player_Engine* players[], string testFileName);
void Lotka_Testing();
void EEG_Testing();
void FileRead_Test();
void AMAF_Testing();
void Param_Impact_Testing_v06();
void AMAF_Testing_Extensive(Game_Engine* game, Player_Engine** players, int simulations, int repeats, int games, const bool disable_sim_set = false);

void FuncApp_test();
void LRP_test_basic();
void LRP_test_linearDW();
void LRP_test_exponentDW();
void LRP_test_linAB_exponentDW(double* score_avg, double* score_avg_last10, double* output_param);

void LRP_test_linAB_exponentDW_FunApp(double* score_avg = NULL, double* score_avg_last10 = NULL, double* last_param_val = NULL, bool force_setting_output = false);
void LRP_test_linAB_exponentDW_FunApp_MulParams(double* score_avg = NULL, double* score_avg_last10 = NULL, double** last_param_val = NULL, bool force_setting_output = false, int num_output_params = 0);

void Tom_Sample_Storage_Test();

void LRP_test_wrapper();
void LRP_test_wrapperMultiPar();

void LRP_improved_v1(double* score_avg = NULL, double* score_avg_last10 = NULL, double** last_param_val = NULL, bool force_setting_output = false, const int set_final_evaluations = 750, double* avg_num_games = NULL, double* final_eval_score = NULL);

//---- WRITE OWN CODE IN THIS PROCEDURE ----
void Main_Testing()
{

#if(!TOM_DEBUG)
	srand((unsigned int)time(NULL));
#endif

	//Benchmark();
	//TicTacToe_Testing();
	//Gomoku_Testing();
	//ConnectFour_Testing();
	//FileRead_Test();
	//Lotka_Testing();
	//EEG_Testing();
	//AMAF_Testing();


	//FuncApp_test();
	//LRP_test_basic();
	//LRP_test_linearDW();
	//LRP_test_exponentDW();
	//LRP_test_linAB_exponentDW();

	//LRP_test_wrapper();
	//LRP_test_linAB_exponentDW_FunApp();

	//LRP_test_linAB_exponentDW_FunApp_MulParams();

	//Param_Impact_Testing_v06();
	//LRP_test_wrapperMultiPar();
	//LRP_test_wrapperMultiPar();


	//LRP_improved_v1();


	//LRP_test_wrapperMultiPar();

	//TicTacToe_Implementation_Test1();
	//Go_Testing();

	
	Hex_Testing();

}

//-----------------------------------------


//Improved LRP search v1 - restart from kNN best average score after a batch of iterations
//kNN is weighted according to the number of repeated evaluations (num_repeats), evaluated sample can be set to a higher weight
void LRP_improved_v1(double* score_avg, double* score_avg_last10, double** last_param_val, bool force_setting_output, const int set_final_evaluations, double* avg_num_games, double* final_eval_score)
{

	
	//konfiguracija tom
	const int		num_simulations_self	= 100;						//number MCTS simulations per move: evaluated player
	const int		num_simulations_opponent	= num_simulations_self;	//number MCTS simulation per move: opponent
	const int		num_games_start		= 100;				//number of games per score output at LRP start
	const int		num_games_end		= num_games_start;	//number of games per score output at LRP end
	const double	min_increase_num_games_fact = 1.0;		//minimal increase factor of games per evaluation if confidence below threshold (is ignored if max_increase_num_games_fact <= 1.0)
	const double	max_increase_num_games_fact	= 1.0;		//maximal increase factor of games per evaluation requested by LRP confidence statistical test (disable by setting <= 1.0)
	const double	start_Cp_self		= 0.2;		//initial Cp: evaluated player
	const double	fixed_CP_opponent	= 0.0;		//initial Cp: opponent
	const double	funcApp_init_weights = 0.0;	//initial function approximator weights (to be optimized by LRP)
	const int		num_LRP_iterations	= 10;		//number of LRP iterations
	const double	dw_start			= 0.200;	//LRP delta weight (change in Cp value): at start
	const double	dw_limit			= 0.0005;	//LRP delta weight (change in Cp value): at end
	const double	lrp_ab_max			= 0.75;		//LRP learning parameter alpha: maximum value
	const double	lrp_ab_min			= 0.02;		//LRP learning parameter alpha: minmum value
	const double	lrp_ab_dscore_min	= 1.0 / (double)((num_games_start+num_games_end)/2.0);	//LRP minimum delta score (to change probabilites with minimum alpha value)
	const double	lrp_ab_dscore_max	= lrp_ab_dscore_min * 10.0;	//MUST BE HIGHER OR EQUAL TO lrp_ab_dscore_min
	const double	lrp_b_koef			= 1;		//LRP learning parameter beta koeficient [0,1] of alpha value
	const bool		exp_dw_decrease		= 0;		//use exponential weight decrease (instead of linear)
	const int		eval_player_position	= 1;		//which player to optimize and evaluate 0 or 1, currently works only for two-player games
	const int		final_evaluation_num_games = 100;	//number of games to evaluate final combination of parameters
														//20000, 95% confidence that true value deviates less by 1%
														//5000,  95% confidence that true value deviates less by 2%
														//1200,  95% confidence that true value deviates less by 4%
														//750,   95% confidence that true value deviates less by 5% (default setting)
														//200,	 95% confidence that true value deviates less by 10%
	const bool		LRP_output_best_kNN	= true;					//if true final param values will be set according to kNN best avg, otherwise final param values are left from the last LRP iteration (true is default setting)
	const bool		KNN_weight_repeats	= true;					//LRP best neighbour by kNN weight by number of repeats per sample (true is default setting)
	const double	LRP_best_neighbour_weight_central = 1.1;	//scalar factor of central (observed) sample weight when averaging with kNN	(1.1 is default setting, 1.0 means equal weight to other samples)

	//number of LRP_iterations between restarts from best position in parameter space (disable by setting to num_LRP_iterations or 0)	
	//const int		LRP_restart_condition_iterations = 0;	//never restart
	const int		LRP_restart_condition_iterations = (int)(sqrt(num_LRP_iterations) + 0.5);	//sqare root of LRP iterations (default setting)
	//const int		LRP_restart_condition_iterations = (int)(num_LRP_iterations/10.0 + 0.5);	//linear to numer of LRP iterations
	
	//user-defined equations for LRP_best_neighbours_num: number of nearest neighbours when searching combination of parameters with best average score
	const int		LRP_best_neighbours_num = (int)(min( (sqrt(num_LRP_iterations/10.0 * 1000.0/((num_games_start+num_games_end)/2.0)))*0.5 , num_LRP_iterations*0.1*LRP_best_neighbour_weight_central) + 0.5);	//sqare root of num LRP iterations and games (default setting)
	//const int		LRP_best_neighbours_num = (int)(min( (sqrt(num_LRP_iterations/20.0 * 1000.0/((num_games_start+num_games_end)/2.0)))*0.5 , num_LRP_iterations*0.1*LRP_best_neighbour_weight_central) + 0.5);	//less KNN, sqare root of num LRP iterations and games
	//const int		LRP_best_neighbours_num = (int)(min( (sqrt(num_LRP_iterations/10.0)*(100.0/((num_games_start+num_games_end)/2.0))) , num_LRP_iterations*0.1*0.5*LRP_best_neighbour_weight_central) + 0.5);	//sqare root of num LRP iterations, linear in games	
	
	//restart from best only if current score differs from best by more than this factor, if (1-current/best >= factor) then restart) (0.0 restarts always, also if last is best), is ignored if LRP_restart_condition_iterations == 0
	//const double	LRP_restart_condition_score_rel_diff = 0.0;	//always restart
	const double	LRP_restart_condition_score_rel_diff = 0.1 * 1/sqrt(LRP_best_neighbours_num) * LRP_best_neighbour_weight_central;	//(default setting)
	const double	LRP_restart_condition_score_rel_diff_end = 0.2 * LRP_restart_condition_score_rel_diff;	//the factor decreases linearly by the number of iterations down to this value

	//confidence interval factor for assessing if two scores belong to different distributions
	const bool		LRP_change_AB_by_confidence = true;		//change LRP alpha-beta parameters proportionally to confidence between scores (true, default setting) or to absolute difference (false)
	const bool		LRP_change_DW_by_confidence = false;	//NOT YET IMPLEMENTED: change LRP delta weight proportionally to confidence between scores (true) or linearly/exponentially between dw_start and dw_limit (false, default setting)
	const double	lrp_score_diff_conf_factor = 1.44;		//(1.44 <- 85% is default setting)
							//99% -> 2.58
							//95% -> 1.96
							//90% -> 1.645
							//85% -> 1.44
							//80% -> 1.28
							//75% -> 1.15
							//70% -> 1.04
	
	//THE NUMBER OF Func_App_UCT_Params must be set in Player_AI_UCT_TomTest.hpp with the constant TOMPLAYER_AI_UCT_TOMTEST_FUNC_APPROX_NUM_PARAMS


	//TODO1: da ce je slabši rezultat, da razveljaviš in daš na prejšnje stanje, parameter pa ni treba dat celo pot nazaj ampak samo do doloèene mere
	
	//TODO2 (veè dela): preizkusi èe bi dva loèena LRPja delovala boljše za iskanje 2 parametrov, 2 varianti:
	//			*oba LRPja istoèasno izvedeta akcijo in opazujeta izhod
	//			*LRPja izmenièno izvajata akcije (to pomeni, da je na razpolago pol manj korakov za posamezen LRP)

	const bool show_full_output = true; //runtime setting

	//Game_Engine* game = new Game_Gomoku();
	Game_Engine* game = new Game_ConnectFour();
	//Game_Engine* game = new Game_TicTacToe();

	Player_AI_UCT_TomTest* optimizingPlayer = new Player_AI_UCT_TomTest(game);
	Player_AI_UCT* opponent = new Player_AI_UCT(game);

	Tom_Function_Approximator* funcApp1 = new Tom_Function_Approximator();
	funcApp1->Initialize();
	funcApp1->num_results = optimizingPlayer->Func_App_UCT_num_params;
	funcApp1->Settings_Apply_New();

	optimizingPlayer->Func_App_UCT_Params = funcApp1;

	//optimizingPlayer->UCT_param_C = start_Cp_self;
	optimizingPlayer->UCT_param_C = start_Cp_self;

	for(int i = 0; i < funcApp1->num_params; i++)
		funcApp1->parameter_weights[i] = funcApp_init_weights;
	//funcApp1->parameter_weights[0] = 0.1;
	//funcApp1->parameter_weights[1] = 1.5;

	//-------- end of settings -------- //

	bool silence_output = ((score_avg != NULL)||(score_avg_last10 != NULL)||(last_param_val != NULL));

	//apply settings and initialize structurs
	optimizingPlayer->UCT_param_IterNum = num_simulations_self;
	opponent->UCT_param_IterNum = num_simulations_opponent;
	opponent->UCT_param_C = fixed_CP_opponent;
	
	Player_Engine* players[2];
	players[eval_player_position] = optimizingPlayer;
	players[1-eval_player_position] = opponent;

	Tom_Lrp* Lrp = new Tom_Lrp();
	Lrp->Initialize();
	Lrp->num_actions = funcApp1->num_actions;
	Lrp->Settings_Apply_New();
	Lrp->Evaluations_History_Allocate(num_LRP_iterations+1, funcApp1->num_params);

	//results storage
	Tom_Sample_Storage<double>* score_history = new Tom_Sample_Storage<double>(num_LRP_iterations+1);

	srand((unsigned int)time(NULL));
	double cpu_time = getCPUTime();
	double cpu_time1;

	//calculate exponential time constant
	double dw_tau		= (double) num_LRP_iterations / (log(dw_start/dw_limit));

	//output test settings
	if((!silence_output)||(force_setting_output)){
		printf("LRP_improved_v1()\n");
		printf("%s\n",game->game_name.c_str());
		printf("Players: "); for(int i = 0; i < 2; i++) printf("%s \t",players[i]->player_name.c_str()); printf("\n");
		printf("Evaluated_player %d  sim %d  start_Cp %1.3f  par_w_init %1.3f\n",eval_player_position+1, num_simulations_self, start_Cp_self, funcApp_init_weights);
		printf("Opponent_player  %d  sim %d  fixed_Cp %3.3f\n",2-eval_player_position, num_simulations_opponent, fixed_CP_opponent);
		printf("LRP_steps %5d  games %d %d\n",num_LRP_iterations, num_games_start, num_games_end);
		printf("LRP_restart %4d  restart_threshold %5.3f %5.3f\n",LRP_restart_condition_iterations, LRP_restart_condition_score_rel_diff, LRP_restart_condition_score_rel_diff_end);
		printf("kNN %2d  central_weight %3.1f  weight_repeats %d\n",LRP_best_neighbours_num,LRP_best_neighbour_weight_central, (int)KNN_weight_repeats);
		printf("enable_output_best_kNN %d  final_eval_games %d\n", (int)LRP_output_best_kNN, final_evaluation_num_games);
		printf("Score_conf_test %4.2f\n",lrp_score_diff_conf_factor);
		printf("Games_by_conf_fact %3.1f %3.1f\n",min_increase_num_games_fact,max_increase_num_games_fact);		
		printf("LRP_ab_by_conf %d  LRP_dw_by_conf %d\n",(int)LRP_change_AB_by_confidence,(int)LRP_change_DW_by_confidence);		
		printf("LRP_ab_min %1.3f  LRP_ab_max %1.3f  LRP_b_koef %1.3f\n", lrp_ab_min, lrp_ab_max, lrp_b_koef);			
		printf("lrp_ab_dscore_min %1.3f lrp_ab_dscore_max %1.3f\n",  lrp_ab_dscore_min, lrp_ab_dscore_max);
		printf("Use_linear_dw_decrease %d  dw %1.5f %1.5f  dw_tau %1.5f\n", (int)(!exp_dw_decrease), dw_start, dw_limit, dw_tau);
		printf("\n");
	}

	//--- LRP algorithm ---//

	double score, previous_score, score_tmp, final_score;
	int num_LRP_batch_iterations, num_games_tmp;
	int selected_action;
	int index_best_setting;
	int count_restarts, count_restart_checks;
	double confidence_normal_dist1;
	double confidence_normal_dist2;
	double confidence_num_games1, confidence_num_games2;
	int total_games_count;

	double dw = dw_start;
	double lrp_ab = 0.0;
	int num_games = num_games_start;

	//evaluate starting setting
	score = game->Evaluate_Players(1,num_games,-1,players,false,eval_player_position) / num_games;
	score_history->Add_Sample(score);
	Lrp->Evaluations_History_Add_Sample(-1, num_games, score, 0.0, funcApp1->parameter_weights);
	total_games_count = num_games;
		
	//int best_iteration = -1;
	//double best_score = score;
	//double best_paramC = start_c1;

	if(!silence_output){
		if(show_full_output){
			printf("  \t     \t Parameters");
		}
		printf("\ni \t score");
		for(int p = 0; p < funcApp1->num_params; p++)
			printf("\t P%d",p);
		if(show_full_output){
			printf(" \t\t action");
			for(int a = 0; a < Lrp->num_actions; a++)
				printf("\t A%d[%%]",a);
			printf("\t dw \t\t lrp_ab");
			printf("\t Games");
			printf("\t NormDistConf");
		}
		printf("\n%d \t%5.1f%",0,score*100);
		for(int p = 0; p < funcApp1->num_params; p++)
			printf("\t%6.3f",funcApp1->parameter_weights[p]);
		if(show_full_output){
			printf(" \t\t ");
			printf("//");
			Lrp->Debug_Display_Probabilites(0);
			printf(" \t %1.5f \t %1.3f",dw, lrp_ab);
			printf(" \t %4d",num_games);
		}
		printf("\n");
	}

	//debug
	//double debug_scores[num_LRP_iterations] = {0.1 , 0.2 , 0.3 , 0.4 , 0.5};
	//srand(0);

	//Lrp iterations
	num_LRP_batch_iterations = 0;
	count_restarts = 0;
	count_restart_checks = 0;
	for(int i = 0; i < num_LRP_iterations; i++){
		
		if(exp_dw_decrease){
			//exponential iterative decrease of weight change step
			dw = dw_start * exp(-i/dw_tau);
		}else{
			//linear iterative decrease of weight change step
			dw = dw_limit + (dw_start - dw_limit) * (1.0 - (double)i/(num_LRP_iterations-1));
		}
		
		//increase number of games per evalation
		
		num_games = (int)((num_games_start + (num_games_end - num_games_start) * ((double)(i+1)/num_LRP_iterations)) + 0.5);
		
		//Lrp select action
		selected_action = Lrp->Select_Action(TOM_LRP_SELECT_ACTION_CHECK_SUM);

		//apply action
		//if(selected_action == Lrp->num_actions-1){
		//	funcApp1->parameter_weights[0] = 0.2;
		//}else{
		//	funcApp1->parameter_weights[0] = -0.5;
		//}
		funcApp1->Action_Update_Weights(selected_action, dw);
		//optimizingPlayer->UCT_param_C = funcApp1->parameter_weights[0];

		//evaluate new setting and save scores
		previous_score = score;						//save previous score
		score = game->Evaluate_Players(1,num_games,-1,players,false,eval_player_position) / num_games;

		//-- statistical difference test --//
		int stat_run_once = 0;
		while(stat_run_once < 2){
				
			//calculate the confidence, that the last two scores belong to different normal distributions
			confidence_normal_dist1 =
				sqrt(
					(Lrp->evaluations_history[Lrp->evaluations_history_num_samples-2].num_repeats)
					* (score - previous_score) * (score - previous_score)
					/ (previous_score * (1-previous_score) + 0.00001)	// + 0.00001 only to avoid division by 0
					/ 2.0 //optional
				);
			//calculate the confidence, that the last two scores belong to different normal distributions
			confidence_normal_dist2 =
				sqrt(
					(num_games)
					* (score - previous_score) * (score - previous_score)
					/ (score * (1-score) + 0.00001)	// + 0.00001 only to avoid division by 0
					/ 2.0 //optional
				);
			confidence_num_games1 = lrp_score_diff_conf_factor*lrp_score_diff_conf_factor*(previous_score * (1-previous_score)) * 2.0 / ((score - previous_score) * (score - previous_score) + 0.00001);
			confidence_num_games2 = lrp_score_diff_conf_factor*lrp_score_diff_conf_factor*(score * (1-score)) * 2.0 / ((score - previous_score) * (score - previous_score) + 0.00001);
		
			//re-evaluate with higher number of games if confidence below threshold (lrp_score_diff_conf_factor)
			num_games_tmp = (int)(((max_increase_num_games_fact-1.0)*num_games) * (1.0 - min(1.0, max(confidence_normal_dist1, confidence_normal_dist2) / lrp_score_diff_conf_factor)));
			if((num_games_tmp > 0) && (stat_run_once == 0)){										//if confidence below threshold, then repeat player evaluation with higher number of games
				num_games_tmp = (int)max(num_games_tmp, num_games*(min_increase_num_games_fact - 1.0));		//set number of additional games minimum to increase_num_games_thr
				score_tmp = game->Evaluate_Players(1,num_games_tmp,-1,players,false,eval_player_position) / num_games_tmp;	//perform additional evaluations
				score = (score_tmp*num_games_tmp + score*num_games) / (num_games_tmp + num_games);	//update score
				num_games += num_games_tmp;		//update number of evaluation games
				stat_run_once = 1;	//repeat statistical test
			}else{
				stat_run_once = 2;	//do not repeat statistical test
			}

		}
		//-- END - statistical difference test --//

		score_history->Add_Sample(score);
		Lrp->Evaluations_History_Add_Sample(selected_action, num_games, score, 0.0, funcApp1->parameter_weights);
		total_games_count += num_games;

		//score = debug_scores[i]; //DEBUG
		
		if(!silence_output){
			printf("%d \t%5.1f%",i+1,score*100);
			for(int p = 0; p < funcApp1->num_params; p++)
				printf("\t%6.3f",funcApp1->parameter_weights[p]);
			printf(" \t\t ");
		}

		//-- adaptively change LRP learning parameter alpha (and beta) --//
		//proportional to confidence between scores
		if(LRP_change_AB_by_confidence){
		
			lrp_ab = max(lrp_ab_min, lrp_ab_max * min(1.0 , max(confidence_normal_dist1, confidence_normal_dist2) / lrp_score_diff_conf_factor));
			//inverse, for testing only - should perform worse //lrp_ab = lrp_ab_min + lrp_ab_max - max(lrp_ab_min, lrp_ab_max * min(1.0 , max(confidence_normal_dist1, confidence_normal_dist2) / 1.96));
		
		//linearly proportional lrp learning parameter
		}else{
			lrp_ab =
					lrp_ab_min + (lrp_ab_max - lrp_ab_min) * max(0.0 , min(1.0 ,
						(abs(score-previous_score) - lrp_ab_dscore_min) / (lrp_ab_dscore_max - lrp_ab_dscore_min)
					));
		}
		Lrp->learn_param_a = lrp_ab;
		Lrp->learn_param_b = lrp_ab * lrp_b_koef;
		//-- END - adaptively change LRP learning parameter alpha (and beta) --//

		//Lrp correct probabilites based on previous and current score
		if(score > previous_score){
			Lrp->Update_Probabilites_Reward(selected_action,Lrp->learn_param_a);
			if(!silence_output) if(show_full_output) printf("+");
		}else{
			Lrp->Update_Probabilites_Penalty(selected_action,Lrp->learn_param_b);
			if(!silence_output) if(show_full_output) printf("-");
		}

		//output
		if(!silence_output){
			if(show_full_output){
				printf("%d",selected_action);
				Lrp->Debug_Display_Probabilites(0);
				printf(" \t %1.5f \t %1.3f",dw, lrp_ab);
				printf(" \t %4d",num_games);
				printf(" \t %5.2f %5.2f %4.0f %4.0f",confidence_normal_dist1, confidence_normal_dist2, confidence_num_games1, confidence_num_games2);
			}
		}
		//remember best
		//if(score >= best_score){
		//	best_iteration = i;
		//	best_score = score;
		//	best_paramC = playerUCT1->UCT_param_C;
		//}

		//Restart from best position periodically after LRP_restart_condition_iterations iterations
		num_LRP_batch_iterations++;
		if( (LRP_restart_condition_iterations > 0) && (num_LRP_batch_iterations >= LRP_restart_condition_iterations) ){
			
			//calculate kNN scores and find best parameter setting
			index_best_setting = Lrp->Evaluations_History_Best_KNN_Average_Slow(LRP_best_neighbours_num, KNN_weight_repeats, LRP_best_neighbour_weight_central, false, 0);

			//check difference between scores of last sample (current state) and best score, threshold decreases linearly (by increasing number of iterations, it is more likely to restart from best position)
			double last_sample_score = Lrp->evaluations_history[(Lrp->evaluations_history_num_samples)-1].kNN_score;
			double best_sample_score = Lrp->evaluations_history[index_best_setting].kNN_score;
			double current_score_ratio;
			if(best_sample_score != 0.0)	//division safety
				current_score_ratio = 1.0 - last_sample_score/best_sample_score;
			else
				current_score_ratio = 0.0;
			double restart_threshold = LRP_restart_condition_score_rel_diff_end + (LRP_restart_condition_score_rel_diff-LRP_restart_condition_score_rel_diff_end) * (1.0 - (double)i/(num_LRP_iterations-1));

			//DEBUG
			//printf("\n"); Lrp->Debug_Display_Evaluations_History();
			//printf("\t %d %f %d %f\t",index_best_setting, best_sample_score, (Lrp->evaluations_history_num_samples)-1, last_sample_score);

			if( current_score_ratio >= restart_threshold ){

				if(!silence_output){
					printf(" \t RESTART %3.3f", current_score_ratio );
				}

				//--do execute restart from best position--

				//reset LRP probabilites
				Lrp->State_Reset();

				//set parameter values to best combination found so far
				for(int p = 0; p < funcApp1->num_params; p++)
					funcApp1->parameter_weights[p] = Lrp->evaluations_history[index_best_setting].param_values[p];

				//set appropriate score to compare in next iteration
				score = Lrp->evaluations_history[index_best_setting].score;
			
				//increase counter of restarts
				count_restarts++;
			}else{
				if(!silence_output){
					printf(" \t CHECK %3.3f", current_score_ratio );
				}
			}

			num_LRP_batch_iterations = 0;
			count_restart_checks++;
		}

		if(!silence_output){
			printf("\n");
		}
	}


	//output best parameter combination from history instead of values in last iteration
	if(LRP_output_best_kNN){
		index_best_setting = Lrp->Evaluations_History_Best_KNN_Average_Slow(LRP_best_neighbours_num, KNN_weight_repeats, LRP_best_neighbour_weight_central, false, 0);
		for(int p = 0; p < funcApp1->num_params; p++)
			funcApp1->parameter_weights[p] = Lrp->evaluations_history[index_best_setting].param_values[p];
	}

	//--- LRP algorithm END---//

	cpu_time = getCPUTime()-cpu_time;
	
	//evaluation of final parameters
	cpu_time1 = getCPUTime();
	final_score = game->Evaluate_Players(1,final_evaluation_num_games,-1,players,false,eval_player_position) / final_evaluation_num_games;
	cpu_time1 = getCPUTime()-cpu_time;

	//output best
	//printf("\nBest score: %5.1f% \t P1->Cp  %5.3f \t i  %d\n", best_score*100, best_paramC, best_iteration+1);

	//output values
	score_history->Calc_AvgDev();
	double score10 = score_history->Calc_Avg_On_Interval((int)(num_LRP_iterations*0.9), num_LRP_iterations);
	if(!silence_output){

		double final_eval_dev = sqrt(1.96*1.96*final_score*(1-final_score)*2.0 / final_evaluation_num_games);

		printf("\navgGam \t avg100\t avg10 \t fEval",final_evaluation_num_games);
		for(int p = 0; p < funcApp1->num_params; p++)
			printf("\t FinP%d",p);
		printf("\t fEvaConf95");
		printf("\n%6.1f \t %5.2f \t %5.2f \t %5.2f", (double)total_games_count / (num_LRP_iterations+1), score_history->avg*100, score10*100, final_score*100);
		for(int p = 0; p < funcApp1->num_params; p++)
			printf("\t%7.4f",funcApp1->parameter_weights[p]);
		printf("\t %5.2f",final_eval_dev*100);

		printf("\n\nRuntime [s]: Lrp %9.3f \t Eval %9.3f \t Total %9.3f\n", cpu_time, cpu_time1, cpu_time+cpu_time1);

		//output LRP evaluations history
		printf("\n");
		//Lrp->Debug_Display_Evaluations_History();
		Lrp->Evaluations_History_Best_Single();
		Lrp->Evaluations_History_Best_KNN_Average_Slow(LRP_best_neighbours_num, true, LRP_best_neighbour_weight_central, false);
	}


	//DEBUG
	//printf("RR: %5.2f%% \t ",(double)count_restarts/count_restart_checks*100.0);	//output restart rate
	//printf("ratio p0: %3.3f\n",optimizingPlayer->debug_dbl_cnt1 / optimizingPlayer->debug_dbl_cnt2 *100.0);	//2-interval C search ratio of use p0 to (p0+p1)

	//return values
	if(score_avg != NULL)
		(*score_avg) = score_history->Calc_Avg();
	if(last_param_val != NULL)
		for(int i = 0; i < funcApp1->num_params; i++)
			(*last_param_val)[i] = funcApp1->parameter_weights[i];
	if(score_avg_last10 != NULL)
		(*score_avg_last10) = score_history->Calc_Avg_On_Interval((int)(num_LRP_iterations*0.9), num_LRP_iterations);
	if(avg_num_games != NULL)
		(*avg_num_games) = (double)total_games_count / (num_LRP_iterations + 1);
	if(final_eval_score != NULL)
		(*final_eval_score) = final_score;

	//free memory
	delete(score_history);
	delete(Lrp);
	delete(optimizingPlayer);
	delete(opponent);
	delete(game);
}

void LRP_test_wrapperMultiPar()
{

	const int num_repeats = 100;					//number of LRP repeats (not STEPS!)
	const int num_LRP_params = TOMPLAYER_AI_UCT_TOMTEST_FUNC_APPROX_NUM_PARAMS;			//the number of weights set by LRP
	const int num_final_evaluations = 5000;	//number of games to evaluate final combination of parameters (default 750)
												//20000, 95% confidence that true value deviates less by 1%
												//5000,  95% confidence that true value deviates less by 2%
												//1200,  95% confidence that true value deviates less by 4%
												//750,   95% confidence that true value deviates less by 5%
												//200,	 95% confidence that true value deviates less by 10%
	// --- settings end --- //

	double score_avg;
	double score_avg_last10;
	double* last_param_val = new double[num_LRP_params];
	double avg_num_games;
	double final_eval_score;

	bool output_settings_once = true;

	double cpu_time = getCPUTime();

	Tom_Sample_Storage<double>* avg_score = new Tom_Sample_Storage<double>(num_repeats);
	Tom_Sample_Storage<double>* avg_score10 = new Tom_Sample_Storage<double>(num_repeats);
	Tom_Sample_Storage<double>* last_param[num_LRP_params];
	for(int p = 0; p < num_LRP_params; p++)		
		last_param[p] = new Tom_Sample_Storage<double>(num_repeats);
	Tom_Sample_Storage<double>* final_score = new Tom_Sample_Storage<double>(num_repeats);
	Tom_Sample_Storage<double>* num_games_per_step = new Tom_Sample_Storage<double>(num_repeats);

	printf("LRP_test_wrapperMultiPar(), %d repeats %d params\n\n", num_repeats, num_LRP_params);

	for(int r = 0; r < num_repeats; r++){
		//LRP_test_linAB_exponentDW_FunApp_MulParams(&score_avg,&score_avg_last10,&last_param_val,output_settings_once,num_LRP_params);
		LRP_improved_v1(&score_avg,&score_avg_last10,&last_param_val,output_settings_once,num_final_evaluations,&avg_num_games,&final_eval_score);

		if(output_settings_once){
			printf("\nRun \t avg \t [%%] \t [%%] \t [%%] \t Final_params\n");
			printf(" \t games\t scr100\t scr10\t final\t "); for(int p = 0; p < num_LRP_params; p++) printf("P%d\t  ",p); printf("\n");
		}

		printf("%d \t%6.1f \t %5.2f \t %5.2f \t %5.2f\t",r,avg_num_games,score_avg*100,score_avg_last10*100,final_eval_score*100); for(int p = 0; p < num_LRP_params; p++) printf("%7.4f  ",last_param_val[p]); printf("\n");
		avg_score->Add_Sample(score_avg);
		avg_score10->Add_Sample(score_avg_last10);
		for(int p = 0; p < num_LRP_params; p++)
			last_param[p]->Add_Sample(last_param_val[p]);
		final_score->Add_Sample(final_eval_score);
		num_games_per_step->Add_Sample(avg_num_games);

		output_settings_once = false;
	}

	avg_score->Calc_AllExceptSum();
	avg_score10->Calc_AllExceptSum();
	for(int p = 0; p < num_LRP_params; p++){
		last_param[p]->Calc_AllExceptSum();
	}
	final_score->Calc_AllExceptSum();
	num_games_per_step->Calc_AllExceptSum();

	printf("\n\n");

	printf("Summary  avg \t [%%] \t [%%] \t [%%] \t Final_params\n");
	printf(" \t games\t scr100\t scr10\t final\t "); for(int p = 0; p < num_LRP_params; p++) printf("P%d\t  ",p); printf("\n");
	printf("avg \t%6.1f\t %5.2f\t %5.2f\t %5.2f\t", num_games_per_step->avg, avg_score->avg*100, avg_score10->avg*100, final_score->avg*100); for(int p = 0; p < num_LRP_params; p++) printf("%7.4f  ", last_param[p]->avg); printf("\n");
	printf("dev \t%6.1f\t %5.2f\t %5.2f\t %5.2f\t", num_games_per_step->dev, avg_score->dev*100, avg_score10->dev*100, final_score->dev*100); for(int p = 0; p < num_LRP_params; p++) printf("%7.4f  ", last_param[p]->dev); printf("\n");
	printf("conf \t%6.1f\t %5.2f\t %5.2f\t %5.2f\t", num_games_per_step->conf, avg_score->conf*100, avg_score10->conf*100, final_score->conf*100); for(int p = 0; p < num_LRP_params; p++) printf("%7.4f  ", last_param[p]->conf); printf("\n");
	printf("median \t%6.1f\t %5.2f\t %5.2f\t %5.2f\t", num_games_per_step->median, avg_score->median*100, avg_score10->median*100, final_score->median*100); for(int p = 0; p < num_LRP_params; p++) printf("%7.4f  ", last_param[p]->median); printf("\n");

	cpu_time = getCPUTime()-cpu_time;
	printf("\n\nTotalRuntime: %9.3f s\n", cpu_time);

	
	delete(last_param_val);
	delete(avg_score);
	delete(avg_score10);
	for(int p = 0; p < num_LRP_params; p++)
		delete(last_param[p]);
}

void Param_Impact_Testing_v06()
{
	//game
	Game_Engine* ticTac = new Game_ConnectFour();
	//Game_Engine* ticTac = new Game_Gomoku();
	//Game_Engine* ticTac = new Game_TicTacToe();

	Player_Engine* players[2];
//	Player_AI_UCT* pUCT;

	//opponent
	//Player_Random* opponent = new Player_Random(ticTac);
	Player_AI_UCT* opponent = new Player_AI_UCT(ticTac);
	
	//choose test learning algorithms
	Player_Engine* pUCTlist[] =		{
										//new Player_AI_UCT(ticTac)
										new Player_AI_UCT_TomTest(ticTac)
										//new Player_AI_UCT(ticTac)//,
										//new Player_AI_UCT_AMAF(ticTac)
									};

	Tom_Function_Approximator* funcApp1 = new Tom_Function_Approximator();
	funcApp1->Initialize();
	funcApp1->num_results = 2;
	funcApp1->Settings_Apply_New();
	funcApp1->parameter_weights[0] = 0.185;
	funcApp1->parameter_weights[1] = 0.40;

	int test_num_algorithms = sizeof(pUCTlist) / sizeof(Player_AI_UCT*);


	//set multiple test configuration
	int test_base_val_iterNum = 100;
	int test_base_val_maxPlys = 0;
	double test_base_val_paramC = 1.0;// TOMPLAYER_AI_UCT_PARAM_C;
	
	int test_comb_num_SimNum = 1;
	int test_comb_num_maxPlys = 1;
	int test_comb_num_paramC = 1;

	double test_fact_SimNum = 1.0;
	double test_fact_maxPlys = 1.0;
	double test_fact_paramC = 1.0;
	
	int runs = 1;
	int base_repeats = test_base_val_iterNum*100000; //10*1000*1000;	//MUST BE AT LEAST EQUAL TO test_base_val_iterNum

	opponent->UCT_param_IterNum = test_base_val_iterNum;
	opponent->UCT_param_C = 0.0;//0.5/sqrt(2);

	int eval_player_position = 1;	//which player to test: 0 or 1; works only for two-player games

	//---execute testing---

	players[1-eval_player_position] = opponent;

	//run tests
	if(base_repeats < test_base_val_iterNum){
		base_repeats = test_base_val_iterNum;
	}
	double cpu_time = getCPUTime();

	//setup result storage
	Tom_Sample_Storage<double>* score[2];

	//printf("Param_Impact_Testing()\n");
	printf("%s repeats %d\n",(ticTac->game_name).c_str(), base_repeats / test_base_val_iterNum);
	printf("Evaluated player %d sim %d\n",eval_player_position+1, test_base_val_iterNum);
	printf("Opponent C %3.3f, sim %d\n",opponent->UCT_param_C,opponent->UCT_param_IterNum);
	printf("\n");
	printf("Par \t Score[%%]\n");
	printf("Cp \t Avg \t Dev \t Conf95\n");
	for(int i = 0; i < test_comb_num_SimNum*test_comb_num_maxPlys*test_comb_num_paramC; i++){


		//int tIterNum = (int)(test_base_val_iterNum * pow(test_fact_SimNum , i % test_comb_num_SimNum));
		//int tmaxply = (int)(test_base_val_maxPlys * pow(test_fact_maxPlys , (int)(i/test_comb_num_SimNum) % test_comb_num_maxPlys));
		//double tparamc = test_base_val_paramC * pow(test_fact_paramC , (int)((int)(i/test_comb_num_SimNum)/test_comb_num_maxPlys) % test_comb_num_paramC);
		int tIterNum = test_base_val_iterNum;
		double tparamc = test_base_val_paramC;

		//printf("\nnSim %d  nPly %d  parC %1.3f",tIterNum,tmaxply,tparamc);
		//printf("\n\nProcedure AMAF_param_Testing(): nSim %d  ParC %1.3f\n\n",tIterNum,tparamc);

		for(int ac = 0; ac < test_num_algorithms; ac++){

			((Player_AI_UCT*)pUCTlist[ac])->UCT_param_IterNum = tIterNum;
			((Player_AI_UCT*)pUCTlist[ac])->UCT_param_C = tparamc;
			//((Player_AI_UCT*)players[1-eval_player_position])->UCT_param_C = tparamc;
			players[eval_player_position] = pUCTlist[ac];
			((Player_AI_UCT_TomTest*)(players[eval_player_position]))->Func_App_UCT_Params = funcApp1;

			//AMAF_Testing_Extensive(ticTac, players, tIterNum, runs, base_repeats / tIterNum);
	
			score[0] = new Tom_Sample_Storage<double>(runs*(base_repeats / tIterNum));
			score[1] = new Tom_Sample_Storage<double>(runs*(base_repeats / tIterNum));

			ticTac->Evaluate_Players(runs,(base_repeats / tIterNum),-1,players,false,eval_player_position,score);
			printf("%3.3f \t %5.2f \t %5.2f \t %5.2f\n",tparamc,score[eval_player_position]->avg*100,score[eval_player_position]->dev*100,score[eval_player_position]->Calc_Confidence()*100);

			delete(score[0]);
			delete(score[1]);
		}

		test_base_val_paramC += test_fact_paramC;

	}

	cpu_time = getCPUTime()-cpu_time;
	printf("\n\nProcedure Param_Impact_Testing_v06(): TotalRuntime: %9.3f s\n", cpu_time);
}


void Tom_Sample_Storage_Test()
{
	Tom_Sample_Storage<double>* tmp = new Tom_Sample_Storage<double>(100);

	for(int i = 0; i < 100; i++)
		tmp->Add_Sample(i,true,false);
	printf("s %3.3f\n",tmp->sum);
	printf("a %3.3f\n",tmp->avg);
	printf("d %3.3f\n",tmp->dev);
	printf("c %3.3f\n",tmp->conf);
	printf("\n");
	tmp->Calc_Dev();
	printf("d %3.3f\n",tmp->dev);
	printf("\n");
	tmp->Calc_All();
	printf("s %3.3f\n",tmp->sum);
	printf("a %3.3f\n",tmp->avg);
	printf("d %3.3f\n",tmp->dev);
	printf("c %3.3f\n",tmp->conf);
	printf("\n");
	printf("c %3.3f\n",tmp->Calc_Confidence());
}

void FuncApp_test()
{
	//Tom_Function_Approximator* evalC = new Tom_Function_Approximator();
	//evalC->Initialize();
	//evalC->Debug_Display_Weights(2);
	//evalC->num_parameters = 3;
	//evalC->Settings_Apply_New();
	//evalC->Debug_Display_Weights(2);

	//Tom_Lrp* Lrp = new Tom_Lrp();
	//Lrp->Initialize();
	//Lrp->Debug_Display_Probabilites();
	//Lrp->num_actions = 5;
	//Lrp->Settings_Apply_New();
	//Lrp->Debug_Display_Probabilites(0);
}

void LRP_test_wrapper()
{

	int num_repeats = 100;


	// --- settings end --- //

	double score_avg;
	double score_avg_last10;
	double last_param_val;
	
	bool output_settings_once = true;

	double cpu_time = getCPUTime();

	Tom_Sample_Storage<double>* avg_score = new Tom_Sample_Storage<double>(num_repeats);
	Tom_Sample_Storage<double>* avg_score10 = new Tom_Sample_Storage<double>(num_repeats);
	Tom_Sample_Storage<double>* last_param = new Tom_Sample_Storage<double>(num_repeats);

	printf("LRP_test_wrapper(), %d repeats\n\n", num_repeats);

	for(int r = 0; r < num_repeats; r++){
		//LRP_test_linAB_exponentDW(&score_avg,&score_avg_last10,&last_param_val);
		LRP_test_linAB_exponentDW_FunApp(&score_avg,&score_avg_last10,&last_param_val,output_settings_once);
		
		if(output_settings_once){
			printf("\nRun \t [%%] \t [%%] \t Final_params\n");
			printf(" \t scr100\t scr10 \t P1\n");
		}

		printf("%d \t %5.2f \t %5.2f \t %6.4f\n",r,score_avg*100,score_avg_last10*100,last_param_val);
		avg_score->Add_Sample(score_avg);
		avg_score10->Add_Sample(score_avg_last10);
		last_param->Add_Sample(last_param_val);

		output_settings_once = false;
	}

	avg_score->Calc_AvgDev(); avg_score->Calc_Confidence();
	avg_score10->Calc_AvgDev(); avg_score10->Calc_Confidence();
	last_param->Calc_AvgDev(); last_param->Calc_Confidence();
	
	printf("\n\n");

	printf("Summary  [%%] \t [%%] \t Final_params\n");
	printf(" \t scr100\t scr10 \t P1\n");
	printf("avg \t %5.2f\t %5.2f\t %6.4f\n", avg_score->avg*100, avg_score10->avg*100, last_param->avg);
	printf("dev \t %5.2f\t %5.2f\t %6.4f\n", avg_score->dev*100, avg_score10->dev*100, last_param->dev);
	printf("conf \t %5.2f\t %5.2f\t %6.4f\n", avg_score->conf*100, avg_score10->conf*100, last_param->conf);

	cpu_time = getCPUTime()-cpu_time;
	printf("\n\nTotalRuntime: %9.3f s\n", cpu_time);
}

//LRP learning with Tom_Function_Approximator object
//  with proportionally linearly changing learning parameters a and b
//  with exponentialy decreasing weight change step
void LRP_test_linAB_exponentDW_FunApp_MulParams(double* score_avg, double* score_avg_last10, double** last_param_val, bool force_setting_output, int num_output_params)
{
	//konfiguracija brane
	//const int num_simulations_self	= 30;						//number MCTS simulations per move: evaluated player
	//const int num_simulations_opponent	= num_simulations_self;	//number MCTS simulation per move: opponent
	//const int num_games				= 1000;		//number of games per score output
	//const double start_Cp_self		= 0.0;		//initial Cp: evaluated player
	//const double fixed_CP_opponent	= 0.0;		//initial Cp: opponent
	//const int num_LRP_iterations	= 100;		//number of LRP iterations
	//const double dw_start			= 0.05;	//LRP delta weight (change in Cp value): at start
	//const double dw_limit			= 0.05/20;	//LRP delta weight (change in Cp value): at end
	//const double lrp_ab_max			= 0.20;		//LRP learning parameter alpha: maximum value
	//const double lrp_ab_min			= 0.20;		//LRP learning parameter alpha: minmum value
	//const double lrp_ab_dscore_min	= 1.0 / (double)num_games;	//LRP minimum delta score (to change probabilites with minimum alpha alpha value)
	//const double lrp_ab_dscore_max	= lrp_ab_dscore_min * 10.0;	//MUST BE HIGHER OR EQUAL TO lrp_ab_dscore_min
	//const double lrp_b_koef			= 1.0;		//LRP learning parameter beta koeficient [0,1] of alpha value
	//const bool exp_dw_decrease		= 1;		//use exponential weight decrease (instead of linear)
	//const int eval_player_position	= 1;			//which player to optimize and evaluate 0 or 1, currently works only for two-player games

	//konfiguracija tom
	const int num_simulations_self	= 100;						//number MCTS simulations per move: evaluated player
	const int num_simulations_opponent	= num_simulations_self;	//number MCTS simulation per move: opponent
	const int num_games				= 100;		//number of games per score output
	const double start_Cp_self		= 1.0;		//initial Cp: evaluated player
	const double fixed_CP_opponent	= 0.0;		//initial Cp: opponent
	const int num_LRP_iterations	= 1000;		//number of LRP iterations
	const double dw_start			= 0.050;	//LRP delta weight (change in Cp value): at start
	const double dw_limit			= 0.002;	//LRP delta weight (change in Cp value): at end
	const double lrp_ab_max			= 0.40;		//LRP learning parameter alpha: maximum value
	const double lrp_ab_min			= 0.02;		//LRP learning parameter alpha: minmum value
	const double lrp_ab_dscore_min	= 1.0 / (double)num_games;	//LRP minimum delta score (to change probabilites with minimum alpha alpha value)
	const double lrp_ab_dscore_max	= lrp_ab_dscore_min * 10.0;	//MUST BE HIGHER OR EQUAL TO lrp_ab_dscore_min
	const double lrp_b_koef			= 1;		//LRP learning parameter beta koeficient [0,1] of alpha value
	const bool exp_dw_decrease		= 0;		//use exponential weight decrease (instead of linear)
	const int eval_player_position	= 1;			//which player to optimize and evaluate 0 or 1, currently works only for two-player games


	//TODO1: da ce je slabši rezultat, da razveljaviš in daš na prejšnje stanje, parameter pa ni treba dat celo pot nazaj ampak samo do doloèene mere
	//TODO2: preizkusi èe bi dva loèena LRPja delovala boljše za iskanje 2 parametrov, 2 varianti:
	//			*oba LRPja istoèasno izvedeta akcijo in opazujeta izhod
	//			*LRPja izmenièno izvajata akcije (to pomeni, da je na razpolago pol manj korakov za posamezen LRP)

	const bool show_full_output = true; //runtime setting

	//Game_Engine* game = new Game_Gomoku();
	Game_Engine* game = new Game_ConnectFour();
	//Game_Engine* game = new Game_TicTacToe();

	Player_AI_UCT_TomTest* optimizingPlayer = new Player_AI_UCT_TomTest(game);
	Player_AI_UCT* opponent = new Player_AI_UCT(game);

	Tom_Function_Approximator* funcApp1 = new Tom_Function_Approximator();
	funcApp1->Initialize();
	if(num_output_params == 0){
		funcApp1->num_results = 2;
	}else{
		funcApp1->num_results = num_output_params;
	}
	funcApp1->Settings_Apply_New();

	optimizingPlayer->Func_App_UCT_Params = funcApp1;

	//optimizingPlayer->UCT_param_C = start_Cp_self;
	optimizingPlayer->UCT_param_C = 1.0;

	//-------- end of settings -------- //

	bool silence_output = ((score_avg != NULL)||(score_avg_last10 != NULL)||(last_param_val != NULL));

	//apply settings and initialize structurs
	optimizingPlayer->UCT_param_IterNum = num_simulations_self;
	opponent->UCT_param_IterNum = num_simulations_opponent;
	opponent->UCT_param_C = fixed_CP_opponent;

	for(int i = 0; i < funcApp1->num_params; i++)
		funcApp1->parameter_weights[i] = start_Cp_self;
	
	Player_Engine* players[2];
	players[eval_player_position] = optimizingPlayer;
	players[1-eval_player_position] = opponent;

	Tom_Lrp* Lrp = new Tom_Lrp();
	Lrp->Initialize();
	Lrp->num_actions = funcApp1->num_actions;
	Lrp->Settings_Apply_New();

	//results storage
	Tom_Sample_Storage<double>* score_history = new Tom_Sample_Storage<double>(num_LRP_iterations);

	srand((unsigned int)time(NULL));
	double cpu_time = getCPUTime();

	//calculate exponential time constant
	double dw_tau		= (double) num_LRP_iterations / (log(dw_start/dw_limit));

	//output test settings
	if((!silence_output)||(force_setting_output)){
		printf("LRP_test_linAB_exponentDW_FunApp_MulParams()\n");
		printf("%s\n",game->game_name.c_str());
		printf("Evaluated player %d sim %d start Cp %1.3f\n",eval_player_position+1, num_simulations_self, start_Cp_self);
		printf("Opponent C %3.3f, sim %d\n",fixed_CP_opponent,num_simulations_opponent);
		printf("Games %d LRP_iter %5d\n", num_games, num_LRP_iterations);
		printf("\n");
		printf("LRP_ab_min %1.3f LRP_ab_max %1.3f dScoreMin %1.3f dScoreMax %1.3f\n",  lrp_ab_min, lrp_ab_max, lrp_ab_dscore_min, lrp_ab_dscore_max);
		printf("LRP_b_koef %1.3f\n",  lrp_b_koef);
		printf("dw_start %1.5f  dw_limit %1.5f  dw_tau %1.5f\n", dw_start, dw_limit, dw_tau);
		printf("Use linear dw decrease: %d\n", (int)(!exp_dw_decrease));
		printf("\n");
	}

	//--- LRP algorithm ---//

	double score, previous_score;
	int selected_action;

	double dw = dw_start;
	double lrp_ab = 0.0;

	//evaluate starting setting
	score = game->Evaluate_Players(1,num_games,-1,players,false,eval_player_position) / num_games;
	
	//int best_iteration = -1;
	//double best_score = score;
	//double best_paramC = start_c1;

	if(!silence_output){
		if(show_full_output){
			printf("  \t     \t Parameters");
		}
		printf("\ni \t score");
		for(int p = 0; p < funcApp1->num_params; p++)
			printf("\t P%d",p);
		if(show_full_output){
			printf(" \t\t action");
			for(int a = 0; a < Lrp->num_actions; a++)
				printf("\t A%d[%%]",a);
			printf("\t dw \t\t lrp_ab");
		}
		printf("\n%d \t%5.1f%%",0,score*100);
		for(int p = 0; p < funcApp1->num_params; p++)
			printf("\t %5.3f",funcApp1->parameter_weights[p]);
		if(show_full_output){
			printf(" \t\t ");
			printf("//");
			Lrp->Debug_Display_Probabilites(0);
			printf(" \t %1.5f \t %1.3f",dw, lrp_ab);
		}
		printf("\n");
	}

	//debug
	//double debug_scores[num_LRP_iterations] = {0.1 , 0.2 , 0.3 , 0.4 , 0.5};
	//srand(0);

	//Lrp iterations
	for(int i = 0; i < num_LRP_iterations; i++){

		if(exp_dw_decrease){
			//exponential iterative decrease of weight change step
			dw = dw_start * exp(-i/dw_tau);
		}else{
			//linear iterative decrease of weight change step
			dw = dw_limit + (dw_start - dw_limit) * (1.0 - (double)i/(num_LRP_iterations-1));
		}
		
		//Lrp select action
		selected_action = Lrp->Select_Action(TOM_LRP_SELECT_ACTION_CHECK_SUM);

		//apply action
		//if(selected_action == Lrp->num_actions-1){
		//	funcApp1->parameter_weights[0] = 0.2;
		//}else{
		//	funcApp1->parameter_weights[0] = -0.5;
		//}
		funcApp1->Action_Update_Weights(selected_action, dw);
		//optimizingPlayer->UCT_param_C = funcApp1->parameter_weights[0];

		//evaluate new setting
		previous_score = score;		//save previous score
		score = game->Evaluate_Players(1,num_games,-1,players,false,eval_player_position) / num_games;
		//score = debug_scores[i]; //DEBUG
		
		if(!silence_output){
			printf("%d \t%5.1f%%",i+1,score*100);
			for(int p = 0; p < funcApp1->num_params; p++)
				printf("\t %5.3f",funcApp1->parameter_weights[p]);
			printf(" \t\t ");
		}

		//linearly proportional lrp learning parameter
		lrp_ab =
				lrp_ab_min + (lrp_ab_max - lrp_ab_min) * max(0.0 , min(1.0 ,
					(abs(score-previous_score) - lrp_ab_dscore_min) / (lrp_ab_dscore_max - lrp_ab_dscore_min)
				));

		Lrp->learn_param_a = lrp_ab;
		Lrp->learn_param_b = lrp_ab * lrp_b_koef;

		//Lrp correct probabilites based on previous and current score
		if(score > previous_score){
			Lrp->Update_Probabilites_Reward(selected_action,Lrp->learn_param_a);
			if(!silence_output) if(show_full_output) printf("+");
		}else{
			Lrp->Update_Probabilites_Penalty(selected_action,Lrp->learn_param_b);
			if(!silence_output) if(show_full_output) printf("-");
		}
		if(!silence_output){
			if(show_full_output){
				printf("%d",selected_action);
				Lrp->Debug_Display_Probabilites(0);
				printf(" \t %1.5f \t %1.3f",dw, lrp_ab);
			}
			printf("\n");
		}
		//remember best
		//if(score >= best_score){
		//	best_iteration = i;
		//	best_score = score;
		//	best_paramC = playerUCT1->UCT_param_C;
		//}
		
		//update score
		score_history->Add_Sample(score);

	}

	//--- LRP algorithm END---//

	cpu_time = getCPUTime()-cpu_time;

	//output best
	//printf("\nBest score: %5.1f%% \t P1->Cp  %5.3f \t i  %d\n", best_score*100, best_paramC, best_iteration+1);
	
	//output values
	score_history->Calc_AvgDev();
	double score10 = score_history->Calc_Avg_On_Interval((int)(num_LRP_iterations*0.9), num_LRP_iterations);
	if(!silence_output){
		printf("\navg100 dev100 \t avg10");
		for(int p = 0; p < funcApp1->num_params; p++)
			printf(" \t FinP%d",p);
		printf("\n");
		printf("%3.2f  %3.2f \t %3.2f",score_history->avg*100, score_history->dev*100, score10*100);
		for(int p = 0; p < funcApp1->num_params; p++)
			printf("\t %3.4f",funcApp1->parameter_weights[p]);
		printf("\n");
		printf("\n\nTotalRuntime: %9.3f s\n", cpu_time);
	}

	//return values
	if(score_avg != NULL)
		(*score_avg) = score_history->Calc_Avg();
	if(last_param_val != NULL)
		for(int i = 0; i < funcApp1->num_params; i++)
			(*last_param_val)[i] = funcApp1->parameter_weights[i];
	if(score_avg_last10 != NULL)
		(*score_avg_last10) = score_history->Calc_Avg_On_Interval((int)(num_LRP_iterations*0.9), num_LRP_iterations);

	//free memory
	delete(score_history);
	delete(Lrp);
	delete(optimizingPlayer);
	delete(opponent);
	delete(game);
}

//LRP learning with Tom_Function_Approximator object
//  with proportionally linearly changing learning parameters a and b
//  with exponentialy decreasing weight change step
void LRP_test_linAB_exponentDW_FunApp(double* score_avg, double* score_avg_last10, double* output_param, bool force_setting_output)
{
	const int num_simulations_self	= 100;
	const int num_games				= 200;
	const int num_LRP_iterations	= 200;
	const double dw_start			= 0.020;
	const double dw_limit			= 0.001;
	const double lrp_ab_min			= 0.02;
	const double lrp_ab_max			= 0.20;
	const double lrp_ab_dscore_min	= 1.0 / (double)num_games;
	const double lrp_ab_dscore_max	= lrp_ab_dscore_min * 10.0;	//MUST BE HIGHER OR EQUAL TO lrp_ab_dscore_min
	const double start_Cp_self		= 0.0;
	const double fixed_CP_opponent	= 0.0;
	const int num_simulations_opponent	= num_simulations_self;

	const int eval_player_position = 1;	//which player to optimize and evaluate 0 or 1, currently works only for two-player games

	const bool show_full_output = true; //runtime setting

	//Game_Engine* game = new Game_Gomoku();
	Game_Engine* game = new Game_ConnectFour();
	//Game_Engine* game = new Game_TicTacToe();

	Player_AI_UCT_TomTest* optimizingPlayer = new Player_AI_UCT_TomTest(game);
	Player_AI_UCT* opponent = new Player_AI_UCT(game);

	Tom_Function_Approximator* funcApp1 = new Tom_Function_Approximator();
	optimizingPlayer->Func_App_UCT_Params = funcApp1;

	//optimizingPlayer->UCT_param_C = start_Cp_self;
	optimizingPlayer->UCT_param_C = 1.0;	//this is the 

	//-------- end of settings -------- //

	bool silence_output = ((score_avg != NULL)||(score_avg_last10 != NULL)||(output_param != NULL));

	//apply settings and initialize structurs
	optimizingPlayer->UCT_param_IterNum = num_simulations_self;
	opponent->UCT_param_IterNum = num_simulations_opponent;
	opponent->UCT_param_C = fixed_CP_opponent;

	funcApp1->Initialize();
	for(int i = 0; i < funcApp1->num_params; i++)
		funcApp1->parameter_weights[i] = start_Cp_self;

	Player_Engine* players[2];
	players[eval_player_position] = optimizingPlayer;
	players[1-eval_player_position] = opponent;

	Tom_Lrp* Lrp = new Tom_Lrp();
	Lrp->Initialize();

	//results storage
	Tom_Sample_Storage<double>* score_history = new Tom_Sample_Storage<double>(num_LRP_iterations);

	srand((unsigned int)time(NULL));
	double cpu_time = getCPUTime();

	//calculate exponential time constant
	double dw_tau		= (double) num_LRP_iterations / (log(dw_start/dw_limit));

	//output test settings
	if((!silence_output)||(force_setting_output)){
		printf("LRP_test_linAB_exponentDW_FunApp()\n");
		printf("%s\n",game->game_name.c_str());
		printf("Evaluated player %d sim %d start Cp %1.3f\n",eval_player_position+1, num_simulations_self, start_Cp_self);
		printf("Opponent C %3.3f, sim %d\n",fixed_CP_opponent,num_simulations_opponent);
		printf("Games %d LRP_iter %5d\n", num_games, num_LRP_iterations);
		printf("\n");
		printf("LRP_ab_min %1.3f LRP_ab_max %1.3f dScoreMin %1.3f dScoreMax %1.3f\n",  lrp_ab_min, lrp_ab_max, lrp_ab_dscore_min, lrp_ab_dscore_max);
		printf("dw_start %1.5f  dw_limit %1.5f  dw_tau %1.5f\n", dw_start, dw_limit, dw_tau);
		printf("\n");
	}

	//--- LRP algorithm ---//

	double score, previous_score;
	int selected_action;

	double dw = dw_start;
	double lrp_ab = 0.0;

	//evaluate starting setting
	score = game->Evaluate_Players(1,num_games,-1,players,false,eval_player_position) / num_games;
	
	//int best_iteration = -1;
	//double best_score = score;
	//double best_paramC = start_c1;

	if(!silence_output){
		if(show_full_output)
			printf("  \t                 \t            pr[%%]  pr[%%]");
		printf("\ni \t score    Cp");
		if(show_full_output){
			printf(" \t\t action     A0     A1 \t\t dw \t\t lrp_ab");
		}
		printf("\n%d \t%5.1f%%    %5.3f \t ",0,score*100,funcApp1->parameter_weights[0]);
		if(show_full_output){
			printf("// \t  ");
			Lrp->Debug_Display_Probabilites(0);
			printf(" \t %1.5f \t %1.3f",dw, lrp_ab);
		}
		printf("\n");
	}

	//Lrp iterations
	for(int i = 0; i < num_LRP_iterations; i++){

		//exponential iterative decrease of weight change step
		dw = dw_start * exp(-i/dw_tau);
		
		//Lrp select action
		selected_action = Lrp->Select_Action(TOM_LRP_SELECT_ACTION_CHECK_SUM);

		//apply action
		funcApp1->Action_Update_Weights(selected_action, dw);
		//optimizingPlayer->UCT_param_C = funcApp1->parameter_weights[0];

		//evaluate new setting
		previous_score = score;		//save previous score
		score = game->Evaluate_Players(1,num_games,-1,players,false,eval_player_position) / num_games;
		if(!silence_output) printf("%d \t%5.1f%%    %5.3f \t ",i+1,score*100,funcApp1->parameter_weights[0]);

		//linearly proportional lrp learning parameter
		lrp_ab =
				lrp_ab_min + (lrp_ab_max - lrp_ab_min) * max(0.0 , min(1.0 ,
					(abs(score-previous_score) - lrp_ab_dscore_min) / (lrp_ab_dscore_max - lrp_ab_dscore_min)
				));

		Lrp->learn_param_a = lrp_ab;
		Lrp->learn_param_b = lrp_ab;

		//Lrp correct probabilites based on previous and current score
		if(score > previous_score){
			Lrp->Update_Probabilites_Reward(selected_action,Lrp->learn_param_a);
			if(!silence_output) if(show_full_output) printf("+");
		}else{
			Lrp->Update_Probabilites_Penalty(selected_action,Lrp->learn_param_b);
			if(!silence_output) if(show_full_output) printf("-");
		}
		if(!silence_output){
			if(show_full_output){
				printf("%d \t  ",selected_action);
				Lrp->Debug_Display_Probabilites(0);
				printf(" \t %1.5f \t %1.3f",dw, lrp_ab);
			}
			printf("\n");
		}
		//remember best
		//if(score >= best_score){
		//	best_iteration = i;
		//	best_score = score;
		//	best_paramC = playerUCT1->UCT_param_C;
		//}
		
		//update score
		score_history->Add_Sample(score);

	}

	//--- LRP algorithm END---//

	cpu_time = getCPUTime()-cpu_time;

	//output best
	//printf("\nBest score: %5.1f%% \t P1->Cp  %5.3f \t i  %d\n", best_score*100, best_paramC, best_iteration+1);
	
	//output values
	score_history->Calc_AvgDev();
	double score10 = score_history->Calc_Avg_On_Interval((int)(num_LRP_iterations*0.9), num_LRP_iterations);
	if(!silence_output){
		printf("\navg100 dev100 \t avg10 \t FinPar\n");
		printf("%3.2f  %3.2f \t %3.2f \t %3.4f\n",score_history->avg*100, score_history->dev*100, score10*100, funcApp1->parameter_weights[0]);
	
		printf("\n\nTotalRuntime: %9.3f s\n", cpu_time);
	}

	//return values
	if(score_avg != NULL)
		(*score_avg) = score_history->Calc_Avg();
	if(output_param != NULL)
		(*output_param) = funcApp1->parameter_weights[0];
	if(score_avg_last10 != NULL)
		(*score_avg_last10) = score_history->Calc_Avg_On_Interval((int)(num_LRP_iterations*0.9), num_LRP_iterations);

	//free memory
	delete(score_history);
	delete(Lrp);
	delete(optimizingPlayer);
	delete(opponent);
	delete(game);
}

//LRP learning
//  with proportionally linearly changing learning parameters a and b
//  with exponentialy decreasing weight change step
void LRP_test_linAB_exponentDW(double* score_avg, double* score_avg_last10, double* output_param)
{
	const int num_simulations		= 100;
	const int num_games				= 200;
	const int num_LRP_iterations	= 100;
	const double dw_start			= 0.020;
	const double dw_limit			= 0.001;
	const double lrp_ab_min			= 0.02;
	const double lrp_ab_max			= 0.20;
	const double lrp_ab_dscore_min	= 1.0 / (double)num_games;
	const double lrp_ab_dscore_max	= lrp_ab_dscore_min * 10.0;	//MUST BE HIGHER OR EQUAL TO lrp_ab_dscore_min
	const double start_c1			= 1.0;
	const double start_c2			= 0.0;

	//Game_Engine* game = new Game_Gomoku();
	Game_Engine* game = new Game_ConnectFour();

	Tom_Lrp* Lrp = new Tom_Lrp();
	Lrp->Initialize();

	Player_AI_UCT* playerUCT1 = new Player_AI_UCT(game);
	Player_AI_UCT* playerUCT2 = new Player_AI_UCT(game);
	Player_Engine* players[] = {playerUCT1,playerUCT2};

	((Player_AI_UCT*) players[0])->UCT_param_IterNum = num_simulations;
	((Player_AI_UCT*) players[1])->UCT_param_IterNum = num_simulations;
	((Player_AI_UCT*) players[0])->UCT_param_C = start_c1;
	((Player_AI_UCT*) players[1])->UCT_param_C = start_c2;

	//results storage
	Tom_Sample_Storage<double>* score_history = new Tom_Sample_Storage<double>(num_LRP_iterations);

	srand((unsigned int)time(NULL));
	double cpu_time = getCPUTime();

	//calculate exponential time constant
	double dw_tau		= (double) num_LRP_iterations / (log(dw_start/dw_limit));

	//output test settings
	//printf("LRP_test_linAB_exponentDW()\n");
	//printf("Test setup:\n");
	//printf("  game %s   sim %4d   games %4d\n",game->game_name.c_str(), num_simulations, num_games);
	//printf("  LRP_iter %5d\n", num_LRP_iterations);
	//printf("  LRP_ab_min %1.3f LRP_ab_max %1.3f dScoreMin %1.3f dScoreMax %1.3f\n",  lrp_ab_min, lrp_ab_max, lrp_ab_dscore_min, lrp_ab_dscore_max);
	//printf("  dw_start %1.5f  dw_limit %1.5f  dw_tau %1.5f\n", dw_start, dw_limit, dw_tau);
	//printf("  c1 %1.3f  c2 %1.3f\n\n", start_c1, start_c2);

	//--- LRP algorithm ---//

	double score, previous_score;
	int selected_action;

	double dw = dw_start;
	double lrp_ab = 0.0;

	//evaluate starting setting
	score = game->Evaluate_Players(1,num_games,-1,players,false,0) / num_games;
	
	int best_iteration = -1;
	double best_score = score;
	double best_paramC = start_c1;

	//printf("  \t                 \t            pr[%%]  pr[%%]\n");
	//printf("i \t score    P1->Cp \t action     A0     A1 \t\t dw \t\t lrp_ab\n");
	//printf("%d \t%5.1f%% ",0,score*100);
	//printf("   %5.3f \t ", playerUCT1->UCT_param_C);
	//printf("// \t  ");
	//Lrp->Debug_Display_Probabilites(0);
	//printf(" \t %1.5f \t %1.3f",dw, lrp_ab);
	//printf("\n");

	//Lrp iterations
	for(int i = 0; i < num_LRP_iterations; i++){

		//exponential iterative decrease of weight change step
		dw = dw_start * exp(-i/dw_tau);
		
		//Lrp select action
		selected_action = Lrp->Select_Action(TOM_LRP_SELECT_ACTION_CHECK_SUM);

		//apply action
		if(selected_action == 0)
			playerUCT1->UCT_param_C += dw;
		else
			playerUCT1->UCT_param_C -= dw;

		//evaluate new setting
		previous_score = score;		//save previous score
		score = game->Evaluate_Players(1,num_games,-1,players,false,0) / num_games;
		//printf("%d \t%5.1f%% ",i+1,score*100);
		//printf("   %5.3f \t ", playerUCT1->UCT_param_C);

		//linearly proportional lrp learning parameter
		lrp_ab =
				lrp_ab_min + (lrp_ab_max - lrp_ab_min) * max(0.0 , min(1.0 ,
					(abs(score-previous_score) - lrp_ab_dscore_min) / (lrp_ab_dscore_max - lrp_ab_dscore_min)
				));

		Lrp->learn_param_a = lrp_ab;
		Lrp->learn_param_b = lrp_ab;

		//Lrp correct probabilites based on previous and current score
		if(score > previous_score){
			Lrp->Update_Probabilites_Reward(selected_action,Lrp->learn_param_a);
			//printf("+");
		}else{
			Lrp->Update_Probabilites_Penalty(selected_action,Lrp->learn_param_b);
			//printf("-");
		}

		//printf("%d \t  ",selected_action);
		//Lrp->Debug_Display_Probabilites(0);
		//
		//printf(" \t %1.5f \t %1.3f",dw, lrp_ab);
		//printf("\n");

		//remember best
		//if(score >= best_score){
		//	best_iteration = i;
		//	best_score = score;
		//	best_paramC = playerUCT1->UCT_param_C;
		//}
		
		//update score
		score_history->Add_Sample(score);

	}

	//--- LRP algorithm END---//

	//output best
	//printf("\nBest score: %5.1f%% \t P1->Cp  %5.3f \t i  %d\n", best_score*100, best_paramC, best_iteration+1);

	cpu_time = getCPUTime()-cpu_time;
	//printf("\n\nTotalRuntime: %9.3f s\n", cpu_time);

	//Lrp->Debug_Display_Probabilites();
	//Lrp->Update_Probabilites_Reward(0,Lrp->learn_param_a);
	//Lrp->Debug_Display_Probabilites();
	//Lrp->Update_Probabilites_Penalty(1,Lrp->learn_param_b);
	//Lrp->Debug_Display_Probabilites();
	//Lrp->State_Reset();
	//Lrp->Debug_Display_Probabilites();
	//delete(Lrp);

	//output values
	(*score_avg) = score_history->Calc_Avg();
	(*output_param) = playerUCT1->UCT_param_C;
	(*score_avg_last10) = score_history->Calc_Avg_On_Interval((int)(num_LRP_iterations*0.9), num_LRP_iterations);

	//free memory
	delete(score_history);
	delete(Lrp);
	delete(playerUCT1);
	delete(playerUCT2);
	delete(game);
}

//LRP learning with exponentially decreasing weight change step
void LRP_test_exponentDW()
{
	int num_simulations		= 100;
	int num_games			= 200;
	int num_LRP_iterations	= 100;
	double dw_start			= 0.05;
	double dw_limit			= 0.001;	
	double lrp_ab			= 0.1;
	double start_c1			= 1.0;
	double start_c2			= 0.0;

	//Game_Engine* game = new Game_Gomoku();
	Game_Engine* game = new Game_ConnectFour();

	Tom_Lrp* Lrp = new Tom_Lrp();
	Lrp->Initialize();

	Player_AI_UCT* playerUCT1 = new Player_AI_UCT(game);
	Player_AI_UCT* playerUCT2 = new Player_AI_UCT(game);
	Player_Engine* players[] = {playerUCT1,playerUCT2};

	((Player_AI_UCT*) players[0])->UCT_param_IterNum = num_simulations;
	((Player_AI_UCT*) players[1])->UCT_param_IterNum = num_simulations;
	((Player_AI_UCT*) players[0])->UCT_param_C = start_c1;
	((Player_AI_UCT*) players[1])->UCT_param_C = start_c2;

	srand((unsigned int)time(NULL));
	double cpu_time = getCPUTime();

	//calculate exponential time constant
	double dw_tau		= (double) num_LRP_iterations / (log(dw_start/dw_limit));

	//output test settings
	printf("LRP_test_exponentDW()\n");
	printf("Test setup:\n");
	printf("  game %s   sim %4d   games %4d\n",game->game_name.c_str(), num_simulations, num_games);
	printf("  LRP_iter %5d  LRP_ab %1.3f\n",  num_LRP_iterations, lrp_ab);
	printf("  dw_start %1.5f  dw_limit %1.5f  dw_tau %1.5f\n", dw_start, dw_limit, dw_tau);
	printf("  c1 %1.3f  c2 %1.3f\n\n", start_c1, start_c2);

	Lrp->learn_param_a = lrp_ab;
	Lrp->learn_param_b = lrp_ab;

	//--- LRP algorithm ---//

	double score, previous_score;
	int selected_action;

	double dw = dw_start;

	//evaluate starting setting
	score = game->Evaluate_Players(1,num_games,-1,players) / num_games;
	
	int best_iteration = -1;
	double best_score = score;
	double best_paramC = start_c1;

	printf("  \t                 \t            pr[%%]  pr[%%]\n");
	printf("i \t score    P1->Cp \t action     A0     A1 \t \t dw\n");
	printf("%d \t%5.1f%% ",0,score*100);
	printf("   %5.3f \t ", playerUCT1->UCT_param_C);
	printf("// \t  ");
	Lrp->Debug_Display_Probabilites(0);
	printf(" \t %1.5f",dw);
	printf("\n");

	//Lrp iterations
	for(int i = 0; i < num_LRP_iterations; i++){

		//exponential iterative decrease of weight change step
		dw = dw_start * exp(-i/dw_tau);
		
		//Lrp select action
		selected_action = Lrp->Select_Action(TOM_LRP_SELECT_ACTION_CHECK_SUM);

		//apply action
		if(selected_action == 0)
			playerUCT1->UCT_param_C += dw;
		else
			playerUCT1->UCT_param_C -= dw;

		//evaluate new setting
		previous_score = score;		//save previous score
		score = game->Evaluate_Players(1,num_games,-1,players) / num_games;
		printf("%d \t%5.1f%% ",i+1,score*100);
		printf("   %5.3f \t ", playerUCT1->UCT_param_C);

		//Lrp correct probabilites based on previous and current score
		if(score > previous_score){
			Lrp->Update_Probabilites_Reward(selected_action,Lrp->learn_param_a);
			printf("+");
		}else{
			Lrp->Update_Probabilites_Penalty(selected_action,Lrp->learn_param_b);
			printf("-");
		}

		printf("%d \t  ",selected_action);
		Lrp->Debug_Display_Probabilites(0);
		
		printf(" \t %1.5f",dw);
		printf("\n");

		//remember best
		if(score >= best_score){
			best_iteration = i;
			best_score = score;
			best_paramC = playerUCT1->UCT_param_C;
		}
		
	}

	//--- LRP algorithm END---//

	//output best
	printf("\nBest score: %5.1f%% \t P1->Cp  %5.3f \t i  %d\n", best_score*100, best_paramC, best_iteration+1);

	cpu_time = getCPUTime()-cpu_time;
	printf("\n\nTotalRuntime: %9.3f s\n", cpu_time);

	//Lrp->Debug_Display_Probabilites();
	//Lrp->Update_Probabilites_Reward(0,Lrp->learn_param_a);
	//Lrp->Debug_Display_Probabilites();
	//Lrp->Update_Probabilites_Penalty(1,Lrp->learn_param_b);
	//Lrp->Debug_Display_Probabilites();
	//Lrp->State_Reset();
	//Lrp->Debug_Display_Probabilites();
	//delete(Lrp);
}

//LRP learning with linearly decreasing weight change step
void LRP_test_linearDW()
{
	int num_simulations		= 100;
	int num_games			= 200;
	int num_LRP_iterations	= 100;
	double dw_max			= 0.2;
	double dw_min			= 0.001;
	double lrp_ab			= 0.1;
	double start_c1			= 1.0;
	double start_c2			= 0.0;

	//Game_Engine* game = new Game_Gomoku();
	Game_Engine* game = new Game_ConnectFour();

	Tom_Lrp* Lrp = new Tom_Lrp();
	Lrp->Initialize();

	Player_AI_UCT* playerUCT1 = new Player_AI_UCT(game);
	Player_AI_UCT* playerUCT2 = new Player_AI_UCT(game);
	Player_Engine* players[] = {playerUCT1,playerUCT2};

	((Player_AI_UCT*) players[0])->UCT_param_IterNum = num_simulations;
	((Player_AI_UCT*) players[1])->UCT_param_IterNum = num_simulations;
	((Player_AI_UCT*) players[0])->UCT_param_C = start_c1;
	((Player_AI_UCT*) players[1])->UCT_param_C = start_c2;

	srand((unsigned int)time(NULL));
	double cpu_time = getCPUTime();

	printf("LRP_test_linearDW()\n");
	printf("Test setup:\n");
	printf("  game %s   sim %4d   games %4d\n",game->game_name.c_str(), num_simulations, num_games);
	printf("  LRP_iter %5d  LRP_ab %1.3f\n",  num_LRP_iterations, lrp_ab);
	printf("  dw_max %1.5f  dw_min %1.5f\n", dw_max, dw_min);
	printf("  c1 %1.3f  c2 %1.3f\n\n", start_c1, start_c2);

	Lrp->learn_param_a = lrp_ab;
	Lrp->learn_param_b = lrp_ab;

	//--- LRP algorithm ---//

	double score, previous_score;
	int selected_action;

	double dw = dw_max;

	//evaluate starting setting
	score = game->Evaluate_Players(1,num_games,-1,players) / num_games;
	
	int best_iteration = -1;
	double best_score = score;
	double best_paramC = start_c1;

	printf("  \t                 \t            pr[%%]  pr[%%]\n");
	printf("i \t score    P1->Cp \t action     A0     A1 \t \t dw\n");
	printf("%d \t%5.1f%% ",0,score*100);
	printf("   %5.3f \t ", playerUCT1->UCT_param_C);
	printf("// \t  ");
	Lrp->Debug_Display_Probabilites(0);
	printf(" \t %1.5f",dw);
	printf("\n");

	//Lrp iterations
	for(int i = 0; i < num_LRP_iterations; i++){

		//linear decrease of weight change step
		dw = dw_min + (dw_max - dw_min) * (1.0 - (double)i/(num_LRP_iterations-1));

		//Lrp select action
		selected_action = Lrp->Select_Action(TOM_LRP_SELECT_ACTION_CHECK_SUM);

		//apply action
		if(selected_action == 0)
			playerUCT1->UCT_param_C += dw;
		else
			playerUCT1->UCT_param_C -= dw;

		//evaluate new setting
		previous_score = score;		//save previous score
		score = game->Evaluate_Players(1,num_games,-1,players) / num_games;
		printf("%d \t%5.1f%% ",i+1,score*100);
		printf("   %5.3f \t ", playerUCT1->UCT_param_C);

		//Lrp correct probabilites based on previous and current score
		if(score > previous_score){
			Lrp->Update_Probabilites_Reward(selected_action,Lrp->learn_param_a);
			printf("+");
		}else{
			Lrp->Update_Probabilites_Penalty(selected_action,Lrp->learn_param_b);
			printf("-");
		}

		printf("%d \t  ",selected_action);
		Lrp->Debug_Display_Probabilites(0);
		
		printf(" \t %1.5f",dw);
		printf("\n");

		//remember best
		if(score >= best_score){
			best_iteration = i;
			best_score = score;
			best_paramC = playerUCT1->UCT_param_C;
		}
		
	}

	//--- LRP algorithm END---//

	//output best
	printf("\nBest score: %5.1f%% \t P1->Cp  %5.3f \t i  %d\n", best_score*100, best_paramC, best_iteration+1);

	cpu_time = getCPUTime()-cpu_time;
	printf("\n\nTotalRuntime: %9.3f s\n", cpu_time);

	//Lrp->Debug_Display_Probabilites();
	//Lrp->Update_Probabilites_Reward(0,Lrp->learn_param_a);
	//Lrp->Debug_Display_Probabilites();
	//Lrp->Update_Probabilites_Penalty(1,Lrp->learn_param_b);
	//Lrp->Debug_Display_Probabilites();
	//Lrp->State_Reset();
	//Lrp->Debug_Display_Probabilites();
	//delete(Lrp);
}

//BASIC LRP learning with fixed parameters
void LRP_test_basic()
{
	int num_simulations		= 100;
	int num_games			= 200;
	int num_LRP_iterations	= 5;
	double dw				= 0.01;
	double lrp_ab			= 0.1;
	double start_c1			= 0.0;
	double start_c2			= 0.0;

	//Game_Engine* game = new Game_Gomoku();
	Game_Engine* game = new Game_ConnectFour();

	Tom_Lrp* Lrp = new Tom_Lrp();
	Lrp->Initialize();

	Player_AI_UCT* playerUCT1 = new Player_AI_UCT(game);
	Player_AI_UCT* playerUCT2 = new Player_AI_UCT(game);
	Player_Engine* players[] = {playerUCT1,playerUCT2};

	((Player_AI_UCT*) players[0])->UCT_param_IterNum = num_simulations;
	((Player_AI_UCT*) players[1])->UCT_param_IterNum = num_simulations;
	((Player_AI_UCT*) players[0])->UCT_param_C = start_c1;
	((Player_AI_UCT*) players[1])->UCT_param_C = start_c2;

	srand((unsigned int)time(NULL));
	double cpu_time = getCPUTime();

	printf("LRP_test_basic()\n");
	printf("Test setup:\n");
	printf("  game %s   sim %4d   games %4d\n",game->game_name.c_str(), num_simulations, num_games);
	printf("  LRP_iter %5d  LRP_ab %1.3f  dw %1.3f\n",  num_LRP_iterations, lrp_ab, dw);
	printf("  c1 %1.3f  c2 %1.3f\n\n", start_c1, start_c2);

	Lrp->learn_param_a = lrp_ab;
	Lrp->learn_param_b = lrp_ab;

	//--- LRP algorithm ---//

	double score, previous_score;
	int selected_action;


	//evaluate starting setting
	score = game->Evaluate_Players(1,num_games,-1,players) / num_games;
	
	int best_iteration = -1;
	double best_score = score;
	double best_paramC = start_c1;

	printf("  \t                 \t            pr[%%]  pr[%%]\n");
	printf("i \t score    P1->Cp \t action     A0     A1\n",score*100);
	printf("%d \t%5.1f%% ",0,score*100);
	printf("   %5.3f \t ", playerUCT1->UCT_param_C);
	printf("// \t  ");
	Lrp->Debug_Display_Probabilites(0);
	printf("\n");

	//Lrp iterations
	for(int i = 0; i < num_LRP_iterations; i++){


		//Lrp select action
		selected_action = Lrp->Select_Action(TOM_LRP_SELECT_ACTION_CHECK_SUM);

		//apply action
		if(selected_action == 0)
			playerUCT1->UCT_param_C += dw;
		else
			playerUCT1->UCT_param_C -= dw;

		//evaluate new setting
		previous_score = score;		//save previous score
		score = game->Evaluate_Players(1,num_games,-1,players) / num_games;
		printf("%d \t%5.1f%% ",i+1,score*100);
		printf("   %5.3f \t ", playerUCT1->UCT_param_C);

		//Lrp correct probabilites based on previous and current score
		if(score > previous_score){
			Lrp->Update_Probabilites_Reward(selected_action,Lrp->learn_param_a);
			printf("+");
		}else{
			Lrp->Update_Probabilites_Penalty(selected_action,Lrp->learn_param_b);
			printf("-");
		}

		printf("%d \t  ",selected_action);
		Lrp->Debug_Display_Probabilites(0);
		
		printf("\n");

		//remember best
		if(score >= best_score){
			best_iteration = i;
			best_score = score;
			best_paramC = playerUCT1->UCT_param_C;
		}
		
	}

	//--- LRP algorithm END---//

	//output best
	printf("\nBest score: %5.1f%% \t P1->Cp  %5.3f \t i  %d\n", best_score*100, best_paramC, best_iteration+1);

	cpu_time = getCPUTime()-cpu_time;
	printf("\n\nTotalRuntime: %9.3f s\n", cpu_time);

	//Lrp->Debug_Display_Probabilites();
	//Lrp->Update_Probabilites_Reward(0,Lrp->learn_param_a);
	//Lrp->Debug_Display_Probabilites();
	//Lrp->Update_Probabilites_Penalty(1,Lrp->learn_param_b);
	//Lrp->Debug_Display_Probabilites();
	//Lrp->State_Reset();
	//Lrp->Debug_Display_Probabilites();
	//delete(Lrp);
}

void AMAF_Testing()
{
	Game_Engine* ticTac = new Game_ConnectFour();
	//Game_Engine* ticTac = new Game_Gomoku();
	//Game_Engine* ticTac = new Game_TicTacToe();

	Player_Human* playerHuman = new Player_Human(ticTac);
	Player_AI_UCT* playerUCT = new Player_AI_UCT(ticTac);
	Player_AI_UCT_AMAF* playerUCT_AMAF = new Player_AI_UCT_AMAF(ticTac);
	Player_AI_UCT_RAVE* playerUCT_RAVE = new Player_AI_UCT_RAVE(ticTac);
	Player_Random* playerRandom = new Player_Random(ticTac);

	Player_AI_UCT_TomTest* UCTtest = new Player_AI_UCT_TomTest(ticTac);

	//Player_Engine* players[] = {playerUCT_AMAF,playerUCT_RAVE};
	Player_Engine* players[] = {UCTtest,playerUCT};

	ticTac->players = players;

	playerUCT->UCT_param_IterNum = 100;

	playerUCT_RAVE->UCT_param_IterNum = playerUCT_AMAF->UCT_param_IterNum = playerUCT->UCT_param_IterNum;
	//playerUCT->UCT_param_C = 1.0 / (2*sqrt(2));
	playerUCT_RAVE->RAVE_param_V = 15.0;

	srand((unsigned int)time(NULL));


	//Player_Engine* tmpPlayer;
	//for(int i = 0; i < 10; i++){
		//ticTac->Evaluate_Players(1,1000,TOMGAME_OUTPUT_DEPTH0,players);
		//tmpPlayer = players[0]; players[0] = players[1]; players[1] = tmpPlayer;
		//ticTac->Evaluate_Players(1,1000,TOMGAME_OUTPUT_DEPTH0,players);
		//tmpPlayer = players[0]; players[0] = players[1]; players[1] = tmpPlayer;
		//
		//playerUCT_RAVE->RAVE_param_V += 2.0;
		//playerUCT->UCT_param_IterNum += 50;
		//playerUCT_AMAF->UCT_param_IterNum = playerUCT1->UCT_param_IterNum;
	//}

	//ticTac->Simulate_Output_Game();

	double approx_worse_C = 0.0;
	double approx_optimal_C = 0.175;	//ticTac 0.1 ; conn4 0.175 ; gomoku ??
	int sim1 = 100;
	int sim2 = 200;

	UCTtest->UCT_param_IterNum = sim1;
	playerUCT->UCT_param_IterNum = sim1;

	//test1
	UCTtest->UCT_param_C = approx_worse_C;
	playerUCT->UCT_param_C = approx_worse_C;

	printf("\n\n%d %d %3.3f %3.3f",UCTtest->UCT_param_IterNum,playerUCT->UCT_param_IterNum,UCTtest->UCT_param_C,playerUCT->UCT_param_C);
	AMAF_Testing_Extensive(ticTac, players, 0, 2, 1000, true);

	UCTtest->UCT_param_C = approx_optimal_C;
	AMAF_Testing_Extensive(ticTac, players, 0, 2, 1000, true);
		
	//test2
	UCTtest->UCT_param_C = approx_worse_C;
	playerUCT->UCT_param_C = approx_optimal_C;

	printf("\n\n%d %d %3.3f %3.3f",UCTtest->UCT_param_IterNum,playerUCT->UCT_param_IterNum,UCTtest->UCT_param_C,playerUCT->UCT_param_C);
	AMAF_Testing_Extensive(ticTac, players, 0, 2, 1000, true);

	UCTtest->UCT_param_C = approx_optimal_C;
	AMAF_Testing_Extensive(ticTac, players, 0, 2, 1000, true);

	//test3
	UCTtest->UCT_param_IterNum = sim2;
	playerUCT->UCT_param_IterNum = sim2;

	UCTtest->UCT_param_C = approx_worse_C;
	playerUCT->UCT_param_C = approx_worse_C;

	printf("\n\n%d %d %3.3f %3.3f",UCTtest->UCT_param_IterNum,playerUCT->UCT_param_IterNum,UCTtest->UCT_param_C,playerUCT->UCT_param_C);
	AMAF_Testing_Extensive(ticTac, players, 0, 2, 1000, true);

	UCTtest->UCT_param_C = approx_optimal_C;
	AMAF_Testing_Extensive(ticTac, players, 0, 2, 1000, true);

	//test4
	UCTtest->UCT_param_C = approx_worse_C;
	playerUCT->UCT_param_C = approx_optimal_C;

	printf("\n\n%d %d %3.3f %3.3f",UCTtest->UCT_param_IterNum,playerUCT->UCT_param_IterNum,UCTtest->UCT_param_C,playerUCT->UCT_param_C);
	AMAF_Testing_Extensive(ticTac, players, 0, 2, 1000, true);

	UCTtest->UCT_param_C = approx_optimal_C;
	AMAF_Testing_Extensive(ticTac, players, 0, 2, 1000, true);
}

void AMAF_Testing_Extensive(Game_Engine* game, Player_Engine** players, int simulations, int repeats, int games, const bool disable_sim_set)
{
	Player_Engine* tmpPlayer;

	if(!disable_sim_set){
		((Player_AI_UCT_AMAF*) players[0])->UCT_param_IterNum = simulations;
		((Player_AI_UCT*) players[1])->UCT_param_IterNum = simulations;
	}

	//printf("\nProcedure AMAF_Testing_Extensive()\n");
	//printf("\nAMAF variant: %d", TOMPLAYER_AI_UCT_AMAF_VARIANT);
	//printf("\nAMAF ignore unexpanded: %d", TOMPLAYER_AI_UCT_AMAF_IGNORE_UNTIL_EXPANDED);
	//printf("\nMCTS simulations: %d\n", simulations);
	
	game->Evaluate_Players(repeats,games,TOMGAME_OUTPUT_DEPTH0,players);
	
	//printf("\n\nPlayer swap\n");
	tmpPlayer = players[0]; players[0] = players[1]; players[1] = tmpPlayer;
	
	game->Evaluate_Players(repeats,games,TOMGAME_OUTPUT_DEPTH0,players);
	
	//reswap players
	tmpPlayer = players[0]; players[0] = players[1]; players[1] = tmpPlayer;
	
}

void FileRead_Test()
{


	fprintf(stderr,"Test error output channel\n");

	double** inputData;
	int nRows, nColumns;

	Read_Input_File_Double(&inputData,&nRows,&nColumns,"testInput.txt");
	printf("File successfully read: num samples %d, num atributes %d\n",nRows,nColumns);

	for(int i = 0; i < nRows; i++)
		delete(inputData[i]);
	delete(inputData);
}

void EEG_Testing()
{
	//DEBUG TESTING
	//Game_Engine *diffSim = new Game_EEG();
	//Player_Passive *player = new Player_Passive(diffSim);
	////Player_SameMove *player= new Player_SameMove(diffSim); player->move = 3;	

	//Player_Engine* players[] = {player};

	//diffSim->players = players;

	//diffSim->Simulate_Output_Game();
	////diffSim->Debug_Random_Move_Output();
	////diffSim->Debug_Random_Move_Output(10);
	////diffSim->Debug_Test_Sequence();
	//-END DEBUG

	string testFileName;
    Game_Engine *diffSim = new Game_EEG();		extern_call_command = ".\\Source\\matlab_code\\call_command_EEG.bat";

	//testFileName = "testInput_1.txt";		((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str(),0,1/20);
	//testFileName = "testInput_2.txt";		((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str(),0,1/20);
	//testFileName = "testInput_3.txt";			((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str(),0.0,0.05);
	//testFileName = "testInput_4.txt";		((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str(),0,1/20);
	//testFileName = "testInput_5.txt";		((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str(),0,1/20);
	//testFileName = "testInput_6.txt";		((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str(),0,1/20);
	//testFileName = "testInput_Simple.txt";		((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str(),0,1/20);

	//!!! WARNING!!! WHEN CHANGING INPUT FILE you must change the value of TOMGAME_EEG_MOVING_AVG_SAMPLES in Game_EEG.hpp !!!
	//testFileName = "EEG_input1.txt";	((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str());
	//testFileName = "EEG_input2.txt";	((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str());
	testFileName = "EEG_input3.txt";	((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str());

	//Player_Human *player = new Player_Human(diffSim);
	Player_Passive *player= new Player_Passive(diffSim);
	//Player_Random *player = new Player_Random(diffSim);
	//Player_SameMove *player= new Player_SameMove(diffSim); player->move = 3;	
	//Player_AI_Simple *player= new Player_AI_Simple(diffSim);

	Player_AI_UCT *pUCT;
	//pUCT = new Player_AI_UCT(diffSim);
	pUCT = new Player_AI_UCT_Reinforce(diffSim);
	//pUCT = new Player_AI_UCT_Reinforce_DPrepeat(diffSim);
	//pUCT = new Player_AI_UCT_Reinforce_DPpassive(diffSim);
	pUCT->UCT_param_IterNum = 9;
	pUCT->UCT_param_defaultPolicy_maxPlys = 1;
	//pUCT->UCT_param_C = 0.0;
	//pUCT->UCT_param_C = 0.08;
	//pUCT->UCT_param_C = 100;

	Player_Engine* players[] = {pUCT};
	//Player_Engine* players[] = {player};

	DiffSim_Testing(diffSim, players, testFileName);
}

void Lotka_Testing(){

	string testFileName;

	Game_Engine *diffSim = new Game_DiffSim();	extern_call_command = ".\\Source\\matlab_code\\call_command.bat";

	testFileName = "testInput_1.txt";
	//testFileName = "testInput_2.txt";
	//testFileName = "testInput_3.txt";
	//testFileName = "testInput_4.txt";
	//testFileName = "testInput_5.txt";	//WARNING: TOMGAME_DIFFSIM_TMP_TARGET_MAX_DIFF in Game_DiffSim.hpp must be set to 100.0 (otherwise score is always 0.0)
	//testFileName = "testInput_6.txt";
	//testFileName = "testInput_Simple.txt";

	((Game_DiffSim*)diffSim)->Set_Target_Trajectory_From_File(testFileName.c_str());

	//Player_Human *player = new Player_Human(diffSim);
	Player_Passive *player= new Player_Passive(diffSim);
	//Player_Random *player = new Player_Random(diffSim);
	//Player_SameMove *player= new Player_SameMove(diffSim); player->move = 3;	
	//Player_AI_Simple *player= new Player_AI_Simple(diffSim);

	Player_AI_UCT *pUCT;
	pUCT = new Player_AI_UCT(diffSim);
	//pUCT = new Player_AI_UCT_Reinforce(diffSim);
	//pUCT = new Player_AI_UCT_Reinforce_DPrepeat(diffSim);
	//pUCT = new Player_AI_UCT_Reinforce_DPpassive(diffSim);
	pUCT->UCT_param_IterNum = 100;
	pUCT->UCT_param_defaultPolicy_maxPlys = 1;
	//pUCT->UCT_param_C = 0.0;
	//pUCT->UCT_param_C = 0.08;
	//pUCT->UCT_param_C = 100;

	Player_Engine* players[] = {pUCT};
	//Player_Engine* players[] = {player};

	DiffSim_Testing(diffSim, players, testFileName);

}

void DiffSim_Testing(Game_Engine* diffSim, Player_Engine* players[], string testFileName)
{

	diffSim->players = players;

	double cpu_time = getCPUTime();


#if(!TOM_OUTPUT_TO_MATLAB)

#if(!TOM_EXTENSIVE_TEST)

	diffSim->Simulate_Output_Game();

#else

	Player_AI_UCT* pUCT = (Player_AI_UCT*)(players[0]);

	//choose test learning algorithms
	Player_AI_UCT* pUCTlist[] =		{
										new Player_AI_UCT(diffSim),
										new Player_AI_UCT_Reinforce(diffSim),
										new Player_AI_UCT_Reinforce_DPrepeat(diffSim),
										new Player_AI_UCT_Reinforce_DPpassive(diffSim)
									};

	int test_num_algorithms = sizeof(pUCTlist) / sizeof(Player_AI_UCT*);


	//set multiple test configuration
	int test_base_val_SimNum = 9;
	int test_base_val_maxPlys = 1;
	double test_base_val_paramC = pUCT->UCT_param_C;
	
	int test_comb_num_SimNum = 3;
	int test_comb_num_maxPlys = 3;
	int test_comb_num_paramC = 2;

	double test_fact_SimNum = 9.0;
	double test_fact_maxPlys = 5.0;
	double test_fact_paramC = 0.0000001;
	
	int runs = 1;
	int repeats = 100;

	//---execute testing---
	printf("\n");
	printf("%s\n",(diffSim->game_name).c_str());
	printf("%s\n",testFileName.c_str());

	//run tests
	for(int i = 0; i < test_comb_num_SimNum*test_comb_num_maxPlys*test_comb_num_paramC; i++){
	
		int tsimnum = (int)(test_base_val_SimNum * pow(test_fact_SimNum , i % test_comb_num_SimNum));
		int tmaxply = (int)(test_base_val_maxPlys * pow(test_fact_maxPlys , (int)(i/test_comb_num_SimNum) % test_comb_num_maxPlys));
		double tparamc = pUCT->UCT_param_C = test_base_val_paramC * pow(test_fact_paramC , (int)((int)(i/test_comb_num_SimNum)/test_comb_num_maxPlys) % test_comb_num_paramC);
		printf("\nnSim %d  nPly %d  parC %1.3f",tsimnum,tmaxply,tparamc);

		for(int ac = 0; ac < test_num_algorithms; ac++){
			pUCT = pUCTlist[ac];
			pUCT->UCT_param_IterNum = tsimnum;
			pUCT->UCT_param_defaultPolicy_maxPlys = tmaxply;
			pUCT->UCT_param_C = tparamc;
			players[0] = pUCT;

			diffSim->Evaluate_Players(runs,repeats,TOMGAME_OUTPUT_DEPTH0,players);
		}

	}
	

#endif

#else

	//traning games for simple AI
	if(dynamic_cast<Player_AI_Simple*>(players[0]) != NULL){
		diffSim->Learn_Players(1000,0);
		diffSim->Game_Reset();
	}

	//single output game
	diffSim->Simulate_Output_Game();

#endif

	cpu_time = getCPUTime()-cpu_time;

//------------------------------------------------
//WRITE CONFIGURATION
#if(TOM_OUTPUT_TO_MATLAB)
	FILE* outputConfStream;

	//create directory (if not already) and open file ".\Runtime_Output\output_conf.txt"
	if (CreateDirectoryA(".\\Runtime_Output", NULL) || ERROR_ALREADY_EXISTS == GetLastError()){
		fopen(&outputConfStream, (TOM_OUTPUT_FOLDER + output_conf_filename).c_str() ,"w");
	}else{		// Failed to create directory.
		printf("WARNING: unable to create 'Runtime_Output' directory, output stream will not be created!");	
	}

	//write configuration data
	fprintf(outputConfStream,"%s\n",(diffSim->game_name).c_str());
	fprintf(outputConfStream,"%s\n",testFileName.c_str());
	fprintf(outputConfStream,"\n");
	diffSim->Calculate_Score();
	fprintf(outputConfStream,"Score: %8.6f\n",diffSim->score[0]);
	fprintf(outputConfStream,"\n");
	fprintf(outputConfStream,"%s\n",(players[0]->player_name).c_str());
	if(dynamic_cast<Player_AI_UCT*>(players[0]) != NULL){
		fprintf(outputConfStream," num Sim: %d\n",((Player_AI_UCT*)players[0])->UCT_param_IterNum);
		fprintf(outputConfStream," ply Lim: %d\n",((Player_AI_UCT*)players[0])->UCT_param_defaultPolicy_maxPlys);
		fprintf(outputConfStream," par C: %6.3f\n",((Player_AI_UCT*)players[0])->UCT_param_C);		
	}
	fprintf(outputConfStream,"\n");
	fprintf(outputConfStream,"Runtime: %9.3f s", cpu_time);

	//close file
	fclose(outputConfStream);

#endif




	//diffSim->Game_Reset();
	//diffSim->Debug_Random_Move_Output();
	//diffSim->Debug_Test_Sequence();
	
}

void Benchmark()
{

	//-------------- set up benchmark test here --------------

	//choose benchmark game/problem
	Game_Engine *game1 = new Game_TicTacToe();
	//Game_Engine *game1 = new Game_ConnectFour();
	//Game_Engine *game1 = new Game_Gomoku();

	//player allocation
	Player_AI_UCT *pUCT1 = new Player_AI_UCT();
	Player_AI_UCT *pUCT2 = new Player_AI_UCT(game1);
	Player_AI_Simple *pSim1 = new Player_AI_Simple(game1);
	Player_AI_Simple *pSim2 = new Player_AI_Simple(game1);
	Player_Random *pRan1 = new Player_Random(game1);
	Player_Random *pRan2 = new Player_Random(game1);
	Player_Human *pHuman = new Player_Human(game1);

	//benchmark visualization and runtime settings
	int learn_out_depth = 0;
	int eval_out_depth = 1;	

	//game settings
	int num_repeats = 10;
	int num_games = 1000;

	//player/learning settings	
	int UCT_simulations = 1000;
	int learning_games = 100000;	//= UCT_simulations*num_games*num_repeats
	double UCT_C = 1.0 / (2*sqrt(2));

	pUCT1->Reset_Settings();
	pUCT1->Reset_Settings(game1);
	pUCT1->UCT_param_IterNum = UCT_simulations;
	pUCT2->UCT_param_IterNum = UCT_simulations;
	pUCT1->UCT_param_C = UCT_C;	
	pUCT2->UCT_param_C = UCT_C;

	pSim1->player_number = 0;
	pSim2->player_number = 1;
	pSim1->external_reset_enabled = 0;
	pSim2->external_reset_enabled = 0;

	//----------------- execute experiments -----------------------

	//pre-learning
	game1->Learn_Two_Players(learning_games,learn_out_depth, pSim1, pSim2);
	game1->Learn_Two_Players(learning_games,learn_out_depth, pSim1, pRan2);
	game1->Learn_Two_Players(learning_games,learn_out_depth, pRan1, pSim2);

	//evaulation
	game1->Evaluate_Two_Players(num_repeats,num_games,eval_out_depth, pUCT1, pUCT1);
	game1->Evaluate_Two_Players(num_repeats,num_games,eval_out_depth, pUCT1, pUCT2);
	game1->Evaluate_Two_Players(num_repeats,num_games,eval_out_depth, pUCT1, pRan1);
	game1->Evaluate_Two_Players(num_repeats,num_games,eval_out_depth, pUCT1, pSim2);
	game1->Evaluate_Two_Players(num_repeats,num_games,eval_out_depth, pSim1, pUCT1);

	//----------------- clear -----------------------

	delete(pUCT1);
	delete(pUCT2);
	delete(pSim1);
	delete(pSim2);
	delete(pRan1);
	delete(pRan2);
	delete(pHuman);
	delete(game1);

}

void ConnectFour_Testing()
{
	Game_Engine* ticTac = new Game_ConnectFour();
	//ticTac->Debug_Test_Sequence();

	Player_Human* playerHuman = new Player_Human(ticTac);
	Player_AI_UCT* playerUCT1 = new Player_AI_UCT(ticTac);

	Player_Engine* players[] = {playerUCT1,playerUCT1};

	ticTac->players = players;
	playerUCT1->UCT_param_IterNum = 100;
	//playerUCT1->UCT_param_C = 1.0 / (2*sqrt(2));

	srand((unsigned int)time(NULL));
	ticTac->Simulate_Output_Game();
}

void Gomoku_Testing()
{
	Game_Engine* gomoku = new Game_Gomoku();
	Player_Human* playerHuman = new Player_Human(gomoku);
	Player_AI_UCT* playerUCT1 = new Player_AI_UCT(gomoku);
	Player_AI_UCT* playerUCT2 = new Player_AI_UCT(gomoku);
	//Player_Engine* players[] = {playerHuman,playerUCT1};
	Player_Engine* players[] = {playerUCT1,playerUCT1};
	gomoku->players = players;

	//players settings
	playerUCT1->UCT_param_IterNum = 5000;
	playerUCT1->UCT_param_C = 1.0 / (2*sqrt(2));
	playerUCT2->UCT_param_IterNum = 10;
	playerUCT2->UCT_param_C = 1.0 / (2*sqrt(2));

	//testing
	unsigned int tmpr = (unsigned int)time(NULL);
	srand(tmpr);
	//srand(1362238250);

	//gomoku->Simulate_Output_Game();
	gomoku->Game_Reset();
	playerUCT1->Reset();
	//playerUCT2->Reset();
	gomoku->Game_Reset();
	gomoku->Simulate_Output_Game();	//UGOTOVI ZAKAJ GA TUKAJ SESUJE ÈE DAM 2x ZAPOREDOMA SIMULATE OUTPUT GAME
	gomoku->Simulate_Output_Game();
	//gomoku->Simulate_Human_Game();

	printf("%d\n",tmpr);

}

void TicTacToe_Testing()
{
	//Game_Engine* ticTac = new Game_ConnectFour();
	Game_Engine* ticTac = new Game_TicTacToe();
	//Game_Engine* ticTac = new Game_Gomoku();

	//ticTac->Debug_Test_Sequence();

	//players
	Player_Human* playerHuman = new Player_Human(ticTac);
	Player_AI_UCT* playerUCT1 = new Player_AI_UCT(ticTac);
	Player_AI_UCT* playerUCT2 = new Player_AI_UCT(ticTac);

	//binding objects and structures
	Player_Engine* players[] = {playerUCT1,playerHuman};
	ticTac->players = players;

	//players settings
	playerUCT1->UCT_param_IterNum = 1000;
	//playerUCT1->UCT_param_C = 1.0 / (2*sqrt(2));
	playerUCT2->UCT_param_IterNum = 50;
	//playerUCT2->UCT_param_C = 1.0 / (2*sqrt(2));

	//testing
	srand((unsigned int)time(NULL));
	//srand(3);

	double cpu_time = getCPUTime();
	//ticTac->Simulate_Output_Game();
	//ticTac->Evaluate_Players(10,100);
	//ticTac->Play_Move(0);
	//ticTac->Play_Move(playerUCT1->Get_Move());
	//ticTac->Play_Move(playerUCT1->Get_Move());
	//ticTac->Play_Move(playerUCT1->Get_Move());
	//playerUCT1->Output_UCT_Tree();
	cpu_time = getCPUTime()-cpu_time;
	printf("Runtime: %9.3f s\n",cpu_time);

	ticTac->Simulate_Output_Game();

	//ticTac->Debug_Test_Sequence();
	//ticTac->Simulate_Human_Game();

	//tt1->Debug_Test_Sequence();
	//Player_AI_Simple* player1 = new Player_AI_Simple(ticTac);
	//Player_AI_Simple* player2 = new Player_AI_Simple(ticTac);
	//player2->player_number = 1;
	//Player_Engine* players[] = {player1, player2};
	////ticTac->Simulate_Output_Game(players);
	//ticTac->Evaluate_Players(players,10,100);

	//players[0] = new Player_Human(ticTac);
	//player2->selectMoveType = TOMPlayer_AI_Simple_MOVE_TYPE_BEST;
	//ticTac->Game_Reset();
	//ticTac->Simulate_Output_Game(players);
}

void TicTacToe_Implementation_Test1(){

	Game_TicTacToe* ticTac = new Game_TicTacToe();
	Game_TicTacToe* tt1 = ticTac;
	Game_Engine* tt2 = new Game_TicTacToe();
	Game_Engine* tt3;

	//srand((unsigned int)time(NULL));

	tt1->Debug_Random_Move_Output(3);
	tt1->Output_Moves_History();
	tt3 = tt1->Create_Duplicate_Game();
	tt3->Debug_Random_Move_Output(3);
	tt3->Output_Moves_History();
	tt2->Copy_Game_State_From(tt1);
	tt2->Output_Moves_History();

	ticTac->Debug_Test_Sequence();
	tt1->Debug_Test_Sequence();

	ticTac->Simulate_Human_Game();
}

//main procedure
int main(int argc, char* argv[])
{
	//save program start time to global variable
#if ((_WIN32 || _WIN64) && _MSC_VER)
	__time64_t long_time;
	_time64( &long_time );								// Get time as 64-bit integer.
	program_start_time = new struct tm();
    _localtime64_s( program_start_time, &long_time );	// Convert to local time.
#else
	time_t long_time;
	time( &long_time );
    program_start_time = localtime( &long_time);					// Convert to local time.
#endif

	char tmpString[256];
	sprintf(tmpString, "__%4d-%d%d-%d%d__%d%d-%d%d-%d%d",
					program_start_time->tm_year+1900,
					(int)(program_start_time->tm_mon+1) / 10,
					(int)(program_start_time->tm_mon+1) % 10,
					(int)(program_start_time->tm_mday) / 10,
					(int)(program_start_time->tm_mday) % 10,
					(int)(program_start_time->tm_hour) / 10,
					(int)(program_start_time->tm_hour) % 10,
					(int)(program_start_time->tm_min) / 10,
					(int)(program_start_time->tm_min) % 10,
					(int)(program_start_time->tm_sec) / 10,
					(int)(program_start_time->tm_sec) % 10
				);
	program_start_time_output_str = string(tmpString);

	//set output filenames
	output_data_filename = (string)TOM_OUTPUT_DATA_FILE_TEXT_TAG + program_start_time_output_str + ".txt";
	output_conf_filename = (string)TOM_OUTPUT_CONF_FILE_TEXT_TAG + program_start_time_output_str + ".txt";

	//------------------------------------------------
	//PROGRAM EXECUTION settings

#if(TOM_OUTPUT_TO_MATLAB)
	//generate filename
	FILE* tmpStream;

	//create directory (if not already) and redirect standard output to ".\Runtime_Output\output.txt"
	if (CreateDirectoryA(".\\Runtime_Output", NULL) || ERROR_ALREADY_EXISTS == GetLastError()){
		tmpStream = freopen((TOM_OUTPUT_FOLDER + output_data_filename).c_str() ,"w",stdout);
	}else{		// Failed to create directory.
		printf("WARNING: unable to create 'Runtime_Output' directory, stdout will not be redirected!");	
	}
#endif

	//------------------------------------------------
	//USER CODE
	Main_Testing();

	//------------------------------------------------
	//MAIN EXIT PROCEDURE

	printf("\n\n");

#if(TOM_OUTPUT_TO_MATLAB)
	fclose(stdout);
	tmpStream = freopen("CONOUT$", "w", stdout);	//redirect standard output back to console
	//call Matlab script for results visualization
	system((extern_call_command + " " + output_data_filename + " " + output_conf_filename + " " + program_start_time_output_str).c_str());	
#else
	if((argc > 1)&&(argv[1][0] == '1')){
		//do not prompt for key-press
	}else{
		fflush(stdout);
		cout << endl << "Press any key to exit.";
		cin.get();
	}

#endif

}

void Hex_Testing()
{
	Game_Engine* hex = new Game_Hex();
	Player_Human* playerHuman = new Player_Human(hex);
	Player_AI_UCT* playerUCT1 = new Player_AI_UCT(hex);
	Player_AI_UCT* playerUCT2 = new Player_AI_UCT(hex);
	//Player_Engine* players[] = {playerHuman,playerUCT1};
	Player_Engine* players[] = {playerUCT1,playerUCT2};
	hex->players = players;

	//players settings
	playerUCT1->UCT_param_IterNum = 5000;
	playerUCT1->UCT_param_C = 1.0 / (2*sqrt(2));
	playerUCT2->UCT_param_IterNum = 5000;
	playerUCT2->UCT_param_C = 0.7 / (2*sqrt(2));

	//testing
	unsigned int tmpr = (unsigned int)time(NULL);
	srand(tmpr);
	//srand(1362238250);
	double cpu_time = getCPUTime();
	//gomoku->Simulate_Output_Game();
	hex->Game_Reset();
	playerUCT1->Reset();
	playerUCT2->Reset();
	hex->Game_Reset();
	hex->Simulate_Output_Game();
	//gomoku->Simulate_Human_Game();
	cpu_time = getCPUTime()-cpu_time;
	printf("Runtime: %9.3f s\n",cpu_time);
	printf("%d\n",tmpr);

}


//
//void Go_Testing()
//{
//	//GoGameEngine::humanGame(2,1,0.0);
//
//	//GoGameEngine::debugTestSequence();
//
//	//GoGameEngine::benchmarkSeries();
//
//
//	//--------------
//
//	srand((unsigned int)time(NULL));
//
//	//input parameters
//	bool playersResetAfterRepeat[] = {0, 0};
//	double playersExploreFact[] = {0.1, 0.1};
//	int board_width = 2;
//	int num_games_learn = 10000;
//	int num_games_evaluate = 100;
//	int num_repeats = 1;
//	int num_UCTsimulation = 100;
//	
//
//	//instantiate game
//	GoGameEngine* game = new GoGameEngine(board_width);
//	game->komi = 0.0;
//
//	//instantiate players
//	PlayerGoAI_basic* player1 = new PlayerGoAI_basic(game,0,playersExploreFact[0]);
//	PlayerGoAI_basic* player2 = new PlayerGoAI_basic(game,1,playersExploreFact[1]);
//	player1->selectMoveType = 1;
//	player2->selectMoveType = 1;
//	player1->resetAfterSeries = playersResetAfterRepeat[0];
//	player2->resetAfterSeries = playersResetAfterRepeat[1];
//
//	PlayerGoHuman* playerHuman1 = new PlayerGoHuman(game);
//
//	PlayerGoAI_UCT* playerUCT1 = new PlayerGoAI_UCT(game,0.1,num_UCTsimulation);
//	PlayerGoAI_UCT* playerUCT2 = new PlayerGoAI_UCT(game,0.1,num_UCTsimulation);
//	playerUCT1->UCT_defaultPolicy_maxPlys = game->board_size;
//	playerUCT2->UCT_defaultPolicy_maxPlys = game->board_size;
//
//	//PlayerGo* players[] = {player1human, player2};
//	//PlayerGo* players[] = {player1, player2};
//	//PlayerGo* players[] = {playerHuman1, playerUCT1};
//	//PlayerGo* players[] = {playerUCT1, playerHuman1};
//	PlayerGo* players[] = {player1, player2};
////	PlayerGo* players1[] = {player1, playerUCT1};
////	PlayerGo* players2[] = {playerUCT2, player2};
//	PlayerGo* players1[] = {playerUCT1, player2};
//	PlayerGo* players2[] = {player1, playerUCT2};
//
//	//execute games
//	//game->komi = 1.0;
//	//game->playOutputGame(players);
//	game->evaluatePlayersPerformance(players, 1, num_games_learn, 0.0, 0, 1.0);	//learning phase for player1 and player2
//	game->evaluatePlayersPerformance(players1, num_repeats, num_games_evaluate, 0.0, 1, 1.0);	//basic AI players are equal with komi 1.0 and exploration 0.1
//	game->evaluatePlayersPerformance(players2, num_repeats, num_games_evaluate, 0.0, 1, 1.0);	//basic AI players are equal with komi 1.0 and exploration 0.1
//	//game->evaluatePlayersPerformance(players, 100, 100000, 1.0, 1);	//basic AI players are equal with komi 1.0 and exploration 0.1
//
//	//clean up
//	delete player1;
//	delete player2;
//	delete playerUCT1;
//	delete playerUCT2;
//	//delete *players;	//not sure if needed
//	delete game;
//
//}