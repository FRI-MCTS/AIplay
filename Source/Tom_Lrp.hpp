#ifndef _TOM_LRP_
#define _TOM_LRP_

//includes
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdarg>
#include <limits>

using namespace std;

// -----------------------------------------------------------//
// GENERAL TEMPLATE FOR OBJECTS

//defines - default values - runtime
#define TOM_COMMON_OBJECT_INIT_WITHOUT_ALLOCATION	false

//defines - enumerators
enum TOM_COMMON_OBJECT_INIT_SETTINGS_MODE{
	TOM_COMMON_OBJECT_INIT_SETTINGS_RESET,
	TOM_COMMON_OBJECT_INIT_SETTINGS_APPLY_NEW
};
enum TOM_COMMON_OBJECT_INFO_ARRAY{
	TOM_COMMON_OBJECT_INFO_NAME,
	TOM_COMMON_OBJECT_INFO_NAME_SHORT,
	TOM_COMMON_OBJECT_INFO_DESCRIPTION,
	TOM_COMMON_OBJECT_INFO_ARRAY_NUM_ELEMENTS
};

//MAIN CLASS
class Tom_Common_Object
{

// -- STRUCTURES -- //
public:
protected:

// -- COMMON PROCEDURES -- //
public:
	Tom_Common_Object();
	//~Tom_Common_Object();
	virtual void Initialize(const int without_allocation = TOM_COMMON_OBJECT_INIT_WITHOUT_ALLOCATION);
	virtual void Settings_Reset();	
	virtual void Settings_Apply_New();
protected:
	virtual void Reinitialize(const int apply_new_settings);

// -- IMPLEMENTATION-SPECIFIC PROCEDURES -- //
public:
	virtual void State_Reset()					{};

protected:
	virtual void Memory_Init_Constants()		{};
	virtual void Memory_Free_Constants()		{};
	virtual void Settings_Reset_Private()		{};
	virtual void Settings_Apply_New_Private()	{};
	virtual void Memory_Allocate()				{};
	virtual void Memory_Free()					{};


// -- VARIABLES -- //
public:
	//string object_info;//[TOMLRP_INFO_ARRAY_NUM_ELEMENTS];

protected:
	bool flag_is_initialized;
	bool flag_is_allocated;


// -- DEBUG AND VISUALIZATION -- //
public:
	int debug_visualization_level;

protected:

};

// -----------------------------------------------------------//
// Linear Reward Punishment (LRP) Learning Automaton

//defines - default values - parameters
#define TOM_LRP_DEFAULT_NUM_ACTIONS	2
#define TOM_LRP_DEFAULT_PARAM_A		0.1
#define TOM_LRP_DEFAULT_PARAM_B		TOM_LRP_DEFAULT_PARAM_A

//defines - default values - runtime
#define TOM_LRP_USE_SQUARE_RANDOM		0
#define TOM_LRP_DEFAULT_OUTPUT_LEVEL	2

//defiens - debug
#define TOM_LRP_SUM_ERROR_BOUND		0.000001

//defines - enumerators
enum TOM_LRP_SELECT_ACTION_CHECK{
	TOM_LRP_SELECT_ACTION_DEFAULT,
	TOM_LRP_SELECT_ACTION_CHECK_SUM
};

class Tom_Lrp : public Tom_Common_Object
{

// -- COMMON PROCEDURES -- //

public:
	Tom_Lrp(){};
	~Tom_Lrp();

protected:
	virtual void Memory_Init_Constants();
	virtual void Settings_Reset_Private();
	virtual void Memory_Allocate();
	virtual void Memory_Free();


// -- IMPLEMENTATION-SPECIFIC STRUCTURES -- //
public:
	typedef struct LRP_sample {

		int		previous_action;	//which action was taken before evaluation

		int		num_repeats;		//number of evaluation repeats
		double	score;				//evaluation score
		double	stddev;				//evaluation deviation

		double* param_values;		//array of parameter values at evaluation
		
		double	kNN_score;			//score obtained from kNN averaging

	} LRP_sample;

// -- IMPLEMENTATION-SPECIFIC PROCEDURES -- //
// LRP

public:

	virtual void State_Reset();
	virtual int  Select_Action(const int check_sum = TOM_LRP_SELECT_ACTION_DEFAULT);
	virtual void Update_Probabilites_Reward(int action_index, double change_step);
	virtual void Update_Probabilites_Penalty(int action_index, double change_step);
	virtual void Debug_Display_Probabilites(const int output_detail_level = TOM_LRP_DEFAULT_OUTPUT_LEVEL);
	
