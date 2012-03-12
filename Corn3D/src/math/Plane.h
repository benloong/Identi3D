//
// File: Plane.h
// =============
// Class to construct a plane.
//

#ifndef CORN3D_SRC_MATH_PLANE_H
#define CORN3D_SRC_MATH_PLANE_H

#include <src/corn3d/General.h>
#include <src/math/Angle.h>
#include <src/math/Vector.h>

namespace Corn3D
{

	enum PointPosition
	{
		PointPosition_PlaneFront,
		PointPosition_PlaneBack,
		PointPosition_Planar,

		PointPosition_ForceInt	= 65535
	};

	class __declspec(dllexport) Plane
	{
	public:
		Vector3 _normal,
				_point;
		float	_distance;

		Plane(void) { _distance = 0; }
		Plane(const Vector3 &normal, const Vector3 &point)
			{ set(normal, point); }
		Plane(const Vector3 &normal, const Vector3 &point, float distance)
			{ set(normal, point, distance); }
		Plane(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2)
			{ set(v0, v1, v2); }

		void	set(const Vector3 &normal, const Vector3 &point)
			{ _normal = normal, _point = point, _distance = -(normal * point); }
		void	set(const Vector3 &normal, const Vector3 &point, float distance)
			{ _normal = normal, _point = point, _distance = distance; }
		void	set(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2)
			{ _normal.cross(v1 - v0, v2 - v0); _distance = _normal * v0; }
		
		PointPosition classify(const Vector3 &point)
		{ 
			float f = point * _normal + _distance;
			if(f > _DEFAULT_EPSILON) return PointPosition_PlaneFront;
			else if(f < -_DEFAULT_EPSILON) return PointPosition_PlaneBack;
			return PointPosition_Planar;
		}

		float	distance(const Vector3 &point)
			{ return abs(_normal * point - _distance); }
		bool	isIntersected(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2)
			{ return !(classify(v0) == classify(v1) == classify(v2)); }

		// TODO complete implementation.
	};

};

#endif