/********************************************************************************
 * 
 * KOSZ Algorithm
 * 
 * Warning: the graph is 1-INDEXED, all inputs are RESISTANCE, not WEIGHT
 * 
 * Public API:
 * 	KOSZ(g): Constructor. 
 * 		GraphSP g: the graph 
 * 
 * 	(AbstractSolver):	implicit typecast to AbstractSolver. The initial guess x0 will be ignored.
 * 
 * 	setb(Vec b):
 * 		Sets the RHS of the equation to b
 * 
 * 	int sampleOTE():
 * 		Samples an off-tree edge w.p. propotional to 1+stretch
 * 		Returns an int k in [0,m), where m is total # of off-tree edges
 * 		The sampled off-tree edge is g.o[k]
 * 
 * 	solver:
 * 		... = solve()
 * 		... = solve(tol)
 * 		... = solve(tol, maxit)
 * 		... = solve(tol, maxit, batchnum)
 * 		LHS accepts any formats defined in abstractsolver.h
 * 
 * 	Inputs:
 * 		tol:		desired tolerance (default 1e-6)
 * 		maxit:	max # of batches to perform, default -1 (unlimited)
 * 		batchnum:	# of toggles in a batch, default -1 (4*n)
 * 	
 * 	Outputs:
 * 		see abstractsolver.h
 * 
 * Examples:
 * 	GraphSP g; Vec b;
 * 	KOSZ s1=KOSZ(g);			//construct as KOSZ Solver
 * 	Vec x=s1.solve(b);
 * 	AbstractSolver s2=KOSZ(g);	//construct as AbstractSolver
 * 	Vec x=s2.solve(b);
 * 
 ********************************************************************************/

#ifndef __KOSZ_H__
#define __KOSZ_H__

#include "common.h"
#include "matrix.h"
#include "abstractsolver.h"
#include "graph.h"

struct KOSZ
{
	int n,m;
	
	struct DS
	{
		int n;
		struct nodetype
		{
			int parent, type;		//type=0: left child; 1: right child; 2: virtual edge
			FLOAT sum, sum_resistance, tag, rPath, fPath;
		};
		nodetype *ds;
		
		void reset();
		FLOAT query(int x);
		void serere(int x, FLOAT value);
	};
	DS ds;
	
	struct otetype
	{
		int x,y;
		FLOAT resist, sum_r, flow;
	};
	otetype *ote;
	
	int *parent, *dfsorder;
	FLOAT *parente;
	
	FLOAT *str_sum;
	
	FLOAT *osum_rf, *tfPath, *tfdelta;
	
	Vec x,b;
	FLOAT err, bnorm;
	
	KOSZ();
	KOSZ(const GraphSP &g);
	void freeMemory();
	int sampleOTE();
	void toggle(int id);
	void setb(Vec _b);
	void convertFlowToSolution();
	void batchToggle(int order_num);
	SolverReturnValue solve(FLOAT tol, int maxbatch, int batchsize);
	SolverReturnValue solve(FLOAT tol, int maxbatch);
	SolverReturnValue solve(FLOAT tol);
	SolverReturnValue solve();
	operator AbstractSolver();
};

#endif
