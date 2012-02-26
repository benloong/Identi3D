//
// File: vector_test.h
// ===================
// Test vector class.
//

#ifndef NOVA3D_UINTTESTS_MATHTEST_VECTOR_TEST_H
#define NOVA3D_UINTTESTS_MATHTEST_VECTOR_TEST_H

#include <src/math/Vector.h>
#include <src/math/Matrix.h>
#include <iostream>

#include "timer.h"

namespace Nova3D
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
			testVectorMatrixMultiply();
			testVectorNormalize();
			testVectorCross();
		}
	};

	class VMMTimerTest : public TimerTest
	{
	public:
		int func1(void);
		int func2(void);
	};

};

#endif
