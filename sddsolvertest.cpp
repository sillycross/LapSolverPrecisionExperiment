#if 0
#include "common.h"
#include "matrix.h"
#include "recursivecg.h"
#include "io.h"
#include "sddsolver.h"

void lemon()
{
	Mat A; 
	A=IO::readMML("outer_4_inner_2.mtx");
	printf("done reading input\n");
	AbstractSolver S=SDDSolver(A,[](const Mat &B) {
		GraphSP g=IO::convertLaplacianMatrixToGraphSP(B);
		AbstractSolver ret=RecursiveCG(g);
		B.freeMemory();
		return ret;
	});
	printf("constructor finished\n");
	int n=A.n;
	Vec x(n);
	rep(i,0,n-1) x[i]=double(rand())/double(RAND_MAX)*100;
	Vec b=A*x;
	
	int flag; double relres; int iter; vector<double> resvec;
	clock_t t_start = clock();
	tie(x,flag,relres,iter,resvec)=S.solve(b);
	clock_t t_end = clock();
	printf("Elapsed time: %0.3lfs\n", double(t_end - t_start) / double(CLOCKS_PER_SEC));

	printf("%.16lf\n",(A*x-b).norm()/b.norm());
	rept(it,resvec)
	{
		if (it==resvec.begin()) printf("[ "); else printf(", ");
		printf("%.16lf ",*it);
	}
	printf("\n");
}

int main()
{
	ios::sync_with_stdio(true);
	#ifndef ONLINE_JUDGE
		freopen("","r",stdin);
	#endif
	lemon();
	return 0;
}

#endif
