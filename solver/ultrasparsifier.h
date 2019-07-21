/********************************************************************************
 * 
 * Ultra Sparsifier
 * 
 * Public API:
 * 	UltraSparsifier(const GraphSP &g, int m):
 * 		Inputs:
 * 			g: the input graph
 * 			m: the off-tree edges to be sampled
 * 		
 * 		Returns:
 * 			the result graph
 * 
 * Note:
 * 	Currently the sparsifier just samples m edge according to stretch, 
 * 	and remove any duplicated edges. So the result might contain less than m edges.
 * 
 *******************************************************************************/

#ifndef __ULTRASPARSIFIER_H__
#define __ULTRASPARSIFIER_H__

#include "common.h"
#include "matrix.h"  
#include "graph.h"

GraphSP UltraSparsifier(const GraphSP &g, int sm);

#endif

	
