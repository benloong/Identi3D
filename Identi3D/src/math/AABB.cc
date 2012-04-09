//
// File: AABB.cpp
// ==============
//

#include <src/math/AABB.h>
#include <src/math/OBB.h>
#include <src/math/Vector.h>
#include <src/math/Plane.h>
#include <src/math/Ray.h>

namespace Identi3D
{

	AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Vector3 &min, const Vector3 &max)
		 : DebugFrame(NULL)
	{
		_bounds[0] = min;
		_bounds[1] = max;
	}

	AxisAlignedBoundingBox::AxisAlignedBoundingBox(const OrientedBoundingBox &obb)
		 : DebugFrame(NULL)
	{
		construct(obb);
	}

	void AxisAlignedBoundingBox::construct(const OrientedBoundingBox &obb)
	{
		Vector3 axis0, axis1, axis2;

		axis0 = obb._axis[0] * obb._extent[0];
		axis1 = obb._axis[1] * obb._extent[1];
		axis2 = obb._axis[2] * obb._extent[2];

		if(axis0._x > axis1._x) {
			if(axis0._x > axis2._x) {
				_bounds[0]._x = -axis0._x;
				_bounds[1]._x = axis0._x;
			} else {
				_bounds[0]._x = -axis2._x;
				_bounds[1]._x = axis2._x;
			}
		} else {
			if(axis1._x > axis2._x) {
				_bounds[0]._x = -axis1._x;
				_bounds[1]._x = axis1._x;
			} else {
				_bounds[0]._x = -axis2._x;
				_bounds[1]._x = axis2._x;
			}
		}
		
		if(axis0._y > axis1._y) {
			if(axis0._y > axis2._y) {
				_bounds[0]._y = -axis0._y;
				_bounds[1]._y = axis0._y;
			} else {
				_bounds[0]._y = -axis2._y;
				_bounds[1]._y = axis2._y;
			}
		} else {
			if(axis1._y > axis2._y) {
				_bounds[0]._y = -axis1._y;
				_bounds[1]._y = axis1._y;
			} else {
				_bounds[0]._y = -axis2._y;
				_bounds[1]._y = axis2._y;
			}
		}
		
		if(axis0._z > axis1._z) {
			if(axis0._z > axis2._z) {
				_bounds[0]._z = -axis0._z;
				_bounds[1]._z = axis0._z;
			} else {
				_bounds[0]._z = -axis2._z;
				_bounds[1]._z = axis2._z;
			}
		} else {
			if(axis1._z > axis2._z) {
				_bounds[0]._z = -axis1._z;
				_bounds[1]._z = axis1._z;
			} else {
				_bounds[0]._z = -axis2._z;
				_bounds[1]._z = axis2._z;
			}
		}
		
		_bounds[0] += obb._center;
		_bounds[1] += obb._center;
	}

	AABBCullResult AxisAlignedBoundingBox::cull(const Plane *plane_array, int plane_count) const
	{
		Vector3 vmin, vmax;
		bool intersected;

		for(int i = 0; i < plane_count; i++)
		{
			if(plane_array[i]._normal._x >= 0.0f) {
				vmin._x = _bounds[0]._x;
				vmax._x = _bounds[1]._x;
			} else {
				vmin._x = _bounds[1]._x;
				vmax._x = _bounds[0]._x;
			}
			
			if(plane_array[i]._normal._y >= 0.0f) {
				vmin._y = _bounds[0]._y;
				vmax._y = _bounds[1]._y;
			} else {
				vmin._y = _bounds[1]._y;
				vmax._y = _bounds[0]._y;
			}
			
			if(plane_array[i]._normal._z >= 0.0f) {
				vmin._z = _bounds[0]._z;
				vmax._z = _bounds[1]._z;
			} else {
				vmin._z = _bounds[1]._z;
				vmax._z = _bounds[0]._z;
			}

			if(((plane_array[i]._normal * vmin) + plane_array[i]._dist) > 0.0f)
				return AABBCullResult_Culled;

			if(((plane_array[i]._normal * vmax) + plane_array[i]._dist) >= 0.0f)
				intersected = true;
		}

		if(intersected) return AABBCullResult_Clipped;
		return AABBCullResult_Visible;
	}

	void AxisAlignedBoundingBox::getPlanes(Plane *planes)
	{
		if(!planes) return ;

		planes[0].set(Vector3(1.0f, 0.0f, 0.0f), _bounds[1]);
		planes[1].set(Vector3(-1.0f, 0.0f, 0.0f), _bounds[0]);
		planes[2].set(Vector3(0.0f, 0.0f, -1.0f), _bounds[0]);
		planes[3].set(Vector3(0.0f, 0.0f, 1.0f), _bounds[1]);
		planes[4].set(Vector3(0.0f, 1.0f, 0.0f), _bounds[1]);
		planes[5].set(Vector3(0.0f, -1.0f, 0.0f), _bounds[0]);
	}

	bool AxisAlignedBoundingBox::contains(const Vector3 &point)
	{
		return ((point._x > _bounds[0]._x && point._x < _bounds[1]._x) &&
			(point._y > _bounds[0]._y && point._y < _bounds[1]._y) &&
			(point._z > _bounds[0]._z && point._z < _bounds[1]._z));
	}

	bool AxisAlignedBoundingBox::contains(const Ray &ray, float length)
	{
		return (contains(ray._origin) && contains(ray._origin + ray._direction * length));
	}

};
