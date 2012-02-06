//
// File: OBB.h
// ===========
// Provide oriented minimum bounding box calculation.
//

#ifndef NOVA3D_SRC_MATH_OBB_H
#define NOVA3D_SRC_MATH_OBB_H

#include <src/nova-3d/NovaGeneral.h>

namespace Nova3D
{

	class OrientedBoundingBox
	{
	private:
		float _half_width, _half_height, _half_depth; // corresponding to axis 0, 1, 2.
		Vector3 _axis0, _axis1, _axis2;
		Vector3 _center;

	public:
		void detransform(const OrientedBoundingBox &obb, const Matrix &m);
		int cull(const Plane *plane_arr, int plane_count);

		inline float getHalfWidth(void) const { return _half_width; }
		inline float getHalfHeight(void) const { return _half_height; }
		inline float getHalfDepth(void) const { return _half_depth; }

		inline const Vector3 getAxis0(void) const { return _axis0; }
		inline const Vector3 getAxis1(void) const { return _axis1; }
		inline const Vector3 getAxis2(void) const { return _axis2; }
	};

};

#endif
