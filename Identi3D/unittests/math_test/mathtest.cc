//
// File: mathtest.cc
// =================
//

#include "vector_test.h"
#include "matrix_test.h"
#include "ray_test.h"

using namespace Identi3D;

int main()
{
	VectorTest::runAllTest();
	MatrixTest::runAllTest();
	RayTest::runAllTest();
	//system("pause");
	return 0;
}