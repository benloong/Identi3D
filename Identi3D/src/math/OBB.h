//
// File: OBB.h
// ===========
// Provide oriented minimum bounding box calculation.
//

#ifndef IDENTI3D_SRC_MATH_OBB_H
#define IDENTI3D_SRC_MATH_OBB_H

#include <src/identi3d/General.h>
#include <src/math/Vector.h>
#include <src/math/Matrix.h>
#include <src/math/Plane.h>

namespace Identi3D
{
	enum OBBCullResult
	{
		OBBCullResult_Visible	= 0,
		OBBCullResult_Culled	= 1,
		OBBCullResult_Clipped	= 2,

		OBBCullResult_ForceInt	= 0xFFFF
	};

	class __declspec(dllexport) OrientedBoundingBox
	{
		friend class Ray;
		friend class Plane;
		friend class AxisAlignedBoundingBox;

	private:
		float _extent[3]; // corresponding to axis 0, 1, 2.
		Vector3 _axis[3];
		Vector3 _center;

	public:
		
		OrientedBoundingBox(void) { memset(_extent, 0, sizeof(_extent)); }

		/*
		 * Detransform the box by the specified matrix.
		 */
		inline void detransform(const OrientedBoundingBox &obb, const Matrix &m)
		{
			Matrix n = m;
			Vector3 t(m(3, 0), m(3, 1), m(3, 2));
			n(3, 0) = n(3, 1) = n(3, 2) = 0.0f;

			_center = obb._center * m;
			_center += t;
			_axis[0] = obb._axis[0] * m;
			_axis[1] = obb._axis[1] * m;
			_axis[2] = obb._axis[2] * m;

			_extent[0] = obb._extent[0];
			_extent[1] = obb._extent[1];
			_extent[2] = obb._extent[2];
		}

		/*
		 * Do a cull test to the box.
		 */
		OBBCullResult cull(PlaneArray &planes) const;

		/*
		 * Intersect with triangle.
		 */
		bool intersect(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) const;

		/*
		 * Get extent on specified axis. 0 - x, 1 - y, 2 - z.
		 */
		inline float getExtent(int n) const { return (n >= 0 && n < 3) ? _extent[n] : 0.0f; }

		/*
		 * Get extent on X axis.
		 */
		inline float getExtentX(void) const { return _extent[0]; }

		/*
		 * Get extent on Y axis.
		 */
		inline float getExtentY(void) const { return _extent[1]; }

		/*
		 * Get extent on Z axis.
		 */
		inline float getExtentZ(void) const { return _extent[2]; }

		/*
		 * Get specified axis vector. 0 - x, 1 - y, 2 - z.
		 */
		inline const Vector3 getAxis(int n) const { return (n >= 0 && n < 3) ? _axis[n] : Vector3(); }

		/*
		 * Get X axis vector.
		 */
		inline const Vector3 getAxisX(void) const { return _axis[0]; }

		/*
		 * Get Y axis vector.
		 */
		inline const Vector3 getAxisY(void) const { return _axis[1]; }

		/*
		 * Get Z axis vector.
		 */
		inline const Vector3 getAxisZ(void) const { return _axis[2]; }

		/*
		 * Get center point of the box.
		 */
		inline const Vector3 getCenter(void) const { return _center; }
	};

};

#endif
