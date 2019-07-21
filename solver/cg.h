/********************************************************************************
 * 
 * Preconditioned Conjugacy Gradiant Method
 * 
 * Public API (almost identical as in MATLAB):
 * 	CG::pcg:
 * 		... = CG::pcg(A,b)
 * 		... = CG::pcg(A,b,tol)
 * 		... = CG::pcg(A,b,tol,maxit)
 * 		... = CG::pcg(A,b,tol,maxit,Preconditioner)
 * 		... = CG::pcg(A,b,tol,maxit,Preconditioner,x0)
 * 		LHS accepts any formats defined in abstractsolver.h
 * 
 * 	PCG(A,precon):
 * 		construct AbstractSolver
 * 		precon is optional (see below)
 * 
 * Inputs:
 * 	Mat A, Vec b: Ax=b, A square real symmetric positive definite
 * 	FLOAT tol: desired tolerance (default 1e-6)
 * 	int maxit: max # of iterations (default -1, unlimited)
 * 	Preconditioner (default none):
 * 		Either
 * 			an "AbstractSolver" type variable, 
 * 		or an function of any of the 4 prototypes:
 * 			Vec Preconditioner(const Vec &b);
 * 			Vec Preconditioner(const Vec &b, FLOAT err);
 * 			Vec Preconditioner(const Vec &b, const Vec &x, FLOAT err);
 *			Vec Preconditioner(const Vec &b, const Vec &x, FLOAT err, FLOAT tol);
 * 		Vec x is the current solution. FLOAT err is the current relative error. 
 * 		It should return M^{-1}b. 
 * 	Vec x0: initial guess (default zero vector)
 *	 
 ********************************************************************************/

#ifndef __CG_H__
#define __CG_H__

#include "common.h"
#include "matrix.h"
#include "abstractsolver.h"

namespace CG
{
	void pcgsolve(const Mat &A, const Vec &b, FLOAT tol, int maxit, const function<void(const Vec&, const Vec&, FLOAT, FLOAT, Vec&)> &precon, const Vec &_x0, SolverReturnValue &ret);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const AbstractSolver &precon, const Vec &x0);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const function<Vec(const Vec&, const Vec&, FLOAT, FLOAT)> &precon, const Vec &x0);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const function<Vec(const Vec&, const Vec&, FLOAT)> &precon, const Vec &x0);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const function<Vec(const Vec&, FLOAT)> &precon, const Vec &x0);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const function<Vec(const Vec&)> &precon, const Vec &x0);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const AbstractSolver &precon);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const function<Vec(const Vec&, const Vec&, FLOAT, FLOAT)> &precon);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const function<Vec(const Vec&, const Vec&, FLOAT)> &precon);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const function<Vec(const Vec&, FLOAT)> &precon);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit, const function<Vec(const Vec&)> &precon);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol, int maxit);
	SolverReturnValue pcg(const Mat &A, const Vec &b, FLOAT tol);
	SolverReturnValue pcg(const Mat &A, const Vec &b); 
}

AbstractSolver PCG(const Mat &A, const AbstractSolver &precon);
AbstractSolver PCG(const Mat &A, const function<Vec(const Vec&, const Vec&, FLOAT, FLOAT)> &precon);
AbstractSolver PCG(const Mat &A, const function<Vec(const Vec&, const Vec&, FLOAT)> &precon);
AbstractSolver PCG(const Mat &A, const function<Vec(const Vec&, FLOAT)> &precon);
AbstractSolver PCG(const Mat &A, const function<Vec(const Vec&)> &precon);
AbstractSolver PCG(const Mat &A);
		
#endif
