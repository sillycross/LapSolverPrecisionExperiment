#include "common.h"
#include "main.h"
#include "treeprecon.h"
#include "recursivecg.h"
#include "io.h"
#include "treefinder.h"
#include "cg.h"

#include <cstdio>

#include "gtest/gtest.h"

Vec GenerateFixedSolVec(int n)
{
    Vec x(n);
    srand(12345);
    rep(i,0,n-1) x[i]=(double(rand())/double(RAND_MAX)-0.5)*10;
    return x;
}

// args: <identifier> <graph adj mat file> [tree file]
//
void PrepareData(Mat& A /*out*/, GraphSP& g /*out*/, Vec& b /*out*/)
{
    ReleaseAssert(g_numArgs == 2 || g_numArgs == 3);
    string matrixFile = g_args[1];
    bool hasTreeFile = false;
    string treeFile;
    if (g_numArgs == 3)
    {
        hasTreeFile = true;
        treeFile = g_args[2];
    }
    A = IO::readMMA(matrixFile);
    if (!hasTreeFile)
    {
        g = IO::convertLaplacianMatrixToGraphSP(A);
    }
    else
    {
        Mat tree = IO::readMML(treeFile);
        g = IO::specifyTree(IO::convertLaplacianMatrixToGraph(A), tree);
    }
    Vec x = GenerateFixedSolVec(A.n);
    b = A * x;
}

string GetOutputVariable()
{
#ifdef USE_MPFR
    const char* precision = "mpfr" MACROTOSTRING(MPFR_PRECISION);
#else
    const char* precision = "double";
#endif
#ifdef USE_L_NORM
    const char* normDesc = "Lnorm";
#else
    const char* normDesc = "2norm";
#endif
    return string(precision) + "_" + string(normDesc);
}

void PrintResults(FILE* file, const char* algName, const GraphSP& g,
                  const vector<FLOAT>& avg, const vector<FLOAT>& stddev,
                  const vector<FLOAT>& minresvec, const vector<FLOAT>& maxresvec)
{
    const char* dataId = g_args[0];
    string varname = GetOutputVariable();
    fprintf(file, "# DataId = %s DataPath = %s TreePath = %s\n# Algorithm = %s Label = %s\n#\n",
            g_args[0],
            g_args[1],
            (g_numArgs == 3 ? g_args[2] : "(not specified)"),
            algName,
            varname.c_str());
    fprintf(file, "if not '%s' in result:\n\tresult['%s']={}\n", dataId, dataId);
    fprintf(file, "if not '%s' in result['%s']:\n\tresult['%s']['%s']={}\n", algName, dataId, dataId, algName);
    fprintf(file, "assert(not '%s' in result['%s']['%s'])\nresult['%s']['%s']['%s']={}\n",
            varname.c_str(), dataId, algName, dataId, algName, varname.c_str());
    fprintf(file, "result['%s']['%s']['%s']['numIters'] = %d\n", dataId, algName, varname.c_str(), (int)avg.size());
    double avgStretch = FloatToDouble(StretchCalculator::calculateTotalStretch(g)) / g.o.size();
    fprintf(file, "result['%s']['%s']['%s']['avgStretch'] = %.16lf\n", dataId, algName, varname.c_str(), avgStretch);
    fprintf(file, "result['%s']['%s']['%s']['relres'] = [\n", dataId, algName, varname.c_str());
    int size = avg.size();
    for (int i = 0; i < size; i++)
    {
        if (i != 0)
        {
            fprintf(file, ", ");
        }
        if (i > 0 && i % 5 == 0)
        {
            fprintf(file, "\n");
        }
        fprintf(file, "%.16le", FloatToDouble(avg[i]));
    }
    fprintf(file, "]\n\n");
    if (strcmp(algName, "recursiveCg") == 0)
    {
        fprintf(file, "result['%s']['%s']['%s']['stddev'] = [\n", dataId, algName, varname.c_str());
        int size = stddev.size();
        for (int i = 0; i < size; i++)
        {
            if (i != 0)
            {
                fprintf(file, ", ");
            }
            if (i > 0 && i % 5 == 0)
            {
                fprintf(file, "\n");
            }
            fprintf(file, "%.16le", FloatToDouble(stddev[i]));
        }
        fprintf(file, "]\n\n");
        fprintf(file, "result['%s']['%s']['%s']['minresvec'] = [\n", dataId, algName, varname.c_str());
        size = minresvec.size();
        for (int i = 0; i < size; i++)
        {
            if (i != 0)
            {
                fprintf(file, ", ");
            }
            if (i > 0 && i % 5 == 0)
            {
                fprintf(file, "\n");
            }
            fprintf(file, "%.16le", FloatToDouble(minresvec[i]));
        }
        fprintf(file, "]\n\n");
        fprintf(file, "result['%s']['%s']['%s']['maxresvec'] = [\n", dataId, algName, varname.c_str());
        size = maxresvec.size();
        for (int i = 0; i < size; i++)
        {
            if (i != 0)
            {
                fprintf(file, ", ");
            }
            if (i > 0 && i % 5 == 0)
            {
                fprintf(file, "\n");
            }
            fprintf(file, "%.16le", FloatToDouble(maxresvec[i]));
        }
        fprintf(file, "]\n\n");
    }
}

