#if 0
#include "common.h"
#include "matrix.h"
#include "recursivecg.h"
#include "io.h"
#include "sddmsolver.h"
#include "jacobiprecon.h"
#include "treeprecon.h"

void lemon(string fname)
{
	Mat A,B; 
	A=IO::readMML(fname);
	//printf("done reading input\n");
	auto f=SDDMSolver(A,B);
	GraphSP g=IO::convertLaplacianMatrixToGraphSP(B);
	AbstractSolver SA=f(RecursiveCG(g,0));
	AbstractSolver SB=f(PCG(B,TreePreconditioner(g)));
	//printf("constructor finished\n");
	
	string gradfname="grads/"+fname.substr(0,fname.length()-4)+"_grad.mtx";
	Vec b=IO::readMMVec(gradfname);
	
	double avgstr=StretchCalculator::calculateTotalStretch(g)/g.o.size();
	
	Vec x; int flag; FLOAT relres; int iter; vector<FLOAT> resvec;
	
	clock_t t_start = clock();
	tie(x,flag,relres,iter,resvec)=SA.solve(b,1e-6,2000);
	clock_t t_end = clock();
	double ta=FLOAT(t_end - t_start) / FLOAT(CLOCKS_PER_SEC);
	string fla="   "; if (flag==1) fla="(M)"; if (flag==2) fla="(S)";
	int itera=iter; double relresa=(A*x-b).norm()/b.norm();
	
	t_start = clock();
	tie(x,flag,relres,iter,resvec)=SB.solve(b,1e-6,2000);
	t_end = clock();
	double tb=FLOAT(t_end - t_start) / FLOAT(CLOCKS_PER_SEC);
	string flb="   "; if (flag==1) flb="(M)"; if (flag==2) flb="(S)";
	int iterb=iter; double relresb=(A*x-b).norm()/b.norm();
	
	printf("%20s %10.3lf %10s %7d%3s %10.3lfs %10.2e %7d%3s %10.3lfs %10.2e\n", fname.c_str(), avgstr, "Actual", itera, fla.c_str(), ta, relresa, iterb, flb.c_str(), tb, relresb);

	/*
	printf("%.16lf\n",(A*x-b).norm()/b.norm());
	rept(it,resvec)
	{
		if (it==resvec.begin()) printf("[ "); else printf(", ");
		printf("%.16lf ",*it);
	}
	printf("\n");
	*/
}

int main(int argc, char *argv[])
{
	if (argc!=2) { printf("needs filename\n"); return 0; }
	ios::sync_with_stdio(true);
	#ifndef ONLINE_JUDGE
		freopen("","r",stdin);
	#endif
	lemon(argv[1]);
	return 0;
}

#endif
