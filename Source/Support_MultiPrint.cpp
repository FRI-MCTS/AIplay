//include header
#include "Support_MultiPrint.hpp"

MultiPrinter::MultiPrinter(const int set_num_files, bool set_print_to_file, string base_filename)
{

	num_files = set_num_files;
	print_also_to_file = set_print_to_file;
	selected_file = 0;
	
	filenames = new string[num_files];
	output_files = new FILE*[num_files];
	for(int i = 0; i < num_files; i++)
		output_files[i] = NULL;

	if(!base_filename.empty()){
		for(int i = 0; i < num_files; i++){
			stringstream tmpStr;
			
			if((i < 10)&&(num_files > 9))
				tmpStr << "__out0" << i;
			else
				tmpStr << "__out" << i;

			filenames[i] = (base_filename + tmpStr.str() + ".txt");
		}
	}
}

MultiPrinter::~MultiPrinter()
{
	Flush();
	Close_Output_Files();	
	delete[] filenames;
	delete[] output_files;
}

int MultiPrinter::Create_Output_Files(int num_files_to_create)
{
	
	int retVal = 0;

	if(num_files_to_create <= 0)
		num_files_to_create = num_files;

	for(int i = 0; i < num_files_to_create; i++){
		output_files[i] = fopen(filenames[i].c_str() ,"w");
		if(output_files[i] == NULL){
			printf("WARNING: unable to open output file: %s\n", filenames[i].c_str());	
			retVal = 1;
		}
	}

	return retVal;
}

void MultiPrinter::Flush(const bool only_stdout)
{
	fflush(stdout);
	if(!only_stdout)
		for(int i = 0; i < num_files; i++)
			if(output_files[i] != NULL)
				fflush(output_files[i]);
}

void MultiPrinter::Close_Output_Files()
{
	for(int i = 0; i < num_files; i++)
		if(output_files[i] != NULL)
			fclose(output_files[i]);
}

void MultiPrinter::Print(const char* format, ...)
{
	va_list vl, vl2;
	va_start(vl, format);
	va_copy(vl2, vl);

	vfprintf(stdout,format,vl);
	if(print_also_to_file)
		vfprintf(output_files[selected_file],format,vl2);

	va_end(vl);
	va_end(vl2);
}

void MultiPrinter::Print(FILE * output1, const char* format, ...)
{
	va_list vl, vl2;
	va_start(vl, format);
	va_copy(vl2, vl);

	vfprintf(output1,format,vl);
	if(print_also_to_file)
		vfprintf(output_files[selected_file],format,vl2);

	va_end(vl);
	va_end(vl2);
}
void MultiPrinter::Print(int id_file1, const char* format, ...)
{
	va_list vl, vl2;
	va_start(vl, format);
	va_copy(vl2, vl);

	vfprintf(output_files[id_file1],format,vl);
	if(print_also_to_file)
		vfprintf(output_files[selected_file],format,vl2);

	va_end(vl);
	va_end(vl2);
}


void MultiPrinter::PrintI(FILE * output1, const char* format, ...)
{
	va_list vl;
	va_start(vl, format);

	vfprintf(output1,format,vl);

	va_end(vl);
}
void MultiPrinter::PrintI(int id_file1, const char* format, ...)
{
	va_list vl;
	va_start(vl, format);

	vfprintf(output_files[id_file1],format,vl);

	va_end(vl);
}


void MultiPrinter::Print(FILE * output1, FILE * output2, const char* format, ...)
{
	va_list vl, vl2;
	va_start(vl, format);
	va_copy(vl2, vl);

	vfprintf(output1,format,vl);
	vfprintf(output2,format,vl2);

	va_end(vl);
	va_end(vl2);
}

void MultiPrinter::Print(int id_file1, int id_file2, const char* format, ...)
{
	va_list vl, vl2;
	va_start(vl, format);
	va_copy(vl2, vl);

	vfprintf(output_files[id_file1],format,vl);
	vfprintf(output_files[id_file2],format,vl2);

	va_end(vl);
	va_end(vl2);
}

void MultiPrinter::Print(FILE * output1, int id_file2, const char* format, ...)
{
	va_list vl, vl2;
	va_start(vl, format);
	va_copy(vl2, vl);

	vfprintf(output1,format,vl);
	vfprintf(output_files[id_file2],format,vl2);

	va_end(vl);
	va_end(vl2);
}

void MultiPrinter::Print(int id_file1, FILE * output2, const char* format, ...)
{
	va_list vl, vl2;
	va_start(vl, format);
	va_copy(vl2, vl);

	vfprintf(output_files[id_file1],format,vl);
	vfprintf(output2,format,vl2);

	va_end(vl);
	va_end(vl2);
}
