#include "io.h"
#include "treefinder.h"

namespace IO
{
	Mat constructMatrixFromGraph(const GraphSP &g)
	{
		int n=g.n;
		static FLOAT *s=new FLOAT[maxn];
		rep(i,0,n) s[i]=0;
		Mat A(n,n);
		rep(i,1,n)
			rept(it,g.e[i])
			{
				int x=i, y=it->first; FLOAT z=it->second;
				x--; y--; z=1.0/z;
				A.entryAddValue(x,y,-z); A.entryAddValue(y,x,-z);
				s[x]+=z; s[y]+=z;
			}
		rept(it,g.o)
		{
			int x=get<0>(*it), y=get<1>(*it); FLOAT z=get<2>(*it);
			x--; y--; z=1.0/z;
			A.entryAddValue(x,y,-z); A.entryAddValue(y,x,-z);
			s[x]+=z; s[y]+=z;
		}
		rep(i,0,n-1) A.entryAddValue(i,i,s[i]);
		A.sortup();
		return A;
	}
	
	GraphSP convertLaplacianMatrixToGraphSP(const Mat &A)	//won't report error if matrix is not symmetric
	{
		assert(A.n==A.m);
		Graph g(A.n);
		static FLOAT *s=new FLOAT[maxn];
		rept(it,A.values)
		{
			int x=it->x, y=it->y; FLOAT z=it->z;
			if (x!=y)
			{
				if (z>0) printf("Warning: Given matrix is not Laplacian, it has positive off-diagonals!\n");
				s[x]+=z; 
				z=-1.0/z;
				g.e[x+1].push_back(make_pair(y+1,z));
			}
			else
			{
				if (z<0) printf("Warning: Given matrix is not Laplacian, it has negative diagonals!\n");
				s[x]+=z;
			}
		}
		//rep(i,0,A.n-1) if (fabs(s[i])>1e-6) printf("Warning: Given matrix is not Laplacian, its row/col sum is not 0!\n");
		GraphSP g2=TreeFinder::findLowStretchTree(g);
		g.freeMemory();
		return g2;
	}
	
	GraphSP readGraphSP(string filename)
	{
		GraphSP g;
		FILE* ff = fopen(filename.c_str(), "r");
		if (!ff) { printf("File error.\n"); assert(0); }
		int n,m;
        ReleaseAssert(fscanf(ff, "%d%d", &n, &m) == 2);
		char buff[100];
        std::ignore = fgets(buff,100,ff);	//skip the "order_num" parameter
		vector< pair<int,FLOAT> > *e=new vector< pair<int,FLOAT> >[n+1];
		vector< tuple<int,int,FLOAT> > o;
		rep(i,1,n) e[i].clear();
		rep(i,1,n-1)
		{
            int x,y; double z; ReleaseAssert(fscanf(ff,"%d%d%lf",&x,&y,&z) == 3);
			e[x+1].push_back(make_pair(y+1,z));
		}
		o.clear();
		rep(i,1,m-(n-1))
		{
            int x,y; double z; ReleaseAssert(fscanf(ff,"%d%d%lf",&x,&y,&z) == 3);
			o.push_back(make_tuple(x+1,y+1,z));
		}
		g.n=n; g.e=e; g.o.swap(o);
		fclose(ff);
		return g;
	}
	
	GraphSP readGraph(string filename)
	{
		FILE* ff = fopen(filename.c_str(), "r");
		if (!ff) { printf("File error.\n"); assert(0); }
		int n,m;
        ReleaseAssert(fscanf(ff, "%d%d", &n, &m) == 2);
		char buff[100];
        std::ignore = fgets(buff,100,ff);	//skip the "order_num" parameter
		Graph h(n); 
		rep(i,1,n) h.e[i].clear();
		rep(i,1,m)
		{
            int x,y; double z; ReleaseAssert(fscanf(ff,"%d%d%lf",&x,&y,&z) == 3);
			h.e[x+1].push_back(make_pair(y+1,z));
			h.e[y+1].push_back(make_pair(x+1,z));
		}
		GraphSP g=TreeFinder::findLowStretchTree(h);
		h.freeMemory();
		fclose(ff);
		return g;
	}

	//TOFIX Only works on symmetric mm right now
	Mat readMML(string filename)
	{
		FILE* ff = fopen(filename.c_str(), "r");
		if (!ff) { printf("File error.\n"); assert(0); }
		int c;
		char buff[100];
		
		do
		{
            std::ignore = fgets (buff, 100, ff);
			c = getc(ff);
			ungetc(c,ff);
		} while(c=='%');
		
		int n,temp,m;
        ReleaseAssert(fscanf(ff, "%d%d%d", &n, &temp, &m) == 3);
				
		int x,y;
		double z;
		
		Mat A=Mat(n,n);
		rep(i,1,m)
		{
            ReleaseAssert(fscanf(ff,"%d%d%lf",&x,&y,&z) == 3);
			x--; y--;
			A.entryAddValue(x,y,z);
			if (x!=y) A.entryAddValue(y,x,z);
		}
		
		fclose(ff);
			
		A.sortup();
		return A;
	}
	
	//TOFIX Only works on symmetric mm right now
	Mat readMMA(string filename)
	{
		FILE* ff = fopen(filename.c_str(), "r");
		if (!ff) { printf("File error.\n"); assert(0); }
		int c;
		char buff[100];
		
		do
		{
            std::ignore = fgets (buff, 100, ff);
			c = getc(ff);
			ungetc(c,ff);
		} while(c=='%');
		
		int n,temp,m;
        ReleaseAssert(fscanf(ff, "%d%d%d", &n, &temp, &m) == 3);
		static FLOAT *s=new FLOAT[maxn];
		rep(i,0,n) s[i]=0;
		int x,y;
		double z;
		
		Mat A=Mat(n,n);
		rep(i,1,m)
		{
            ReleaseAssert(fscanf(ff,"%d%d%lf",&x,&y,&z) == 3);
			x--; y--;

			A.entryAddValue(x,y,-z);
			A.entryAddValue(y,x,-z);
			s[x]+=z; s[y]+=z;
		}
		
		fclose(ff);
		
		rep(i,0,n-1) A.entryAddValue(i,i,s[i]);
		A.sortup();
		return A;
	}

    Vec readMMVec(string filename)
    {
		FILE* ff = fopen(filename.c_str(), "r");
		if (!ff) { printf("File error.\n"); assert(0); }
		int c;
		char buff[100];
		
		do
		{
            std::ignore = fgets (buff, 100, ff);
			c = getc(ff);
			ungetc(c,ff);
		} while(c=='%');
		
		int n,m;
        ReleaseAssert(fscanf(ff, "%d%d", &n, &m) == 2);
		assert(m==1);

		Vec v(n);

		double z;
	     
		rep(i,0,n-1)
		{
            ReleaseAssert(fscanf(ff,"%lf",&z) == 1);
			FLOAT zz=z;
			v[i]=zz;
		}
		
		fclose(ff);
		
		
		return v;
    }
}

