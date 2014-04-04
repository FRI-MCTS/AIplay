/**
Class with functions for printing simultaneously to multiple outputs (e.g. file and stdout)

*/

#ifndef _TOM_SUPPORT_MULTIPRINT_
#define _TOM_SUPPORT_MULTIPRINT_

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdarg>

using namespace std;

class MultiPrinter
{

public:
	
	int num_files;

	bool print_also_to_file;
	int  selected_file;
	string * filenames;

	MultiPrinter(const int set_num_files = 0, bool set_print_to_file = false, string base_filename = "");
	~MultiPrinter();

	virtual int  Create_Output_Files(int num_files_to_create = 0);	//0 means that all files will be created (num_files)
	virtual void Flush(const bool only_stdout = false);
	virtual void Close_Output_Files();

	virtual void Print(const char* format, ...);
	virtual void Print(FILE * output1, const char* format, ...);
	virtual void Print(int id_file1, const char* format, ...);
	
	virtual void PrintI(FILE * output1, const char* format, ...);					//ignores print_also_to_file
	virtual void PrintI(int id_file1, const char* format, ...);						//ignores print_also_to_file

	virtual void Print(FILE * output1, FILE * output2, const char* format, ...);	//ignores print_also_to_file
	virtual void Print(int id_file1, int id_file2, const char* format, ...);		//ignores print_also_to_file
	virtual void Print(FILE * output1, int id_file2, const char* format, ...);		//ignores print_also_to_file
	virtual void Print(int id_file1, FILE * output2, const char* format, ...);		//ignores print_also_to_file


private:

	FILE ** output_files;

};

#endif