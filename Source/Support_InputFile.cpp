#include "Support_InputFile.hpp"

/**
Reads a file with records of type double (one per line). Caller must free *read_data after use.
Warning: code currently does not recognize if there are more than the specified number of atributes per line (it does not distinguish in records per line, but only in the total sum)
	TODO improve: move to new line after each sample (set of atributes), and check than no 'new line' occured between scanning atributes of same sample

@Return zero value if error occured. Allocates road_data as array of samples*atributes*type(double).
*/
int Read_Input_File_Double(double*** read_data, int* nRows, int* nColumns, const char* fileName, const char* filePath)
{
	int c, num_rows, count_scan, num_columns;
	FILE* inFile;

	//open file
	inFile = fopen(((string)filePath+(string)fileName).c_str(), "r");
	if(inFile == NULL){
		fprintf(stderr,"ERROR: Read_Input_File_Double(): could not open file '%s%s'",filePath,fileName);
		return 0;
	}

	//read first line (number of samples in file)
	count_scan = fscanf(inFile,"%d %d",&num_rows,&num_columns);
	if (count_scan == EOF) {
		if (ferror(inFile)) {
			perror("fscanf");
		} else {
			fprintf(stderr, "Error: Read_Input_File_Double(): fscanf() fscanf reached end of file, no matching characters, no matching failure: wrong format of file '%s%s' (line 1)\n",filePath,fileName);
		}
		return 0;
	} else if (count_scan != 2) {
		fprintf(stderr, "Error: Read_Input_File_Double(): fscanf() matching failure: wrong format of file '%s%s' (line 1)\n",filePath,fileName);
		return 0;
	}

	//allocate read samples array
	*read_data = new double*[num_rows];
	for(int i = 0; i < num_rows; i++)
		(*read_data)[i] = new double[num_columns];

	//read all samples
	c = 0;
	while((!feof(inFile)) && (c < num_rows)){

		//read next sample (iterate through atributes)
		for(int i = 0; i < num_columns; i++){
			fscanf(inFile,"%lf",&((*read_data)[c][i]));

			if (count_scan == EOF) {
				if (ferror(inFile)) {
					perror("fscanf");
				} else {
					fprintf(stderr, "Error: Read_Input_File_Double(): fscanf() fscanf reached end of file, no matching characters, no matching failure: wrong format of file '%s%s' (line %d)\n",filePath,fileName,c+1);
				}
				return 0;
			} else if (count_scan != 2) {
				fprintf(stderr, "Error: Read_Input_File_Double(): fscanf() matching failure: wrong format of file '%s%s' (line %d)\n",filePath,fileName,c+1);
				return 0;
			}
		}

		//increase storage counter
		c++;

	}

	//close file
	if (fclose(inFile) == EOF) {
		perror("fclose");
	}
	
	//check if number of rows match
	if(c < num_rows){
		fprintf(stderr, "Warning: Read_Input_File_Double(): file '%s%s' number of lines mismatch: successfully read %d, speficied in file %d\n",filePath,fileName,c,num_rows);
	}

	*nRows = num_rows;
	*nColumns = num_columns;

	return 1;
}