string GetFileName(const char* algName)
{
    string res = "results/";
    res += g_args[0];
    res += "_";
    res += algName;
    res += "_";
    res += GetOutputVariable();
    res += ".py.txt";
    return res;
}

TEST(Benchmark, RunTreePcg)
{
    const char* algName = "treePcg";
    Mat A; GraphSP g; Vec b;
    PrepareData(A /*out*/, g /*out*/, b /*out*/);
    printf("data read finished\n");

    AbstractSolver S = PCG(A, TreePreconditioner(g));
    printf("constructor finished\n");

    Vec x; int flag; FLOAT relres; int iter; vector<FLOAT> resvec;
    tie(x,flag,relres,iter,resvec)=S.solve(b,
                                           1e-10 /*does not matter here, tree precond is accurate*/,
                                           -300 /*force 300 iters*/);
    ReleaseAssert(flag == MaxIterReached);

    printf("Writing results to file %s..\n", GetFileName(algName).c_str());
    FILE* file = fopen(GetFileName(algName).c_str(), "w");
    ReleaseAssert(file);
    PrintResults(file, algName, g, resvec, vector<FLOAT>(), vector<FLOAT>(), vector<FLOAT>());
    fclose(file);
}

TEST(Benchmark, RunRecursiveCg)
{
    const char* algName = "recursiveCg";
    Mat A; GraphSP g; Vec b;
    PrepareData(A /*out*/, g /*out*/, b /*out*/);
    printf("data read finished\n");

    const int numRuns = 20;
    vector<FLOAT> resvecs[numRuns];
    for (int k = 0; k < numRuns; k++)
    {
        AbstractSolver S=RecursiveCG(g);
        printf("constructor finished\n");

        Vec x; int flag; FLOAT relres; int iter; vector<FLOAT> resvec;
        tie(x,flag,relres,iter,resvec)=S.solve(b,
                                               1e-10 /*only affects preconditioner tolerance*/,
                                               -300 /*force 300 iters*/);
        ReleaseAssert(flag == MaxIterReached);
        resvecs[k] = resvec;
        ReleaseAssert(resvec.size() == resvecs[0].size());
        printf("Completed %d/%d\n", k, numRuns);
    }
    vector<FLOAT> avg, stddev;
    for (size_t i = 0; i < resvecs[0].size(); i++) { avg.push_back(0); }
    for (int k = 0; k < numRuns; k++)
    {
        for (size_t i = 0; i < resvecs[0].size(); i++)
        {
            avg[i] += resvecs[k][i];
        }
    }
    for (size_t i = 0; i < resvecs[0].size(); i++) { avg[i] /= numRuns; }
    for (size_t i = 0; i < resvecs[0].size(); i++) { stddev.push_back(0); }
    for (int k = 0; k < numRuns; k++)
    {
        for (size_t i = 0; i < resvecs[0].size(); i++)
        {
            stddev[i] += (resvecs[k][i] - avg[i]) * (resvecs[k][i] - avg[i]);
        }
    }
    for (size_t i = 0; i < resvecs[0].size(); i++) { stddev[i] = mysqrt(stddev[i] / numRuns); }
    vector<FLOAT> minresvec, maxresvec;
    minresvec = resvecs[0];
    maxresvec = resvecs[0];
    for (int k = 1; k < numRuns; k++)
    {
        for (size_t i = 0; i < resvecs[0].size(); i++)
        {
            if (resvecs[k][i] < minresvec[i])
            {
                minresvec[i] = resvecs[k][i];
            }
            if (resvecs[k][i] > maxresvec[i])
            {
                maxresvec[i] = resvecs[k][i];
            }
        }
    }
    printf("Writing results to file %s..\n", GetFileName(algName).c_str());
    FILE* file = fopen(GetFileName(algName).c_str(), "w");
    ReleaseAssert(file);
    PrintResults(file, algName, g, avg, stddev, minresvec, maxresvec);
    fclose(file);
}
