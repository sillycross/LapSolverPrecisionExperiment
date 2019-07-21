#if 0
#include "common.h"
#include "matrix.h"
#include "kosz.h"
#include "io.h"

vector< pair<int,double> > e[maxn];
vector< tuple<int,int,double> > o;

void lemon(string s)
{
	Mat A; GraphSP g;
	IO::readGraph(s,g,A);
	int n=A.n;
	Vec x(n);
	rep(i,0,n-1) x[i]=double(rand())/double(RAND_MAX)*100;
	Vec b=A*x;
	
	printf("average stretch = %.16lf\n",StretchCalculator::calculateTotalStretch(g)/g.o.size());
	
	KOSZ w=KOSZ(g);
	double relres; int flag; int iter; vector<double> resvec;
	clock_t t_start = clock();
	w.setb(b);
	tie(x,flag,relres,iter,resvec)=w.solve();
	clock_t t_end = clock();
	
	/*
	AbstractSolver w=KOSZ(g);
	double relres; int flag; int iter; vector<double> resvec;
	clock_t t_start = clock();
	tie(x,flag,relres,iter,resvec)=w.solve(b);
	clock_t t_end = clock();
	*/
	printf("Elapsed time: %0.3lfs\n", double(t_end - t_start) / double(CLOCKS_PER_SEC));
	printf("%.16lf\n",(A*x-b).norm()/b.norm());
	printf("%.16lf %d\n",relres,iter);
	rept(it,resvec) printf("%.16lf ",*it); printf("\n");
}

int main()
{
	ios::sync_with_stdio(true);
	#ifndef ONLINE_JUDGE
	//	freopen("t1.txt","r",stdin);
	#endif
	//lemon("t1.txt");
	lemon("t7.txt");
	//lemon("t1.txt");
	return 0;
}

#endif
