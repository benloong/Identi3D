//
// File: ray_test.h
// ================
// Test ray class.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_UINTTESTS_MATHTEST_RAY_TEST_H
#define NOVA3D_UINTTESTS_MATHTEST_RAY_TEST_H

#include <src/math/Ray.h>
#include "vector_test.h"

namespace Nova3D
{
	
	class RayTest
	{
	public:
		static void triangle_intersection_test(void)
		{
			Ray r(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
			Vector3 v0(5.0f, 3.0f, -1.0f), v1(5.0f, -3.0f, -1.0f), v2(5.0f, 0.0f, 3.0f), pt;
			float distance;

			if(r.intersect(v0, v1, v2, false, &distance, &pt)) {
				puts("[INTERSECTION DETECTED]");
				printf(" DISTANCE = %.4f, HIT POINT: ", distance);
				VectorTest::write_vector(pt);
				puts("");
			}else{
				puts("nothing happens.");
			}
		}
	};

};

#endif
