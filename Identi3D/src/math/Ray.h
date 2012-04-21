//
// File: Ray.h
// ===========
// Ray definition.
//

#ifndef IDENTI3D_SRC_MATH_RAY_H
#define IDENTI3D_SRC_MATH_RAY_H

#include <src/identi3d/General.h>
#include <src/math/Vector.h>
#include <src/math/Matrix.h>

namespace Identi3D
{

	class __declspec(dllexport) Ray
	{
		friend class Plane;
		friend class AxisAlignedBoundingBox;

	private:
		Vector3 _origin, _direction;

		// for fast intersection calculation.
		int _sign[3];
		Vector3 _inverse;

	public:

		/*
		 * Constructor: Do nothing.
		 */
		Ray(void) {} ;
		
		/*
		 * Constructor: Set origin and direction.
		 */
		Ray(const Vector3 &origin, const Vector3 &direction)
		{
			set(origin, direction);
		}

		/*
		 * Destructor: Do nothing.
		 */
		~Ray(void) {} ;

		/*
		 * Create a ray using its origin and direction.
		 */
		inline void set(const Vector3 &origin, const Vector3 &direction)
		{
			_origin = origin;
			_direction = direction;
			_direction.normalize();

			_inverse.set(1.0f / _direction._x, 
						 1.0f / _direction._y, 
						 1.0f / _direction._z);
			_sign[0] = _inverse._x < 0;
			_sign[1] = _inverse._y < 0;
			_sign[2] = _inverse._z < 0;
		}

		/*
		 * Detransform the ray by the specified matrix.
		 */
		inline void detransform(const Matrix &m)
		{	
			Matrix s(m);
		
			_origin._x -= s._d1;
			_origin._y -= s._d2;
			_origin._z -= s._d3;
		
			s.translate(0.0f, 0.0f, 0.0f);
			s.inverse(s);

			_origin *= s;
			_direction *= s;
		}

		/*
		 * Test intersection with a triangle.
		 */
		bool intersect(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, 
					   bool cull_backside = false, float *distance = NULL, Vector3 *hit = NULL) const;
		/*
		 * Test intersection with a plane.
		 */
		bool intersect(const Plane &p, bool cull_backside = false,
					   float *distance = NULL, Vector3 *hit = NULL) const;

		/*
		 * Test intersection with an Axis Aligned Bounding Box.
		 */
		bool intersect(const AxisAlignedBoundingBox &aabb, 
					   float *distance = NULL, Vector3 *hit = NULL) const;
		
		/*
		 * Test intersection with an Oriented Bounding Box.
		 */
		bool intersect(const OrientedBoundingBox &obb,
					   float *distance = NULL, Vector3 *hit = NULL) const;

		/*
		 * Get the origin of the ray.
		 */
		inline const Vector3 &getOrigin(void) const { return _origin; }

		/*
		 * Get the direction of the ray.
		 */
		inline const Vector3 &getDirection(void) const { return _direction; }
	};

};

#endif
