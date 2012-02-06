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

namespace Nova3D
{

	class VectorTest
	{
	private:
		static const Vector3 regular_normalize(const Vector3 &vec)
		{
			float length = sqrt(vec._x * vec._x + vec._y * vec._y + vec._z * vec._z);
			return Vector3(vec._x / length, vec._y / length, vec._z / length);
		}

	public:
		static void write_vector(const Vector3 &vec)
		{
			printf("(%.6f, %.6f, %.6f)", vec._x, vec._y, vec._z);
		}

		static void test_vector_normalize(int n = 1)
		{
			int pass = 0;
			srand((UINT)time(NULL));
			for(int i = 0; i < n; i++) {
				Vector3 vec(float(rand() % 100), float(rand() % 100), float(rand() % 100));
				printf("Random Vector: "); write_vector(vec); puts("");
				Vector3 a = regular_normalize(vec);
				vec.normalize();
				printf("Correct answer: "); write_vector(a); puts("");
				printf("Library answer: "); write_vector(vec); puts("");
				if(_fcomp(a._x, vec._x, 0.001f) && _fcomp(a._y, vec._y, 0.001f) && _fcomp(a._z, vec._z, 0.001f))
					puts("> [PASS]"), pass++;
				else puts("> [FAIL]");
			}
			printf("TEST COMPLETE %d/%d PASS.\n", pass, n);
		}

		static void test_vector_mul_matrix()
		{
			Matrix m = {1, 2, 4, 0,
						2, 4, 0, 3,
						3, 3, 5, 1,
						0, 2, 0, 4};
			Vector3 v(3, 3, 4);
			v *= m;
			printf("vector-matrix multipie: ");
			write_vector(v);
			puts("");
			// correct answer: 2.5, 2.1, 3.9
		}

		static void test_vector_angle()
		{
			Vector3 v1(3, 3, 4), v2(6, 4, 1);
			printf("angle of (v1, v2): %.4f\n", v1.getAngle(v2).getRadian());
			// correct answer: 0.641928
		}

		static void test_vector_cross()
		{
			Vector3 v1(3, 3, 4), v2(6, 4, 1);
			Vector3 v;
			v.cross(v1, v2);
			printf("vector cross result: ");
			write_vector(v);
			puts("");
			// correct answer: (-13, 21, -6)
		}

	};

};

#endif
