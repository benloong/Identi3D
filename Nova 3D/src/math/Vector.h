//
// File: Vector.h
// ==============
// Provides vector computation ability.
//

#ifndef NOVA3D_SRC_MATH_VECTOR_H
#define NOVA3D_SRC_MATH_VECTOR_H

#include "src/nova-3d/NovaGeneral.h"
#include <cmath>
#include <xmmintrin.h>

namespace Nova3D
{

	class Vector3
	{
	public:
		__declspec(align(16)) union
		{
			__m128 data;
			struct
			{
				float _x, _y, _z, _w;
			};
		};

		//float _x, _y, _z, _w;

		Vector3(void);
		Vector3(float x, float y, float z);
		~Vector3(void);

		inline void set(float x, float y, float z, float w = 1.0f) { _x = x; _y = y; _z = z; _w = w; }

		Vector3 &operator =(const Vector3 &vec);

		void operator +=(const Vector3 &vec);
		void operator -=(const Vector3 &vec);
		void operator *=(float f);
		void operator /=(float f);
		
		const Vector3 operator +(const Vector3 &vec) const;
		const Vector3 operator -(const Vector3 &vec) const;
		const Vector3 operator *(float f) const;
		float operator *(const Vector3 &vec) const;
		const Vector3 operator /(float f) const;

		float getLength(void);
		void normalize(void);
		void cross(const Vector3 &u, const Vector3 &v);

		inline float getSquaredLength(void) const { return (_x * _x + _y * _y + _z * _z); }
		inline void negate(void) { _x = -_x, _y = -_y, _z = -_z; }
		inline void diff(const Vector3 &u, const Vector3 &v) { _x = u._x - v._x, _y = u._y - v._y, _z = u._z - v._z; }
		inline float angleWith(Vector3 &vec) { return (float)acos(((*this) * vec) / (getLength() * vec.getLength())); }

		inline float getX(void) const { return _x; }
		inline float getY(void) const { return _y; }
		inline float getZ(void) const { return _z; }
	};

}

#endif
