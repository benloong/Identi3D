//
// File: matrix_test.h
// ===================
// Test matrix class.
//

#ifndef CORN3D_UINTTESTS_MATHTEST_MATRIX_TEST_H
#define CORN3D_UINTTESTS_MATHTEST_MATRIX_TEST_H

#include <src/math/Vector.h>
#include <src/math/Matrix.h>
#include <iostream>

namespace Corn3D
{

	class MatrixTest
	{
	public:
		static void testMatrixMatrixMultiply(void);
		static void testMatrixInverse(void);
		static const Matrix generateRandomMatrix(void);

		static void runAllTest(void)
		{
			testMatrixMatrixMultiply();
			testMatrixInverse();
		}
	};

};

#endif