#ifndef _TOM_SAMPLE_STORAGE_
#define _TOM_SAMPLE_STORAGE_

//includes
#include <cmath>
#include "Support_get_median_torben.hpp"
#include "Support_get_median_quickselect.hpp"

// -----------------------------------------------------------//
// Measurements storage class -- header

//global defines
#define TOM_CONFIDENCE_INTERVAL_T_80	1.282
#define TOM_CONFIDENCE_INTERVAL_T_90	1.645
#define TOM_CONFIDENCE_INTERVAL_T_95	1.960
#define TOM_CONFIDENCE_INTERVAL_T_99	2.576
#define TOM_CONFIDENCE_INTERVAL_T_999	3.291

//defines - default values - parameters
#define TOM_SAMPLE_STORAGE_ADD_SAMPLE_CALC_SUM	true
#define TOM_SAMPLE_STORAGE_ADD_SAMPLE_CALC_AVG	false
#define TOM_SAMPLE_STORAGE_CALC_MEDIAN_MODIFY	false
#define TOM_SAMPLE_STORAGE_CALC_MEDIAN_BELOW_MIDDLE	false

//defines - debug and protection
#define TOM_SAMPLE_STORAGE_ADD_SAMPLE_SAFETY_CHECK	true

template <class T>
class Tom_Sample_Storage
{
public:
	Tom_Sample_Storage(int new_max_samples);
	virtual ~Tom_Sample_Storage();

	void Reset();
	void Reset(int new_max_samples);

	void Add_Sample(T value, const bool calculate_sum = TOM_SAMPLE_STORAGE_ADD_SAMPLE_CALC_SUM, const bool calculate_avg = TOM_SAMPLE_STORAGE_ADD_SAMPLE_CALC_AVG, const bool safety_check = TOM_SAMPLE_STORAGE_ADD_SAMPLE_SAFETY_CHECK);

	double	Calc_All();
	double	Calc_AllExceptSum();

	T		Calc_Sum();
	double	Calc_AvgDev();
	double	Calc_Avg();
	double	Calc_Dev();

	double	Calc_Avg_On_Interval(int index_start, int index_end);
	double	Calc_AvgDev_On_Interval(int index_start, int index_end, double* output_avg, double* output_dev);	//returns average

	double	Calc_Confidence(double t_constant = TOM_CONFIDENCE_INTERVAL_T_95);
	
	double	Calc_Median(const bool allow_modify_array = TOM_SAMPLE_STORAGE_CALC_MEDIAN_MODIFY, const bool if_even_number_select_below_middle = TOM_SAMPLE_STORAGE_CALC_MEDIAN_BELOW_MIDDLE);

	int max_samples;	//maximum number of samples: array length
	T* samples;			//pointer to array of samples

	int n;				//current number of samples

	T sum;				//sum
	double avg;			//average value
	double dev;			//std. deviation
	double conf;		//confidence interval
	double median;			//median

private:
	void Init(int new_max_samples);

};

// Measurements storage class -- implementation

template <class T>
Tom_Sample_Storage<T>::Tom_Sample_Storage(int new_max_samples)
{
	Init(new_max_samples);
}

template <class T>
Tom_Sample_Storage<T>::~Tom_Sample_Storage()
{
	delete[] samples;
}

template <class T>
void Tom_Sample_Storage<T>::Init(int new_max_samples)
{
	this->max_samples = new_max_samples;
	samples = new T[max_samples];
	Reset();
}

template <class T>
void Tom_Sample_Storage<T>::Reset()
{
	n = 0;
	sum = (T)0.0;
	avg = 0.0;
	dev = 0.0;
	conf = 0.0;
}

template <class T>
void Tom_Sample_Storage<T>::Reset(int new_max_samples)
{
	delete[] samples;
	Init(new_max_samples);
}

template <class T>
void Tom_Sample_Storage<T>::Add_Sample(T value, const bool calculate_sum, const bool calculate_avg, const bool safety_check)
{
	//check for index out of bounds
	if(safety_check){
		if((n >= max_samples)||(n < 0)){
			printf("ERROR: Tom_Sample_Storage():Add_Sample(): index %d out of array, size %d\n", n, max_samples);
			return;
		}
	}

	//add sample
	samples[n] = value;
	n++;

	//calculate avg and sum
	if(calculate_sum){

		sum += value;
		if(calculate_avg)
			avg = (double)sum / n;

	}else if(calculate_avg){

		avg += (((double)value - avg) / n);

	}

}

