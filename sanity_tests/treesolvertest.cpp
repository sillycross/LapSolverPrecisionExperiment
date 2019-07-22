#include "common.h"
#include "matrix.h"
#include "graph.h"
#include "abstractsolver.h"
#include "treefinder.h"
#include "treeprecon.h"
#include "cg.h"
#include "io.h"

#include "gtest/gtest.h"

TEST(Sanity, TreePcg)
{
    Mat A = IO::readMMA("sanity_tests/graph.mtx");
    //Mat tree = IO::readMML("tree.mtx");
    //GraphSP g = IO::specifyTree(IO::convertLaplacianMatrixToGraph(A), tree);
    GraphSP g = IO::convertLaplacianMatrixToGraphSP(A);
    // Vec x = IO::readMMVec("x.vec");
    Vec x(A.n);
    rep(i,0,A.n-1) x[i]=double(rand())/double(RAND_MAX)*100;
    Vec b = A*x;
    printf("done reading input\n");
    AbstractSolver S = PCG(A, TreePreconditioner(g));
    printf("constructor finished\n");

    int flag; FLOAT relres; int iter; vector<FLOAT> resvec;
    clock_t t_start = clock();
    tie(x,flag,relres,iter,resvec)=S.solve(b, 1e-15);
    clock_t t_end = clock();
    printf("Elapsed time: %0.3lfs\n", double(t_end - t_start) / double(CLOCKS_PER_SEC));

    printf("%.16lf\n", FloatToDouble((A*x-b).norm()/b.norm()));
    printf("Num iters = %d\n", iter);
    rept(it,resvec)
    {
        //if (it==resvec.begin()) printf("[ "); else printf(", ");
        printf("%d %.16lf \n", (int)(it - resvec.begin()), FloatToDouble(*it));
    }
    printf("\n");

    ReleaseAssert(relres < 1e-6);
}
