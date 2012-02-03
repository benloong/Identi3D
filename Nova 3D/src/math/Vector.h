//
// File: Vector.h
// ==============
// Provides vector computation ability.
//

#ifndef NOVA3D_SRC_MATH_VECTOR_H
#define NOVA3D_SRC_MATH_VECTOR_H

#include <src/nova-3d/NovaGeneral.h>
#include <src/math/Angle.h>

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

		Vector3(void)
			{ set(0.0f, 0.0f, 0.0f); }
		Vector3(float x, float y, float z)
			{ set(x, y, z); }
		~Vector3(void) {} ;

		void set(float x, float y, float z, float w = 1.0f) 
			{ _x = x; _y = y; _z = z; _w = w; }
		
		bool operator ==(const Vector3 &vec);
		Vector3 &operator =(const Vector3 &vec);

		void operator +=(const Vector3 &vec);
		void operator -=(const Vector3 &vec);
		void operator *=(float f);
		void operator *=(const Matrix &m);
		void operator /=(float f);
		
		const Vector3 operator +(const Vector3 &vec) const;
		const Vector3 operator -(const Vector3 &vec) const;
		const Vector3 operator *(float f) const;
		const Vector3 operator *(const Matrix &m) const;
		float operator *(const Vector3 &vec) const;
		const Vector3 operator /(float f) const;

		float getLength(void);
		void normalize(void);
		void cross(const Vector3 &u, const Vector3 &v);

		float getSquaredLength(void) const 
			{ return (_x * _x + _y * _y + _z * _z); }
		void negate(void) 
			{ _x = -_x, _y = -_y, _z = -_z; }
		void diff(const Vector3 &u, const Vector3 &v) 
			{ _x = u._x - v._x, _y = u._y - v._y, _z = u._z - v._z; }
		const Angle getAngle(Vector3 &vec) 
			{ return Angle((Radian)acos(((*this) * vec) / (getLength() * vec.getLength()))); }

		float getX(void) const { return _x; }
		float getY(void) const { return _y; }
		float getZ(void) const { return _z; }
	};

}

#endif
