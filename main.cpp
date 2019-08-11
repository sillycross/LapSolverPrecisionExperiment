#include <cstdio>
#include <cstdlib> 
#include "gtest/gtest.h"
#include "common.h"
#include "recursivecg.h"

int g_numArgs;
char** g_args;

namespace {

void PrintInformation()
{
	printf("--------------- General Information ---------------\n");
    printf("Host:                ");
	fflush(stdout);
	std::ignore = system("whoami | tr -d '\\n' && printf '@' && cat /etc/hostname");
    printf("Build flavor:        %s\n", MACROTOSTRING(BUILD_FLAVOR));
    printf("Floating point type: %s\n", GetFloatingPointPrecision());
    printf("---------------------------------------------------\n");
}

bool StartsWith(const char* s, const char* subs)
{
    int la = strlen(s);
    int lsubs = strlen(subs);
    if (la < lsubs) { return false; }
    return memcmp(s, subs, lsubs) == 0;
}

}	// annoymous namespace

int main(int argc, char **argv)
{
	PrintInformation();
	::testing::InitGoogleTest(&argc, argv);

    g_numArgs = argc - 1;
    g_args = new char*[g_numArgs];
    for (int i = 1; i < argc; i++)
    {
        g_args[i - 1] = argv[i];
    }

    for (int i = 0; i < g_numArgs; i++)
    {
        const char* param = "--sample-edges=";
        if (StartsWith(g_args[i], param))
        {
            int val = std::stoi(std::string(g_args[i] + strlen(param)));
            ReleaseAssert(val > 0);
            g_sampleFixedEdges = true;
            g_numEdgesToSample = val;
            printf("Sampling a fixed number of %d edges\n", g_numEdgesToSample);
            for (int j = i + 1; j < g_numArgs; j++)
            {
                g_args[j - 1] = g_args[j];
            }
            g_numArgs--;
            break;
        }
    }
	return RUN_ALL_TESTS();
}

