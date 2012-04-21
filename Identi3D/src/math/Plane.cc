//
// File: Plane.cc
// ==============
//

#include <src/math/Plane.h>
#include <src/math/Ray.h>
#include <src/math/AABB.h>
#include <src/math/OBB.h>

namespace Identi3D
{

	bool Plane::intersect(const Plane &plane, Ray *ray) const
	{
		Vector3 c;
		float sqrlen;

		c.cross(_normal, plane._normal);
		sqrlen = c.getSquaredLength();

		if(fabs(sqrlen) < FLT_EPSILON) return false;

		if(ray)
		{
			float slen1 = _normal.getSquaredLength();
			float slen2 = plane._normal.getSquaredLength();
			float product = _normal * plane._normal;
			float det = slen1 * slen2 - product * product;

			if(fabs(sqrlen) < FLT_EPSILON) return false;

			det = 1.0f / det;
			float c1 = (slen2 * _dist - product * plane._dist) * det;
			float c2 = (slen1 * plane._dist - product * _dist) * det;

			(*ray)._direction = c;
			(*ray)._origin = _normal * c1 + plane._normal * c2;
		}

		return true;
	}

	bool Plane::intersect(const AxisAlignedBoundingBox &aabb) const
	{
		Vector3 vmin, vmax;

		if(_normal._x >= 0.0f) {
			vmin._x = aabb._bounds[0]._x;
			vmax._x = aabb._bounds[1]._x;
		} else {
			vmin._x = aabb._bounds[1]._x;
			vmax._x = aabb._bounds[0]._x;
		}
		if(_normal._y >= 0.0f) {
			vmin._y = aabb._bounds[0]._y;
			vmax._y = aabb._bounds[1]._y;
		} else {
			vmin._y = aabb._bounds[1]._y;
			vmax._y = aabb._bounds[0]._y;
		}
		if(_normal._z >= 0.0f) {
			vmin._z = aabb._bounds[0]._z;
			vmax._z = aabb._bounds[1]._z;
		} else {
			vmin._z = aabb._bounds[1]._z;
			vmax._z = aabb._bounds[0]._z;
		}

		if(_normal * vmin + _dist > 0.0f) return false;
		if(_normal * vmax + _dist < 0.0f) return false;

		return true;
	}

	bool Plane::intersect(const OrientedBoundingBox &obb) const
	{
		float r = fabs(obb._extent[0] * (_normal * obb._axis[0])) +
			fabs(obb._extent[1] * (_normal * obb._axis[1])) +
			fabs(obb._extent[2] * (_normal * obb._axis[2]));

		return (distance(obb._center) <= r);
	}

};
