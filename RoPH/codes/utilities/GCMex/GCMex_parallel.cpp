/* GCMex.cpp Version 2.3.0 
 *
 * Copyright 2009 Brian Fulkerson <bfulkers@cs.ucla.edu> 
 */

#include "mex.h"
#include "GCoptimization.h"
#include <stdlib.h>
#include <omp.h>

//to compile, run:
//mex -O -g  -largeArrayDims COMPFLAGS="/openmp $COMPFLAGS" GCMex_parallel.cpp graph.cpp GCoptimization.cpp LinkedBlockList.cpp maxflow.cpp
//to compile in Linux, run:
//mex -O -g  -largeArrayDims CXXFLAGS="\$CXXFLAGS -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" GCMex_parallel.cpp graph.cpp GCoptimization.cpp LinkedBlockList.cpp maxflow.cpp

void GCblock(double* classes, float * data, mxArray* pairwise, float * labelcost, bool expansion, int num_pixels, int num_labels, mxArray*labels, double*energy, double*energy_after, int max_iter)
{

double* label = mxGetPr(labels);

//处理只有一个像素节点的特殊情况
if (num_pixels == 1) 
{
    *energy = data[(int)classes[0]];
    int minlabel = (int)classes[0];
    double mincost  = *energy;
    for(int i = 0; i < num_labels; i++)
      if(data[i] < mincost) {
        mincost = data[i];
        minlabel = i;
      }
    label[0] = minlabel;
    *energy_after = mincost;
    return;
}

//处理一般的情况
GCoptimizationGeneralGraph * gc = new GCoptimizationGeneralGraph(num_pixels, num_labels);
for (int i = 0; i < num_pixels; i++) {
  gc->setLabel(i, (int)classes[i]);
}

gc->setDataCost(data);

/* Data costs are nlabels rows x npixels cols */
gc->setSmoothCost(labelcost);

/* Set spatialy varying part of the smoothness cost with the neighborhood 
 */
mwSize total = 0;
double * pair = mxGetPr(pairwise);
mwIndex * ir = mxGetIr(pairwise);
mwIndex * jc = mxGetJc(pairwise);
for (int col=0; col < num_pixels; col++) {
  mwIndex starting_row_index = jc[col];
  mwIndex stopping_row_index = jc[col+1];
  if (starting_row_index == stopping_row_index)
    continue;
  
  for (int idx = starting_row_index; idx < stopping_row_index; idx++) {
    /* only set bottom triangle of pairwise, per GC_README */
    if ( ir[idx] > col )
      gc->setNeighbors(ir[idx], col, pair[total]);
    total++;
  }
}

*energy = gc->compute_energy();

if(expansion)
  gc->expansion(max_iter);
else
  gc->swap(max_iter);

*energy_after = gc->compute_energy();

for (int i = 0; i < num_pixels; i++ )
  label[i] = gc->whatLabel(i);

delete gc;
}

//mex function
void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
    int max_iter = INT_MAX;
    
    //ERROR CHECKING
    if (nrhs != 1 && nrhs!=2)
        mexErrMsgTxt("One or two arguments are required.");

	if (nrhs==2)
		max_iter = *mxGetPr(prhs[1]);
    
    size_t n_block = mxGetN(prhs[0]);
    //mexPrintf("n_block=%d\n",n_block);
    
    //extract block information
    double **classes = new double*[n_block];
    float **data = new float*[n_block];
    mxArray **pairwise = new mxArray*[n_block];
    float **labelcost = new float*[n_block];
    bool *expansion = new bool[n_block];
    int *num_pixels = new int[n_block];
    int *num_labels = new int[n_block];
    for(int i=0;i<n_block;i++){
        classes[i] = mxGetPr(mxGetCell(prhs[0],5*i));
        mxArray* tmp = mxGetCell(prhs[0],5*i+1);
        data[i] = (float*)mxGetPr(tmp);
        num_labels[i] = (int)mxGetM(tmp);
        num_pixels[i] = (int)mxGetN(tmp);
        pairwise[i] = mxGetCell(prhs[0],5*i+2);
        labelcost[i] = (float*)mxGetPr(mxGetCell(prhs[0],5*i+3));
        expansion[i] = (bool)mxGetScalar(mxGetCell(prhs[0],5*i+4));
        //mexPrintf("num_labels=%d, num_pixels=%d, %d\n",num_labels[i],num_pixels[i],expansion[i]);
    }
    
    //allocate outputs
    plhs[0] = mxCreateCellMatrix(1,n_block);
    plhs[1] = mxCreateNumericMatrix(1, n_block, mxDOUBLE_CLASS, mxREAL);
    plhs[2] = mxCreateNumericMatrix(1, n_block, mxDOUBLE_CLASS, mxREAL);
    mxArray **label = new mxArray*[n_block];
    for(int i=0;i<n_block;i++){
        label[i] = mxCreateNumericMatrix(1, num_pixels[i], mxDOUBLE_CLASS, mxREAL);
    }
    double *energy = mxGetPr(plhs[1]);
    double *energy_after = mxGetPr(plhs[2]);
    
    //run GCMex
    #pragma omp parallel for
    for(int j=0;j<n_block;j++) {
        //mexPrintf("%d\n",j);
        //#pragma omp critical
        {
            GCblock(classes[j],data[j],pairwise[j],labelcost[j],expansion[j],num_pixels[j],num_labels[j],label[j],&energy[j],&energy_after[j],max_iter);
        }
    }
    
    //get results
    for(int i=0;i<n_block;i++) {
        mxSetCell(plhs[0], i, label[i]);
    }
    
    delete[] classes;
    delete[] data;
    delete[] pairwise;
    delete[] labelcost;
    delete[] expansion;
    delete[] num_pixels;
    delete[] num_labels;
    delete[] label;
}
