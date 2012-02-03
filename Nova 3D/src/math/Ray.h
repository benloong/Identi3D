//
// File: Ray.h
// ===========
// Ray definition.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_SRC_MATH_RAY_H
#define NOVA3D_SRC_MATH_RAY_H

#include <src/nova-3d/NovaGeneral.h>
#include <src/math/Vector.h>

namespace Nova3D
{

	class Ray
	{
	public:
		Vector3 _origin, _direction;

		Ray(void) {} ;
		Ray(const Vector3 &origin, const Vector3 &direction) 
			{ set(origin, direction); };
		~Ray(void) {} ;

		void set(const Vector3 &origin, const Vector3 &direction);
		void detransform(const Matrix &m);

		bool intersect(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, 
					   bool cull_backside = false, float *distance = NULL, Vector3 *hit = NULL) const;
	};

};

#endif
