#ifndef _TOM_SUPPORT_INPUTFILE_
#define _TOM_SUPPORT_INPUTFILE_

#include <cstdio>
#include <string>
using namespace std;

#define TOM_READ_INPUT_FILE_DEFAULT_PATH (".\\Runtime_Input\\")

int Read_Input_File_Double(double*** read_data, int* nRows, int* nColumns, const char* fileName, const char* filePath = TOM_READ_INPUT_FILE_DEFAULT_PATH);

#endif