/**
Calculates sum, average, deviation, confidence and median
Return average
*/
template <class T>
double Tom_Sample_Storage<T>::Calc_All()
{
	Calc_Sum();
	Calc_AllExceptSum();
	return avg;
}

/**
Calculates sum, average, deviation, confidence and median
Return average
*/
template <class T>
double Tom_Sample_Storage<T>::Calc_AllExceptSum()
{
	Calc_AvgDev();
	Calc_Confidence();
	Calc_Median();
	return avg;
}

/**
Calculates average and deviation
WARNING: sum must have already been calculated
Return average
*/
template <class T>
double Tom_Sample_Storage<T>::Calc_AvgDev()
{
	Calc_Avg();
	Calc_Dev();
	return avg;
}

/**
Calculates and returns sum
*/
template <class T>
T Tom_Sample_Storage<T>::Calc_Sum()
{
	sum = (T)0.0;
	for(int i = 0; i < n; i++)
		sum += samples[i];
	return sum;
}

/**
Calculates and returns average
WARNING: sum must have already been calculated
*/
template <class T>
double Tom_Sample_Storage<T>::Calc_Avg()
{
	avg = (double)sum / n;
	return avg;
}

/**
Calculates and returns standard deviation
WARNING: avg must have already been calculated
*/
template <class T>
double Tom_Sample_Storage<T>::Calc_Dev()
{
	double tmpSum = 0.0;
	for(int i = 0; i < n; i++)
		tmpSum += ( (samples[i]-avg)*(samples[i]-avg) );
	dev = sqrt(tmpSum / n);
	return dev;
}

/**
Calculates and returns the average on a given interval of samples
WARNING: index_end >= index_start
*/
template <class T>
double Tom_Sample_Storage<T>::Calc_Avg_On_Interval(int index_start, int index_end)
{
	double tmp = 0.0;
	for(int i = index_start; i < index_end; i++)
		tmp += samples[i];
	return (tmp / (double)(index_end - index_start));
}

/**
Calculates the average and deviation on a given interval of samples
WARNING: index_end >= index_start
The values are returned through pointer parameters
*/
template <class T>
double Tom_Sample_Storage<T>::Calc_AvgDev_On_Interval(int index_start, int index_end, double* output_avg, double* output_dev)
{
	double tmpAvg = Calc_Avg_On_Interval(index_start, index_end);
	double tmpSum = 0.0;
	for(int i = index_start; i < index_end; i++)
		tmpSum += ( (samples[i]-tmpAvg)*(samples[i]-tmpAvg) );
	(*output_dev) = sqrt(tmpSum / (index_end-index_start));
	(*output_avg) = tmpAvg;
	return tmpAvg;
}

/**
Calculates and returns confidence interval from average value
WARNING: std. deviation must have already been calculated
*/
template <class T>
double Tom_Sample_Storage<T>::Calc_Confidence(double t_constant)
{
	conf = t_constant * (dev / (sqrt(n)));
	return conf;
}

template <class T>
double Tom_Sample_Storage<T>::Calc_Median(const bool allow_modify_array, const bool if_even_number_select_below_middle)
{
	//get median value, if number of elements is even then the first value below middle is returned 
	if(!allow_modify_array){
		median = torben<T>(samples, n);
	}else{
		median = quick_select<T>(samples, n);
	}

	//if number of elements is even the find 
	//CURRENTLY SLOW IMPLEMENTATION, FIND BETTER OR USE ONLY median below middle value for even numbers
	if(!TOM_SAMPLE_STORAGE_CALC_MEDIAN_BELOW_MIDDLE){
		if(n % 2 == 0){
			int countLarger = 0;
			double minDiff = DBL_MAX;
			double nextMedianValue = median;
			for(int i = 0; i < n; i++){
				if(samples[i] > median){
					if(samples[i] - median < minDiff){
						minDiff = samples[i] - median;
						nextMedianValue = samples[i];
					}
					countLarger++;
				}				
			}
			//calculate average of the two values
			if(countLarger >= n/2)				
				median = (median + nextMedianValue) / 2.0;
		}
	}

	return median;
}

#endif