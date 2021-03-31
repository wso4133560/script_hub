#include "hs_compile_test.hpp"
#include "hs_compile_multi_test.hpp"

int main(int argc, char *argv[])
{
	HsCompileTest hsCompileTest;
	hsCompileTest.TestModeBlock();
	hsCompileTest.TestModeStream();
	hsCompileTest.TestModeVectored();
	
	HsCompileMultiTest hsCompileMultiTest;
	hsCompileMultiTest.TestModeBlock();
	hsCompileMultiTest.TestLogic();
	return 0;
}