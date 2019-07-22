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
        return std::max(50, g.n / 30);
	}
}

AbstractSolver RecursiveCG(const GraphSP &g, int flag)
{
    int sm= flag ? RecursiveCGHelper::shrinkStrategy(g) : -1;
	//if (sm==-1) return Richardson(IO::constructMatrixFromGraph(g),g);
	//if (sm==-1) { Mat mG=IO::constructMatrixFromGraph(g); return PCG(mG,TreePreconditioner(g)); }
	if (sm==-1) return KOSZ(g);
	GraphSP g2=UltraSparsifier(g,sm); 
    AbstractSolver preconS=ContractSolver(g2,[](const GraphSP &h) { return RecursiveCG(h, 0); });
	Mat mG=IO::constructMatrixFromGraph(g);
	//Mat mG2=IO::constructMatrixFromGraph(g2);
	g2.freeMemory();
	AbstractSolver S1=PCG(mG,preconS);
    return S1;
}

