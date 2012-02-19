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
		friend class Matrix;

	public:
		Vector3(void) : 
		  _x(0.0f), _y(0.0f), _z(0.0f), _w(1.0f) {} ;
		Vector3(float x, float y, float z) :
		  _x(x), _y(y), _z(z), _w(1.0f) {} ;
		~Vector3(void) {} ;

		/*
		 * Set values of vector.
		 */
		void set(float x, float y, float z, float w = 1.0f);
		void setX(float x) { _x = x; } ;
		void setY(float y) { _y = y; } ;
		void setZ(float z) { _z = z; } ;

		/*
		 * Get values of vector.
		 */
		float getX(void) const { return _x; }
		float getY(void) const { return _y; }
		float getZ(void) const { return _z; }

		/*
		 * Copy vector data via __m128 '=' operator.
		 */
		Vector3 &operator =(const Vector3 &vec) { _data = vec._data; return *this; } ;

		/*
		 * Basic vector operation.
		 */
		void operator +=(const Vector3 &vec);
		void operator -=(const Vector3 &vec);
		void operator *=(float f);
		void operator *=(const Matrix &m);

		const Vector3 operator +(const Vector3 &vec) const;
		const Vector3 operator -(const Vector3 &vec) const;
		const Vector3 operator *(float f) const;
		const Vector3 operator *(const Matrix &m) const;
		float operator *(const Vector3 &vec) const;

		/*
		 * Get length of vector.
		 */
		float getLength(void);

		/*
		 * Normalize the vector (x, y, z < 1.0f).
		 */
		void normalize(void);

		/*
		 * Calculate the cross product of two vectors.
		 * New vector is perpendicular to u, v.
		 */
		void cross(const Vector3 &u, const Vector3 &v);

		/*
		 * Get squared length (len^2).
		 */
		float getSquaredLength(void) const;

		/*
		 * Negate the vector (x,y,z -> -x,-y,-z).
		 */
		void negate(void);

		/*
		 * Calculate difference between u, v.
		 */
		void diff(const Vector3 &u, const Vector3 &v);

		/*
		 * Get the angle between this & vec.
		 */
		const Angle getAngle(Vector3 &vec);

	private:
		/*
		 * Data should be aligned to 16 bit for fast SSE computation.
		 */
		_DATA_ALIGN union
		{
			struct { float _x, _y, _z, _w; };
			__m128 _data;
		};
	};

}

#endif // NOVA3D_SRC_MATH_VECTOR_H
