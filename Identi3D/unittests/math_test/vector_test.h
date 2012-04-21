//
// File: vector_test.h
// ===================
// Test vector class.
//

#ifndef IDENTI3D_UINTTESTS_MATHTEST_VECTOR_TEST_H
#define IDENTI3D_UINTTESTS_MATHTEST_VECTOR_TEST_H

#include <src/math/Vector.h>
#include <src/math/Matrix.h>
#include <iostream>

#include "timer.h"

#define _fcomp(a, b) (fabs((a) - (b)) < FLT_EPSILON)
#define _fzero(a) (fabs(a) < FLT_EPSILON)

namespace Identi3D
{

	class VectorTest
	{
	public:
		static void testVectorMatrixMultiply(void);
		static void testVectorNormalize(void);
		static void testVectorCross(void);
		static const Vector3 generateRandomVector(void);

		static void runAllTest(void)
		{
			srand((unsigned int)time(NULL));
			testVectorMatrixMultiply();
			testVectorNormalize();
			testVectorCross();
		}
	};

};

#endif
