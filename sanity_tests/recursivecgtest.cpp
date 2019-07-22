#include "common.h"
#include "matrix.h"
#include "recursivecg.h"
#include "io.h"

#include "gtest/gtest.h"

TEST(Sanity, RecursiveCG)
{
	Mat A; GraphSP g;
    A = IO::readMMA("sanity_tests/graph.mtx");
    g = IO::convertLaplacianMatrixToGraphSP(A);
	printf("done reading input\n");
    AbstractSolver S=RecursiveCG(g);
	printf("constructor finished\n");
	int n=A.n;
	Vec x(n);
	rep(i,0,n-1) x[i]=double(rand())/double(RAND_MAX)*100;
	Vec b=A*x;
	
    int flag; FLOAT relres; int iter; vector<FLOAT> resvec;
	clock_t t_start = clock();
    tie(x,flag,relres,iter,resvec)=S.solve(b);
	clock_t t_end = clock();
	printf("Elapsed time: %0.3lfs\n", double(t_end - t_start) / double(CLOCKS_PER_SEC));

    printf("Iters = %d\n", iter);
    printf("%.16lf\n", FloatToDouble((A*x-b).norm()/b.norm()));
	rept(it,resvec)
	{
		if (it==resvec.begin()) printf("[ "); else printf(", ");
        printf("%.16lf ", FloatToDouble(*it));
	}
	printf("\n");
}
