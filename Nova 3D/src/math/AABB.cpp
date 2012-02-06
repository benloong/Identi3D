//
// File: AABB.cpp
// ==============
//

#include <src/math/AABB.h>

#include <src/math/Vector.h>

namespace Nova3D
{

	AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Vector3 &_min, const Vector3 &_max)
	{
		_bounds[0] = _min;
		_bounds[1] = _max;
	}

};
