//
// File: ray_test.h
// ================
// Test ray class.
//

#ifndef CORN3D_UINTTESTS_MATHTEST_RAY_TEST_H
#define CORN3D_UINTTESTS_MATHTEST_RAY_TEST_H

#include <src/math/Ray.h>
#include <src/math/AABB.h>
#include <src/math/Plane.h>
#include "vector_test.h"

namespace Corn3D
{
	
	class RayTest
	{
	public:
		static void triangle_intersection_test(void)
		{
			Ray r(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.1f, 0.0f));
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

		static void plane_intersection_test(void)
		{
			Plane p(Vector3(0, 0, 1), Vector3(0, 0, 0));
			Ray r(Vector3(1, 1, 1), Vector3(-1, -1, -1));
			float d;
			Vector3 hit;

			if(r.intersect(p, true, &d, &hit)) {
				puts("[INTERSECTION DETECTED]");
				printf(" DISTANCE = %.4f, HIT POINT: ", d);
				VectorTest::write_vector(hit);
				puts("");
			}else{
				puts("nothing happens.");
			}
		}
		
		static void aabb_intersection_test(void)
		{
			AxisAlignedBoundingBox aabb(Vector3(-1, -1, 1), Vector3(1, 2, 2));
			Ray r(Vector3(0, 0, 0), Vector3(0, 1, 1));
			float d = 0.0f;
			Vector3 hit;

			if(r.intersect(aabb, &d, &hit)) {
				puts("[INTERSECTION DETECTED]");
				printf(" DISTANCE = %.4f, HIT POINT: ", d);
				VectorTest::write_vector(hit);
				puts("");
			}else{
				puts("nothing happens.");
			}
		}

	};

};

#endif
