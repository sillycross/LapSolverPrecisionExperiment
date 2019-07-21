/********************************************************************************
 * 
 * SDDM Matrix Solver
 * 
 * Solving a SDDM matrix can be reduced to solving a Laplacian matrix of size n+1 by adding a ground vertex.
 * 
 * Public API:
 * 	SDDMSolver(const Mat &A, Mat &B):
 * 		Inputs: 
 * 			Mat A: the SDDM Matrix
 * 		
 * 		Outputs:
 * 			Mat B: a Laplacian matrix of size n+1
 * 			Returns a function f, which takes an AbstractSolver as input and returns an AbstractSolver
 * 			When f is fed with an AbstractSolver for B, it returns an AbstractSolver for A.
 * 
 * 	SDDMSolver(const Mat &A, const function<AbstractSolver(const Mat&)> &f):
 * 		Inputs: 
 * 			Mat A: the SDDM Matrix:
 * 			Function f: a function which, on input Laplacian matrix B, returns an AbstractSolver for B
 *		
 *		Outputs:
 *			An AbstractSolver for A.
 *
 *		Example:
 *			AbstractSolver S=SDDMSolver(A,[](const Mat &B) {
 *				GraphSP g=IO::convertLaplacianMatrixToGraphSP(B);	//construct graph corresponding to B
 *				AbstractSolver ret=RecursiveCG(g);				//construct the Recursive CG solver
 *				B.freeMemory();							//B itself is no longer used, free memory
 *				return ret;
 *			}); 
 *
 ********************************************************************************/

#ifndef __SDDM_H__
#define __SDDM_H__

#include "common.h"
#include "matrix.h"
#include "graph.h"
#include "abstractsolver.h"

function<AbstractSolver(const AbstractSolver&)> SDDMSolver(const Mat &A, Mat &B);

AbstractSolver SDDMSolver(const Mat &A, const function<AbstractSolver(const Mat&)> &Sv);

#endif
