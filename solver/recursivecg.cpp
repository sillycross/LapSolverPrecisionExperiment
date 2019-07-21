#include "recursivecg.h"
#include "common.h"
#include "matrix.h"
#include "graph.h"
#include "cg.h"
#include "kosz.h"
#include "io.h"
#include "ultrasparsifier.h"
#include "contractsolver.h"
#include "jacobiprecon.h"
#include "treeprecon.h"
#include "treefinder.h"
#include "richardson.h"

namespace RecursiveCGHelper
{
	int shrinkStrategy(const GraphSP &g)	//returns # of OTE to be sampled, -1 for direct solver
	{
		int n=g.n, sm;
		if (n>=10000000) 
			sm=50000;
		else if (n>=1000000)
			sm=5000;
		else if (n>=100000)
			sm=1000;
		else sm=-1;
		return sm;
	}
}

AbstractSolver RecursiveCG(const GraphSP &g, int flag)
{
	int sm=RecursiveCGHelper::shrinkStrategy(g);
	//if (sm==-1) return Richardson(IO::constructMatrixFromGraph(g),g);
	//if (sm==-1) { Mat mG=IO::constructMatrixFromGraph(g); return PCG(mG,TreePreconditioner(g)); }
	if (sm==-1) return KOSZ(g);
	GraphSP g2=UltraSparsifier(g,sm); 
	AbstractSolver preconS=ContractSolver(g2,[flag](const GraphSP &h) { return RecursiveCG(h,flag); });
	Mat mG=IO::constructMatrixFromGraph(g);
	//Mat mG2=IO::constructMatrixFromGraph(g2);
	g2.freeMemory();
	AbstractSolver S1=PCG(mG,preconS);
	/*
	AbstractSolver S1=PCG(mG,[preconS](const Vec &b, const Vec &x, FLOAT err, FLOAT tol)->Vec {
		return preconS.solve(b,0.1);	//solve to 0.1 tolerance only
	});
	*/
	if (flag>0)
	{
		int ff=flag;
		AbstractSolver S=PCG(mG,JacobiPreconditioner(mG));
		//AbstractSolver S=PCG(mG,TreePreconditioner(g));
		return AbstractSolver([S,S1,ff](const Vec &b, FLOAT tol, int maxit, const Vec &x0) {
			//iterate ff times for a good starting solution
			Vec x1; int flag; tie(x1,flag)=S.solve(b,tol,ff,x0);
			return S1.solve(b,tol,maxit,x1);
		});
	}
	else	return S1;
}

