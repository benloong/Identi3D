//
// File: ray_test.cc
// =================
//

#include "ray_test.h"

using namespace std;

namespace Identi3D
{

	void RayTest::testRayTriangleIntersection(void)
	{
		Ray r(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.1f, 0.0f));
		Vector3 v0(5.0f, 3.0f, -1.0f), v1(5.0f, -3.0f, -1.0f), v2(5.0f, 0.0f, 3.0f), pt;
		float distance;
		
		cout << "RTI> Ray located on " << r.getOrigin() << " pointing toward " << 
			r.getDirection() << "." << endl;
		cout << "RTI> Intersecting with Triangle " << v0 << ", " << v1 << ", " << v2 << "." << endl;
		if(r.intersect(v0, v1, v2, false, &distance, &pt)) {
			cout << "RTI> [INTERSECTION DETECTED]" << endl;
			cout << "RTI>   Distance = " << distance << ", Hit point: " << pt << endl;
		}else{
			cout << "RTI> Nothing happens." << endl;
		}
		cout << endl;
	}

	void RayTest::testRayPlaneIntersection(void)
	{
		// TODO: Waiting Plane class to be implemented.

		//Plane p(Vector3(0, 0, 1), Vector3(0, 0, 0));
		//Ray r(Vector3(1, 1, 1), Vector3(-1, -1, -1));
		//float d;
		//Vector3 hit;

		//if(r.intersect(p, true, &d, &hit)) {
		//	puts("[INTERSECTION DETECTED]");
		//	printf(" DISTANCE = %.4f, HIT POINT: ", d);
		//	VectorTest::write_vector(hit);
		//	puts("");
		//}else{
		//	puts("nothing happens.");
		//}
	}

	void RayTest::testRayAABBIntersection(void)
	{
		AxisAlignedBoundingBox aabb(Vector3(-1, -1, 1), Vector3(1, 2, 2));
		Ray r(Vector3(0, 0, 0), Vector3(0, 1, 1));
		float d = 0.0f;
		Vector3 hit;
		
		cout << "RAI> Ray located on " << r.getOrigin() << " pointing toward " << 
			r.getDirection() << "." << endl;
		cout << "RAI> Intersecting with Axis Aligned Bounding Box from " << 
			aabb.getMin() << " to " << aabb.getMax() << "." << endl;
		if(r.intersect(aabb, &d, &hit)) {
			cout << "RAI> [INTERSECTION DETECTED]" << endl;
			cout << "RAI>   Distance = " << d << ", Hit point: " << hit << endl;
		}else{
			cout << "RAI> Nothing happens." << endl;
		}
		cout << endl;
	}

};
