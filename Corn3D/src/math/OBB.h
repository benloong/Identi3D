//
// File: OBB.h
// ===========
// Provide oriented minimum bounding box calculation.
//

#ifndef CORN3D_SRC_MATH_OBB_H
#define CORN3D_SRC_MATH_OBB_H

#include <src/corn3d/General.h>

namespace Corn3D
{

	class __declspec(dllexport) OrientedBoundingBox
	{
		friend class Ray;

	private:
		float _extent[3]; // corresponding to axis 0, 1, 2.
		Vector3 _axis[3];
		Vector3 _center;

	public:
		void detransform(const OrientedBoundingBox &obb, const Matrix &m);
		int cull(const Plane *plane_arr, int plane_count);

		inline float getExtent(int n) const { return (n >= 0 && n < 3) ? _extent[n] : 0.0f; }
		inline float getExtentX(void) const { return _extent[0]; }
		inline float getExtentY(void) const { return _extent[1]; }
		inline float getExtentZ(void) const { return _extent[2]; }

		inline const Vector3 getAxis(int n) const { return (n >= 0 && n < 3) ? _axis[n] : Vector3(); }
		inline const Vector3 getAxisX(void) const { return _axis[0]; }
		inline const Vector3 getAxisY(void) const { return _axis[1]; }
		inline const Vector3 getAxisZ(void) const { return _axis[2]; }
		inline const Vector3 getCenter(void) const { return _center; }
	};

};

#endif
