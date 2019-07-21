#ifndef __TREEPRECON_H__
#define __TREEPRECON_H__

#include "common.h"
#include "matrix.h"
#include "graph.h"

function<Vec(const Vec&)> TreePreconditioner(const GraphSP &g);

#endif
