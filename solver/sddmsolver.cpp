#include "sddmsolver.h"
#include "common.h"
#include "matrix.h"
#include "graph.h"
#include "treefinder.h"
#include "abstractsolver.h"

function<AbstractSolver(const AbstractSolver&)> SDDMSolver(const Mat &A, Mat &B)
{
	static FLOAT128 *d=new FLOAT128[maxn];
	int n=A.n; 
	rep(i,0,n-1) d[i]=0; 
	B=Mat(n+1,n+1);
	rept(it,A.values)
	{
		int x=it->x, y=it->y; FLOAT z=it->z;
		d[x]+=z;
		B.entryAddValue(x+1,y+1,z);
		if (x!=y && z>0) printf("Warning: Given matrix is not SDDM, it contains positive off-diagonal!\n");
	}
	FLOAT s=0;
	rep(i,0,n-1)
	{
		FLOAT z=d[i];
		if (z<-1e-6) 
			printf("Warning: Given matrix is not SDDM, its diagonal is not dominating!\n"); 
		else if (z<0)
			printf("Warning: Given matrix is not SDDM, its diagonal is not dominating! (floating error?)\n"); 
		s+=z;
		B.entryAddValue(i+1,0,-z); B.entryAddValue(0,i+1,-z);
	}
	B.entryAddValue(0,0,s);
	B.sortup();
	
	return [n](const AbstractSolver &S) {
		return AbstractSolver([n,S](const Vec &b, FLOAT tol, int maxit, const Vec &x0) {
			assert(n==b.n);
			Vec b1(n+1); Vec x1(n+1);
			FLOAT s=0;
			rep(i,0,n-1) { b1[i+1]=b[i]; s+=b[i]; } 
			b1[0]=-s;
			FLOAT b1norm=b1.norm(), bnorm=b.norm();
            rep(i,0,n-1) x1[i+1]=x0[i];
            x1[0]=0;
			/*
			 * ||Bx'-b'||<=tol'*||b'||
			 * ||Ax-b||<=||Bx'-b'||
			 * Want: ||Ax-b||<=tol*|b||
			 * So: tol'=tol*||b||/||b'|| is an upper bound
			 */
			SolverReturnValue ret=S.solve(b1,tol*bnorm/b1norm,maxit,x1);	
			Vec x(n);
			rep(i,0,n-1) x[i]=ret.x[i+1]-ret.x[0];
			ret.x=x;
			return ret;
		});
	};
}

AbstractSolver SDDMSolver(const Mat &A, const function<AbstractSolver(const Mat&)> &Sv) 
{
	Mat B; auto fn=SDDMSolver(A,B);
	return fn(Sv(B));
}

