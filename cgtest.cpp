#include "common.h"
#include "matrix.h"
#include "cg.h"
#include "io.h"
#include "jacobiprecon.h"
#include "abstractsolver.h"

#include "gtest/gtest.h"

TEST(CGTest, Sanity)
{
	Mat A; GraphSP g;
    g = IO::readGraph("t7.txt");
    A = IO::constructMatrixFromGraph(g);
	int n=g.n; Vec x(n);
	rep(i,0,n-1) x[i]=double(rand())/double(RAND_MAX)*100;
	Vec b=A*x;
	int flag,iter; double relres;
	Vec x0; vector<double> rm;
	AbstractSolver w=PCG(A,JacobiPreconditioner(A));
	clock_t t_start = clock();
	//tie(x0,flag,relres,iter,rm)=CG::pcg(A,b,1e-4,-1,JacobiPreconditioner(A));
	tie(x0,flag,relres,iter,rm)=w.solve(b);
	clock_t t_end = clock();
	printf("Elapsed time: %0.3lfs\n", double(t_end - t_start) / double(CLOCKS_PER_SEC));
	printf("%.16lf\n",(A*x0-b).norm()/b.norm());
	printf("%.16lf\n",relres);
	printf("%d %d\n",flag,iter);
    rept(it,rm) printf("%.16lf ",*it);
    printf("\n");
    ReleaseAssert(relres < 1e-4);
}
