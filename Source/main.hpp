#ifndef _TOM_MAIN_
#define _TOM_MAIN_


//include C libraries
#include <iostream>
#include <cstdio>
#include <ctime>

//include other

#include "Game_ConnectFour.hpp"
#include "Game_Gomoku.hpp"
#include "Game_TicTacToe.hpp"
#include "Game_DiffSim.hpp"
#include "Game_EEG.hpp"
#include "Game_Hex.hpp"

#include "Player_Engine.hpp"
#include "Player_AI_Simple.hpp"
#include "Player_AI_UCT.hpp"
#include "Player_AI_UCT_Reinforce.hpp"
#include "Player_AI_UCT_AMAF.hpp"
#include "Player_AI_UCT_RAVE.hpp"

#include "Player_AI_UCT_TomTest.hpp"

#include "Tom_Lrp.hpp"
#include "Tom_Sample_Storage.hpp"

#include "Support_MultiPrint.hpp"
#include "Support_InputFile.hpp"

#include "MPI.hpp"

using namespace std;


//defines
#define TOM_GLOBAL_DUPLICATE_OUTPUT_TO_FILE		1

#define TOM_OUTPUT_FOLDER				(".\\Runtime_Output\\")
#define TOM_OUTPUT_DATA_FILE_TEXT_TAG	("output_data")
#define TOM_OUTPUT_CONF_FILE_TEXT_TAG	("output_conf")

//compatiblity defines
// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif
// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif



//global variables
struct tm * program_start_time;
string program_start_time_output_str;
string output_data_filename;
string output_conf_filename;
string extern_call_command;

MultiPrinter * gmp;

#endif
