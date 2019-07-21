/********************************************************************************
 * 
 * Recursive CG Graph Laplacian Solver
 * 
 * Public API:
 * 	RecursiveCG(const GraphSP &g, int flag): 
 * 		Inputs:
 * 			GraphSP g: the graph
 * 			int flag: the RecursiveCG solver will call JacobiCG with max=flag iters, 
 * 			          to get a good initial solution (default 10)
 * 	
 * 		Returns:
 * 			an AbstractSolver for g
 * 			see abstractsolver.h for definition
 * 
 * FIXME: 
 * 	atm the shrinkStrategy is very crude, can tweak for better performance
 * 
 ********************************************************************************/

#ifndef __RECURSIVECG_H__
#define __RECURSIVECG_H__

#include "common.h"
#include "matrix.h"
#include "graph.h"
#include "abstractsolver.h"

AbstractSolver RecursiveCG(const GraphSP &g, int flag = 10);

#endif
