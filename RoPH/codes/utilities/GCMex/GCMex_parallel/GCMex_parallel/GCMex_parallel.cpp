// GCMex_parallel.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "GCMex_parallel.h"

#include "mex.h"
#include "GCoptimization.h"
#include <stdlib.h>
#include <omp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib,"libmx.lib")
#pragma comment(lib,"libmex.lib")
#pragma comment(lib,"libmat.lib")

/* GCMex.cpp Version 2.3.0 
 *
 * Copyright 2009 Brian Fulkerson <bfulkers@cs.ucla.edu> 
 */

//to compile, run:
//mex GCMex_parallel.cpp -largeArrayDims  COMPFLAGS="/openmp $COMPFLAGS"
//to compile in Linux, run:
//mex GCMex_parallel.cpp -largeArrayDims -fopenmp

void GCMex_cell(const mxArray*in, mxArray*out, int n_block){
    mxArray **prhs = new mxArray*[5*n_block];
    mxArray **plhs = new mxArray*[3*n_block];
    for(int i=0;i<5*n_block;i++){
        prhs[i] = mxGetCell(in,i);
        mexCallMATLAB(0, NULL, 1, prhs+i, "disp");
    }
    #pragma omp parallel for
    for(int j=0;j<n_block;j++) {
        //mexCallMATLAB(3,&plhs[3*j],5,&prhs[5*j],"GCMex");
    }
    for(int i=0;i<3*n_block;i++){
        mxSetCell(out, i, plhs[i]);
        //mexCallMATLAB(0, NULL, 1, plhs+i, "disp");
    }
    delete[] prhs;
    delete[] plhs;
}

//mex function
void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
    
    //ERROR CHECKING
    if (nrhs != 1)
        mexErrMsgTxt("One argument is required.");
    
    int n_block = mxGetN(prhs[0]);
    mexPrintf("n_block=%d\n",n_block);
    plhs[0] = mxCreateCellMatrix(3,n_block);
    
    //run GCMex
    GCMex_cell(prhs[0], plhs[0], n_block);
}
