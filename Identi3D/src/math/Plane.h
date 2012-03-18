//
// File: Plane.h
// =============
// Class to construct a plane.
//

#ifndef IDENTI3D_SRC_MATH_PLANE_H
#define IDENTI3D_SRC_MATH_PLANE_H

#include <src/identi3d/General.h>
#include <src/math/Angle.h>
#include <src/math/Vector.h>

namespace Identi3D
{

	enum PlanePosition
	{
		PlanePosition_Front,
		PlanePosition_Back,
		PlanePosition_Planar,

		PlanePosition_ForceInt	= 65535
	};

	class __declspec(dllexport) Plane
	{
		friend class AxisAlignedBoundingBox;
		friend class Ray;

	private:
		Vector3 _normal,	// normal of the plane
				_point;		// point on the plane
		float	_dist;		// distance to origin

	public:

		Plane(void) : _dist(0.0f) {} ;
		Plane(const Vector3 &normal, const Vector3 &point);
		Plane(const Vector3 &normal, const Vector3 &point, float distance);
		Plane(const Vector3 &point0, const Vector3 &point1, const Vector3 &point2);

		/*
		 * Create a plane using its normal and a point on the plane.
		 */
		void set(const Vector3 &normal, const Vector3 &point);

		/*
		 * Create a plane using its normal, a point on the plane and its distance to the origin.
		 */
		void set(const Vector3 &normal, const Vector3 &point, float distance);

		/*
		 * Create a plane using two vectors formed from three points.
		 */
		void set(const Vector3 &point0, const Vector3 &point1, const Vector3 &point2);
		
		/*
		 * Calculate the distance from point to plane.
		 */
		float distance(const Vector3 &point) const;

		/*
		 * Classify the point with respect to plane.
		 */
		PlanePosition classify(const Vector3 &point) const;

		/*
		 * Test intersection with a triangle.
		 */
		bool intersect(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) const;

		/*
		 * Test intersection with a plane.
		 */
		bool intersect(const Plane &plane, Ray *ray = NULL) const;

		/*
		 * Test intersection with an Axis Aligned Bounding Box.
		 */
		bool intersect(const AxisAlignedBoundingBox &aabb) const;

		/*
		 * Test intersection with an Oriented Bounding Box.
		 */
		bool intersect(const OrientedBoundingBox &obb) const;
	};

};

#endif