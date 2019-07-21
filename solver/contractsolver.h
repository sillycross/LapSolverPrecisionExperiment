/********************************************************************************
 * 
 * Graph Contraction Solver
 * 
 * On a connected graph G with n vertices and m edges, 
 * G can be contracted to a graph H with no more than 4(m-n+1) vertices and 5(m-n+1) edges
 * A Laplacian solver for H can be converted to a solver for G in linear time.
 * Graph Contraction Solver takes a graph G and a solver for H, and constructs the solver for G.
 * 
 * Public API:
 * 	ContractSolver(const GraphSP &g, GraphSP &h):
 *		Inputs:
 *			GraphSP g, the input graph
 * 		
 * 		Outputs:
 * 			The contracted graph of g is stored in h
 * 			Returns a function f, which takes an AbstractSolver as parameter, and returns an AbstractSolver
 * 			If f is fed with an AbstractSolver for h, it returns an AbstractSolver for g.
 * 
 * 		Example:
 * 			auto f=ContractSolver(g,h);	
 * 			AbstractSolver S=KOSZ(h);	//construct KOSZ solver for graph h
 * 			AbstractSolver T=f(S);		//now T is a solver for graph g
 * 
 * 	ContractSolver(const GraphSP &g, const function<AbstractSolver(const GraphSP&)> &f):
 * 		Inputs:
 *			GraphSP g, the input graph
 * 			f: a function which, when given a graph as input, returns an AbstractSolver for that graph.
 * 
 * 		Outputs:
 * 			the AbstractSolver for g.
 * 
 * 		Example:
 * 			AbstractSolver S=ContractSolver(g,[](const GraphSP &h) {
 * 				return (AbstractSolver)KOSZ(h);
 *			});
 * 
 ********************************************************************************/

#ifndef __CONTRACTSOLVER_H__
#define __CONTRACTSOLVER_H__

#include "common.h"
#include "matrix.h"
#include "graph.h"
#include "abstractsolver.h"

function<AbstractSolver(const AbstractSolver&)> ContractSolver(const GraphSP &g, GraphSP &hs);

AbstractSolver ContractSolver(const GraphSP &g, const function<AbstractSolver(const GraphSP&)> &Sv);

#endif
