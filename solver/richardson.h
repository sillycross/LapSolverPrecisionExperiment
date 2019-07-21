#ifndef __RICHARDSON_H__
#define __RICHARDSON_H__

#include "common.h"
#include "matrix.h"
#include "abstractsolver.h"
#include "graph.h"

AbstractSolver Richardson(const Mat &A, const GraphSP &g);

AbstractSolver Richardson(const Mat &A, const AbstractSolver &precon);

#endif
