//
// File: OBB.h
// ===========
// Provide oriented minimum bounding box calculation.
//

#ifndef IDENTI3D_SRC_MATH_OBB_H
#define IDENTI3D_SRC_MATH_OBB_H

#include <src/identi3d/General.h>
#include <src/math/Vector.h>

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
		void detransform(const OrientedBoundingBox &obb, const Matrix &m);

		/*
		 * Do a cull test to the box.
		 */
		int cull(const Plane *plane_arr, int plane_count);

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
