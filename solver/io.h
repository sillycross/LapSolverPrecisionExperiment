/********************************************************************************
 * 
 * File IO Interface
 * 
 * Public API:
 * 	Mat IO::constructMatrixFromGraph(g):
 * 		GraphSP g: the graph
 * 		returns a Mat, the Laplacian matrix of graph g
 * 
 * 	GraphSP IO::convertLaplacianMatrixToGraphSP(const Mat &A)
 * 		Mat A: the Laplacian matrix
 * 		returns the graph corresponding to the Laplacian matrix
 * 		FIXME: 
 * 			Need to check input is indeed Laplacian matrix. 
 * 			Currently I'm not doing this bcuz of floating error issues.
 * 
 * 	GraphSP IO::readGraph(filename):
 * 		string filename: the input file name
 *		Reads input, calls the low stretch tree finder, and returns a GraphSP object.
 *   
 * 	GraphSP IO::readGraphSP(filename):
 * 		string filename: the input file name
 * 		Reads input, returns the GraphSP object.
 * 		It assumes that the first n-1 edges form a directed spanning tree rooted at 1.
 * 
 * 	Mat IO::readMML(filename):
 * 		string filename: the input file name
 * 		Reads the Matrix Market format input, and returns the matrix.
 *		It assumes that the input contains only the upper-triangular part of the matrix.
 * 
 * 	Mat IO::readMMA(filename):
 * 		string filename: the input file name
 * 		Reads the Matrix Market format input, and returns the matrix.
 *		It assumes that the input is the adjancy matrix of a graph, and contains only upper-triangular part.
 * 		It then constructs the Laplacian matrix corresponding to the graph.
 * 
 * 	Mat IO::readMMVec(filename):
 * 		string filename: the input file name
 * 		Reads the Matrix Market format input, and returns the vector.
 *		It assumes that the input is a dense matrix market format file, with a single column.
 *
 * 
 ********************************************************************************/

#ifndef __IO_H__
#define __IO_H__

#include "common.h"
#include "matrix.h"
#include "graph.h"

namespace IO
{
	Mat constructMatrixFromGraph(const GraphSP &g);
	
	GraphSP convertLaplacianMatrixToGraphSP(const Mat &A);	//won't report error if matrix is not symmetric

	GraphSP readGraphSP(string filename);
	
	GraphSP readGraph(string filename);

	//TOFIX Only works on symmetric mm right now
	Mat readMML(string filename);
	
	//TOFIX Only works on symmetric mm right now
	Mat readMMA(string filename);

    Vec readMMVec(string filename);
}

#endif
