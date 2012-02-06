//
// File: AABB.h
// ============
// Provide axis-aligned minimum bounding box calculation.
//

#ifndef NOVA3D_SRC_MATH_AABB_H
#define NOVA3D_SRC_MATH_AABB_H

#include <src/nova-3d/NovaGeneral.h>
#include <src/math/Vector.h>

namespace Nova3D
{

	class AxisAlignedBoundingBox
	{
		friend class Ray;

	private:
		Vector3 _bounds[2];

	public:
		AxisAlignedBoundingBox(void) {} ;
		AxisAlignedBoundingBox(const Vector3 &_min, const Vector3 &_max);
		AxisAlignedBoundingBox(const OrientedBoundingBox &obb);
		~AxisAlignedBoundingBox(void) {} ;

		int cull(const Plane *plane_arr, int plane_count);
		void getPlane(Plane *p);
		bool Contains(const Ray &r, float length);

		inline const Vector3 &getMin(void) const { return _bounds[0]; }
		inline const Vector3 &getMax(void) const { return _bounds[1]; }
	};

};

#endif
