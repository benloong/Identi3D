//
// File: Plane.h
// =============
// Class to construct a plane.
//

#ifndef IDENTI3D_SRC_MATH_PLANE_H
#define IDENTI3D_SRC_MATH_PLANE_H

#include <src/identi3d/General.h>
#include <src/math/AlignedAllocator.h>
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

	typedef std::vector<Plane, AlignedAllocator<Plane, 16> > PlaneList;

	class __declspec(dllexport) Plane
	{
		friend class Ray;
		friend class Polygon;
		friend class AxisAlignedBoundingBox;
		friend class OrientedBoundingBox;

	private:
		Vector3 _normal,	// normal of the plane
				_point;		// point on the plane
		float	_dist;		// distance to origin

	public:

		/*
		 * Constructor: Do nothing.
		 */
		Plane(void) : _dist(0.0f) {} ;

		/*
		 * Constructor: Set plane from normal vector and a point on the plane.
		 */
		Plane(const Vector3 &normal, const Vector3 &point)
		{
			_normal = normal;
			_point = point;
			_dist = -(normal * point);
		}

		/*
		 * Constructor: Set plane from normal, point and distance to origin.
		 */
		Plane(const Vector3 &normal, const Vector3 &point, float distance)
		{
			_normal = normal;
			_point = point;
			_dist = distance;
		}

		/*
		 * Constructor: Set plane from three points on the plane.
		 */
		Plane(const Vector3 &point0, const Vector3 &point1, const Vector3 &point2)	
		{
			Vector3 v1, v2;

			v1 = point1 - point0;
			v2 = point2 - point0;
			_normal.cross(v1, v2);
			_dist = _normal * point0;
			_point = point0;
		}

		/*
		 * Create a plane using its normal and a point on the plane.
		 */
		inline void set(const Vector3 &normal, const Vector3 &point)
		{
			_normal = normal;
			_point = point;
			_dist = -(normal * point);
		}

		/*
		 * Create a plane using its normal, a point on the plane and its distance to the origin.
		 */
		inline void set(const Vector3 &normal, const Vector3 &point, float distance)
		{
			_normal = normal;
			_point = point;
			_dist = distance;
		}

		/*
		 * Create a plane using two vectors formed from three points.
		 */
		inline void set(const Vector3 &point0, const Vector3 &point1, const Vector3 &point2)
		{
			Vector3 v1, v2;

			v1 = point1 - point0;
			v2 = point2 - point0;
			_normal.cross(v1, v2);
			_dist = _normal * point0;
			_point = point0;
		}
		
		/*
		 * Calculate the distance from point to plane.
		 */
		inline float distance(const Vector3 &point) const
		{
			return (fabs(_normal * point) - _dist);
		}

		/*
		 * Classify the point with respect to plane.
		 */
		inline PlanePosition classify(const Vector3 &point) const
		{
			float dist = fabs(_normal * point) + _dist;

			if(dist > FLT_EPSILON) return PlanePosition_Front;
			if(dist < FLT_EPSILON) return PlanePosition_Back;
			return PlanePosition_Planar;
		}

		/*
		 * Test intersection with a triangle.
		 */
		inline bool intersect(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) const
		{
			PlanePosition pos = classify(v0);
			return ((pos == classify(v1)) && (pos == classify(v2)));
		}

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