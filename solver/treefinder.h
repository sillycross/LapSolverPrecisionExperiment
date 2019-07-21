/********************************************************************************
 * 
 * Low Stretch Spanning Tree utility
 * 
 * Public API:
 * 	vector<FLOAT> StretchCalculator::calculatePathResistance(const GraphSP &g):
 * 		Input: g, the graph.
 * 		Returns: 
 * 			The tree-path resistance corresponding to off-tree edges of g, in the same order as g.o.
 *			Stretch of an off-tree edge is by definition (tree-path resistance)/(off-tree edge resistance).
 *	 
 * 	FLOAT StretchCalculator::calculateTotalStretch(const GraphSP &g):
 * 		Input: g, the graph.
 * 		Returns: the total stretch of g.
 * 
 * 	GraphSP TreeFinder::findLowStretchTree(const Graph &g):
 * 		Input: g, the graph.
 * 		Returns: 
 * 			same graph, but with a spanning tree.
 * 			currently it tries Dijkstra tree and MST, and picks the better one.
 * 
 * 	GraphSP TreeFinder::findLowStretchTree(const GraphSP &g):
 * 		Input: g, the graph.
 * 		Returns: 
 * 			same graph, but with a spanning tree.
 * 			It ignores the current spanning tree in g, and finds a new one.
 * 
 ********************************************************************************/

#ifndef __TREEFINDER_H__
#define __TREEFINDER_H__

#include "common.h"
#include "graph.h"

namespace StretchCalculator
{
	vector<FLOAT> calculatePathResistance(const GraphSP &g);
	
	FLOAT calculateTotalStretch(const GraphSP &g);
}

namespace TreeFinder
{
	GraphSP findLowStretchTree(const Graph &g);
	
	GraphSP findLowStretchTree(const GraphSP &g);
}

#endif
