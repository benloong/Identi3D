//
// File: AABB.h
// ============
// Provide axis-aligned minimum bounding box calculation.
//

#ifndef IDENTI3D_SRC_MATH_AABB_H
#define IDENTI3D_SRC_MATH_AABB_H

#include <src/identi3d/General.h>
#include <src/math/Vector.h>

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

	private:
		Vector3 _bounds[2];

	public:
		AxisAlignedBoundingBox(void) {} ;
		AxisAlignedBoundingBox(const Vector3 &min, const Vector3 &max);
		AxisAlignedBoundingBox(const OrientedBoundingBox &obb);
		~AxisAlignedBoundingBox(void) {} ;

		/*
		 * Construct an Axis Aligned Bounding Box from an Oriented Bounding Box.
		 */
		void construct(const OrientedBoundingBox &obb);

		/*
		 * Do a cull test to Axis Aligned Bounding Box.
		 */
		AABBCullResult cull(const Plane *plane_array, int plane_count) const;

		/*
		 * Get 6 planes of the box. ENSURE the plane array is larger or equal than 6.
		 * Order: RIGHT LEFT FRONT BACK TOP BOTTOM.
		 */
		void getPlanes(Plane *planes);

		/*
		 * Check if the point is inside the box.
		 */
		bool contains(const Vector3 &point);

		/*
		 * Check if the segement is inside the box.
		 */
		bool contains(const Ray &ray, float length);

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
