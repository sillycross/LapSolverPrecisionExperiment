#include <cstdio>
#include <cstdlib> 
#include "gtest/gtest.h"
#include "common.h"

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
	return RUN_ALL_TESTS();
}

