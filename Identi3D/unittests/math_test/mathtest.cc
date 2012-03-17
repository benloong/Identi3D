//
// File: mathtest.cc
// =================
//

#include "vector_test.h"
#include "matrix_test.h"

using namespace Identi3D;

int main()
{
	VectorTest::runAllTest();
	MatrixTest::runAllTest();
	VMMTimerTest vmmtt;
	//vmmtt.runTest();
	system("pause");
	return 0;
}