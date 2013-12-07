#ifndef _TOM_GAME_INTERFACE_
#define _TOM_GAME_INTERFACE_

//include C libraries
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <random>
#include <limits>
#include <cfloat>
//#include <cmath>
//#include <ctime>
//#include <cstring>
//#include <cstdlib>

//include other
#include "Support_GetCPUTime.hpp"
#include "Tom_Sample_Storage.hpp"


//defines - global DEBUG, WARNING: specific to Visual Studio
#ifdef _DEBUG
#define TOM_DEBUG 1
#else
#define TOM_DEBUG 0
#endif
#define TOM_DISABLE_RANDOM	0

//defines - output to file and visualization with Matlab
//WARNING: currently implemented only for DIFFSIM and EEG testing
#define TOM_OUTPUT_TO_MATLAB	0
#define TOM_EXTENSIVE_TEST		~(TOM_OUTPUT_TO_MATLAB)

//defines - global VISUALIZATION levels
#define TOMGAME_OUTPUT_DEPTH0	0
#define TOMGAME_OUTPUT_DEPTH1	1
#define TOMGAME_OUTPUT_DEPTH2	2
#define TOMGAME_OUTPUT_DEPTH3	3
#define TOMGAME_OUTPUT_DEPTH4	4
#define TOMGAME_OUTPUT_DEPTH5	5

using namespace std;



#endif