#include "ultrasparsifier.h"
#include "common.h"
#include "matrix.h"
#include "io.h"
#include "kosz.h"

GraphSP UltraSparsifier(const GraphSP &g, int sm)
{
	KOSZ z=KOSZ(g);	//just to sample off-tree edges....
	vector< tuple<int,int,FLOAT> > no;
	set<int> vis;
	rep(i,1,sm)
	{
		int t=z.sampleOTE();
		if (vis.find(t)!=vis.end()) continue;
		vis.insert(t);
		no.push_back(g.o[t]);
	}
	z.freeMemory(); 
	int n=g.n;
	GraphSP h(n); rep(i,1,n) h.e[i]=g.e[i]; h.o.swap(no);
	return h;
}

