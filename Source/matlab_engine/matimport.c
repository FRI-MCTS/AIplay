/*
 * Create a simple MAT-file to import into MATLAB.
 * Calling syntax:
 *
 *   matimport
 *
 * Simplified version of the matcreat.c program.
 * See the MATLAB External Interfaces Guide for 
 * compiling information.
 *
 * Copyright 2010 The MathWorks, Inc.
 */
/* $Revision: 1.1.6.2 $ */
#include <stdio.h>
#include <string.h> /* For memcpy() */
#include <stdlib.h> /* For EXIT_FAILURE, EXIT_SUCCESS */
#include "mat.h"

int main() {
  
  /* MAT-file */
  MATFile *pmat;
  const char *myFile = "matimport.mat";
  
  /* Data from external source */
  const char *extString = "Data from External Device";
  double extData[9] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
  
  /* Variables for mxArrays  */
  mxArray *pVarNum, *pVarChar;
  
  /* MATLAB variable names */
  const char *myDouble = "inputArray";
  const char *myString = "titleString";
  
  int status; 

  /* Create and open MAT-file */
  printf("Creating file %s...\n\n", myFile);
  pmat = matOpen(myFile, "w");
  if (pmat == NULL) {
    printf("Error creating file");
    return(EXIT_FAILURE);
  }

  /* Create mxArrays and copy external data */
  pVarNum = mxCreateDoubleMatrix(3,3,mxREAL);
  if (pVarNum == NULL) {
      printf("Unable to create mxArray with mxCreateDoubleMatrix\n");
      return(EXIT_FAILURE);
  }
  memcpy((void *)(mxGetPr(pVarNum)), (void *)extData, sizeof(extData));

  pVarChar = mxCreateString(extString);
  if (pVarChar == NULL) {
      printf("Unable to create mxArray with mxCreateString\n");
      return(EXIT_FAILURE);
  }

  /* Write data to MAT-file */
  status = matPutVariable(pmat, myString, pVarChar);
  if (status != 0) {
      printf("Error writing %s.\n", myString);
      return(EXIT_FAILURE);
  } 
  
  status = matPutVariable(pmat, myDouble, pVarNum);
  if (status != 0) {
      printf("Error writing %s.\n", myDouble);
      return(EXIT_FAILURE);
  }  
  
  if (matClose(pmat) != 0) {
    printf("Error closing file %s.\n", myFile);
    return(EXIT_FAILURE);
  }

  /* Clean up */
  mxDestroyArray(pVarNum);
  mxDestroyArray(pVarChar);

  printf("Done\n");
  return(EXIT_SUCCESS);
}
