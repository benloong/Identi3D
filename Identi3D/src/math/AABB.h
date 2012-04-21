//
// File: AABB.h
// ============
// Provide axis-aligned minimum bounding box calculation.
//

#ifndef IDENTI3D_SRC_MATH_AABB_H
#define IDENTI3D_SRC_MATH_AABB_H

#include <src/identi3d/General.h>
#include <src/utils/DebugManager.h>
#include <src/math/Plane.h>
#include <src/math/Vector.h>
#include <src/math/Ray.h>

namespace Identi3D
{

	enum AABBCullResult
	{
		AABBCullResult_Visible	= 0,
		AABBCullResult_Culled	= 1,
		AABBCullResult_Clipped	= 2,

		AABBCullResult_ForceInt	= 0xFFFF
	};

	class __declspec(dllexport) AxisAlignedBoundingBox
	{
		friend class Ray;
		friend class Plane;
		friend class Polygon;

	private:
		Vector3 _bounds[2];

	public:
		
		/*
		 * Constructor: Do nothing.
		 */
		AxisAlignedBoundingBox(void) {} ;

		/*
		 * Constructor: Set AABB to specified minimum & maximum vertex.
		 */
		AxisAlignedBoundingBox(const Vector3 &min, const Vector3 &max)
		{			
			_bounds[0] = min;
			_bounds[1] = max;
		}

		/*
		 * Constructor: Get AABB from OBB.
		 */
		AxisAlignedBoundingBox(const OrientedBoundingBox &obb)
		{
			construct(obb);
		}

		/*
		 * Destructor: Do nothing.
		 */
		~AxisAlignedBoundingBox(void) {} ;

		/*
		 * Construct an Axis Aligned Bounding Box from an Oriented Bounding Box.
		 */
		void construct(const OrientedBoundingBox &obb);

		/*
		 * Do a cull test to Axis Aligned Bounding Box.
		 * On MSVC2010: Hack <vector> at line 879 or around, changing _Ty _Val to _Ty &_Val.
		 */
		AABBCullResult cull(PlaneArray &planes) const;

		/*
		 * Get 6 planes of the box. ENSURE the plane array is larger or equal than 6.
		 * Order: RIGHT LEFT FRONT BACK TOP BOTTOM.
		 */
		void getPlanes(PlaneArray &planes)
		{
			planes.resize(6);
			planes[0].set(Vector3(1.0f, 0.0f, 0.0f), _bounds[1]);
			planes[1].set(Vector3(-1.0f, 0.0f, 0.0f), _bounds[0]);
			planes[2].set(Vector3(0.0f, 0.0f, -1.0f), _bounds[0]);
			planes[3].set(Vector3(0.0f, 0.0f, 1.0f), _bounds[1]);
			planes[4].set(Vector3(0.0f, 1.0f, 0.0f), _bounds[1]);
			planes[5].set(Vector3(0.0f, -1.0f, 0.0f), _bounds[0]);
		}

		/*
		 * Check if the point is inside the box.
		 */
		inline bool contains(const Vector3 &point)
		{
			return ((point._x > _bounds[0]._x && point._x < _bounds[1]._x) &&
				(point._y > _bounds[0]._y && point._y < _bounds[1]._y) &&
				(point._z > _bounds[0]._z && point._z < _bounds[1]._z));
		}

		/*
		 * Check if the segement is inside the box.
		 */
		inline bool contains(const Ray &ray, float length)
		{
			return (contains(ray._origin) && contains(ray._origin + ray._direction * length));
		}

		/*
		 * Get the minimum vertex of the box.
		 */
		inline const Vector3 &getMin(void) const { return _bounds[0]; }
		
		/*
		 * Get the maximum vertex of the box.
		 */
		inline const Vector3 &getMax(void) const { return _bounds[1]; }
	};

};

#endif