	virtual void Evaluations_History_Allocate(int max_num_samples, int num_parameter_weights);
	virtual void Evaluations_History_Resize(int new_max_num_samples, int new_num_parameter_weights);
	virtual void Evaluations_History_Add_Sample(int action, int num_repeats, double score, double stddev, double* parameter_weights);
	virtual void Debug_Display_Evaluations_History();

	virtual int	 Evaluations_History_Best_Single(const int output_detail_level = TOM_LRP_DEFAULT_OUTPUT_LEVEL);
	virtual int	 Evaluations_History_Best_KNN_Average_Slow(int num_neighbours, const bool weighted_by_repeats = false, const double central_weight_factor = 1.0, const bool manhattan_distance = true, const int output_detail_level = TOM_LRP_DEFAULT_OUTPUT_LEVEL);
// -- VARIABLES -- //
public:
	//parameters
	double learn_param_a, learn_param_b;
	int num_actions;

	//number of updates
	int	num_updates_since_alloc;
	int	num_updates_since_reset;

	//history of evaluations
	int			evaluations_history_num_samples;
	int			evaluations_history_max_samples;
	int			evaluations_history_num_param_values;
	LRP_sample*	evaluations_history;

protected:
	double* action_probabilities;
	//int last_action;


// -- DEBUG AND VISUALIZATION -- //

};



// -----------------------------------------------------------//
// Function aproximator with weighted parameters

//defines - default values - parameters
#define TOM_FUNC_APPROX_DEFAULT_NUM_RESULTS				1
#define TOM_FUNC_APPROX_DEFAULT_NUM_PARAM_PER_RESULT	1
#define TOM_FUNC_APPROX_DEFAULT_INIT_PARAM_WEIGHTS		0.0
#define TOM_FUNC_APPROX_DEFAULT_PARAM_CHANGE_STEP		0.1
#define TOM_FUNC_APPROX_DEFAULT_RESULTS_SCALAR_FACT		1.0

//defines - default values - runtime
#define TOM_FUNC_APPROX_OUTPUT_WEIGHTS_ON_UPDATE		-1
#define TOM_FUNC_APPROX_DEFAULT_OUTPUT_LEVEL			2

//defines - debug and protection
#define TOM_FUNC_APPROX_STORAGE_ACTION_SAFETY_CHECK		true

class Tom_Function_Approximator : public Tom_Common_Object
{
	
// -- COMMON PROCEDURES -- //

public:
	Tom_Function_Approximator(){};
	~Tom_Function_Approximator();
	virtual void State_Reset()					{};

protected:
	virtual void Memory_Init_Constants();
	virtual void Settings_Reset_Private();
	virtual void Memory_Allocate();
	virtual void Memory_Free();


// -- IMPLEMENTATION-SPECIFIC PROCEDURES -- //
// Sigmoid function with weighted parameters

public:
	//public procedures
	void Calculate_Results();
	void Calculate_Results(double bias);
	void Calculate_Results(double p1, double bias);
	void Calculate_Results(double p1, double p2, double bias);
	void Calculate_Results(double p1, double p2, double p3, double bias);
	void Calculate_Results(int n_params, ...);
	void Action_Update_Weights(int action, const int debug_output_level = TOM_FUNC_APPROX_OUTPUT_WEIGHTS_ON_UPDATE, const bool safety_check = TOM_FUNC_APPROX_STORAGE_ACTION_SAFETY_CHECK);
	void Action_Update_Weights(int action, double delta_weight, const int debug_output_level = TOM_FUNC_APPROX_OUTPUT_WEIGHTS_ON_UPDATE, const bool safety_check = TOM_FUNC_APPROX_STORAGE_ACTION_SAFETY_CHECK);
	void Set_Weights_Change_Step(double new_change_step);
	void Debug_Display_Weights(const int output_detail_level = TOM_FUNC_APPROX_DEFAULT_OUTPUT_LEVEL);

// -- VARIABLES -- //
public:
	//parameters
	int num_parameters_per_result;		//number of parameters per result
	double* parameter_weights_change_step;
	double* results_scalar_factors;

	//output
	double* results;
	int num_actions;
	int num_params;

	//read-only (specific to implementation)
	int num_results;
	int num_actions_per_parameter;

	//internal (could be protected, but for ease of access we leave it public)
	double* parameter_weights;

protected:
	bool Validate_Action(int action);
};



#endif