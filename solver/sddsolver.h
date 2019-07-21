/********************************************************************************
 * 
 * SDD Matrix Solver
 * 
 * Solving a SDD matrix can be reduced to solving a Laplacian matrix of size n*2.
 * This file uses the one-step reduction given in [KOSZ13], instead of the two-step Gremban reduction.
 * 
 * Public API:
 * 	SDDSolver(const Mat &A, Mat &B):
 * 		Inputs: 
 * 			Mat A: the SDD Matrix
 * 		
 * 		Outputs:
 * 			Mat B: a Laplacian matrix of size n*2
 * 			Returns a function f, which takes an AbstractSolver as input and returns an AbstractSolver
 * 			When f is fed with an AbstractSolver for B, it returns an AbstractSolver for A.
 * 
 * 	SDDSolver(const Mat &A, const function<AbstractSolver(const Mat&)> &f):
 * 		Inputs: 
 * 			Mat A: the SDD Matrix:
 * 			Function f: a function which, on input Laplacian matrix B, returns an AbstractSolver for B
 *		
 *		Outputs:
 *			An AbstractSolver for A.
 *
 *		Example:
 *			AbstractSolver S=SDDSolver(A,[](const Mat &B) {
 *				GraphSP g=IO::convertLaplacianMatrixToGraphSP(B);	//construct graph corresponding to B
 *				AbstractSolver ret=RecursiveCG(g);				//construct the Recursive CG solver
 *				B.freeMemory();							//B itself is no longer used, free memory
 *				return ret;
 *			}); 
 *
 ********************************************************************************/

#ifndef __SDD_H__
#define __SDD_H__

#include "common.h"
#include "matrix.h"
#include "graph.h"
#include "abstractsolver.h"

function<AbstractSolver(const AbstractSolver&)> SDDSolver(const Mat &A, Mat &B);

AbstractSolver SDDSolver(const Mat &A, const function<AbstractSolver(const Mat&)> &Sv);

#endif
