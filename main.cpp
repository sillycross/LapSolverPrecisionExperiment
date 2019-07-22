#include <cstdio>
#include <cstdlib> 
#include "gtest/gtest.h"
#include "common.h"

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

	return RUN_ALL_TESTS();
}

