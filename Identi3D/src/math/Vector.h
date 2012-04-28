//
// File: Vector.h
// ==============
// Provides vector computation ability.
//

#ifndef IDENTI3D_SRC_MATH_VECTOR_H
#define IDENTI3D_SRC_MATH_VECTOR_H

#include <src/identi3d/General.h>
#include <src/utils/DebugFrame.h>

#if !defined(_SSE_ONLY)
#include <src/utils/CPU.h>
#endif

namespace Identi3D
{

	class __declspec(dllexport) Vector3
	{
		friend class Matrix;
		friend class Ray;
		friend class Plane;
		friend class AxisAlignedBoundingBox;

	public:
		explicit Vector3(void)
			: _x(0.0f), _y(0.0f), _z(0.0f), _w(1.0f) {} ;
		explicit Vector3(float x, float y, float z)
			: _x(x), _y(y), _z(z), _w(1.0f) {} ;
		~Vector3(void) {} ;

		/*
		 * Set values of vector.
		 */
		inline void set(float x, float y, float z, float w = 1.0f)
		{
			_x = x, _y = y, _z = z, _w = w;
		}

		/*
		 * Set x of vector.
		 */
		inline void setX(float x)
		{
			_x = x;
		}

		/*
		 * Set y of vector.
		 */
		inline void setY(float y)
		{
			_y = y;
		}

		/*
		 * Set z of vector.
		 */
		inline void setZ(float z)
		{
			_z = z;
		}

		/*
		 * Get x of vector.
		 */
		inline float getX(void) const
		{
			return _x;
		}

		/*
		 * Get y of vector.
		 */
		inline float getY(void) const
		{
			return _y;
		}

		/*
		 * Get z of vector.
		 */
		inline float getZ(void) const
		{
			return _z;
		}

		/*
		 * Copy vector data via __m128's '=' operator.
		 */
		inline Vector3 &operator=(const Vector3 &vec)
		{
			_data = vec._data;
			return *this;
		}

		/*
		 * A vector plus a vector.
		 */
		inline void operator +=(const Vector3 &vec)
		{
			_mm_add_ps(_data, vec._data);
		}

		/*
		 * A vector subtracts a vector.
		 */
		inline void operator -=(const Vector3 &vec)
		{
			_mm_sub_ps(_data, vec._data);
		}

		/*
		 * Scale the vector.
		 */
		inline void operator *=(float f)
		{
			_x *= f, _y *= f, _z *= f;
		}

		/*
		 * A vector multiplies a matrix.
		 */
		inline void operator *=(const Matrix &m)
		{
			(*this) = (*this) * m;
		}

		/*
		 * A vector plus a vector.
		 */
		inline const Vector3 operator +(const Vector3 &vec) const
		{
			return Vector3(_x + vec._x, _y + vec._y, _z + vec._z);
		}

		/*
		 * A vector subtracts a vector.
		 */
		inline const Vector3 operator -(const Vector3 &vec) const
		{
			return Vector3(_x - vec._x, _y - vec._y, _z - vec._z);
		}

		/*
		 * Scale the vector.
		 */
		inline friend __declspec(dllexport) const Vector3 operator *(float f, const Vector3 &vec)
		{
			return vec * f;
		}
		
		/*
		 * Scale the vector.
		 */
		inline friend __declspec(dllexport) const Vector3 operator *(const Vector3 &vec, float f)
		{
			return Vector3(vec._x * f, vec._y * f, vec._z *f);
		}

		/*
		 * A vector multiplies a matrix.
		 */
		friend __declspec(dllexport) const Vector3 operator *(const Matrix &m, const Vector3 &v);
		
		/*
		 * A vector multiplies a matrix.
		 */
		inline friend __declspec(dllexport) const Vector3 operator *(const Vector3 &v, const Matrix &m)
		{
			return m * v;
		}

		/*
		 * Dot product between two vectors.
		 */
		inline float operator *(const Vector3 &vec) const
		{
			return (_x * vec._x + _y * vec._y + _z * vec._z);
		}

		/*
		 * Write vector data to stream.
		 */
		inline friend std::ostream __declspec(dllexport) &operator <<(std::ostream &out, const Vector3 &vec)
		{
			out << "(" << vec._x << ", " << vec._y << ", " << vec._z << ")";
			return out;
		}

		/*
		 * Get length of const vector.
		 */
		inline float getLengthConst(void) const
		{
			return sqrt(_x * _x + _y * _y + _z * _z);
		}

		/*
		 * Get length of vector.
		 */
		inline float getLength(void)
		{

#if !defined(_SSE_ONLY)

			// TODO: replace old singleton code.
			if(!CpuInfo::instance().isSSESupported()) {
				return sqrt(_x * _x + _y * _y + _z * _z);
			}

#endif // !defined(_SSE_ONLY)
		
			__m128 s;
			float result;
			_w = 0.0f;
			s = _mm_mul_ps(_data, _data);
			s = _mm_add_ps(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 3, 0, 1)));
			s = _mm_add_ps(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 3, 3)));
			result = _mm_sqrt_ss(s).m128_f32[0];
			_w = 1.0f;
			return result;
		}

		/*
		 * Normalize the vector (x, y, z < 1.0f).
		 */
		inline void normalize(void)
		{

#if !defined(_SSE_ONLY)

			if(!CpuInfo::instance().isSSESupported()) {
				float f = sqrt(_x * _x + _y * _y + _z * _z);
				if((fabs(f) > FLT_EPSILON)) _x /= f, _y /= f, _z /= f;
				return ;
			}

#endif // !defined(_SSE_ONLY)

			__m128 s;
			_w = 0.0f;
			s = _mm_mul_ps(_data, _data);
			s = _mm_add_ps(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 3, 0, 1)));
			s = _mm_add_ps(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 0, 3, 2)));
			_data = _mm_mul_ps(_mm_rsqrt_ps(s), _data);
			_w = 1.0f;
		}

		/*
		 * Calculate the cross product of two vectors.
		 * New vector is perpendicular to u, v.
		 */
		inline void cross(const Vector3 &u, const Vector3 &v)
		{

#if !defined(_SSE_ONLY)

			if(!CpuInfo::instance().isSSESupported()) {
				_x = u._y * v._z - u._z * v._y;
				_y = u._z * v._x - u._x * v._z;
				_z = u._x * v._y - u._y * v._x;
				_w = 1.0f;
				return ;
			}

#endif // !defined(_SSE_ONLY)

			__m128 s, t;
			s = _mm_shuffle_ps(v._data, v._data, _MM_SHUFFLE(3, 0, 2, 1));
			t = _mm_mul_ps(u._data, s);
			_data = _mm_sub_ps(
				_mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 2, 1)), 
				_mm_mul_ps(_mm_shuffle_ps(u._data, u._data, _MM_SHUFFLE(3, 1, 0, 2)), s));
			_w = 1.0f;
		}

		/*
		 * Get squared length (len^2).
		 */
		inline float getSquaredLength(void) const
		{
			return (_x * _x + _y * _y + _z * _z);
		}

		/*
		 * Negate the vector (x,y,z -> -x,-y,-z).
		 */
		inline void negate(void)
		{
			// TODO: add no SSE support situation.

			static const __m128 neg_vec = {-0.0f, -0.0f, -0.0f, 1.0f};
			_mm_or_ps(_data, neg_vec);
		}

		/*
		 * Calculate difference between u, v.
		 */
		inline void diff(const Vector3 &u, const Vector3 &v)
		{
			_x = u._x - v._x;
			_y = u._y - v._y;
			_z = u._z - v._z;
		}

		/*
		 * Get the angle between this & vec (const version).
		 */
		inline float getAngle(const Vector3 &vec) const
		{
			return acosf(((*this) * vec) / (getLengthConst() * vec.getLengthConst()));
		}

		/*
		 * Get the angle between this & vec.
		 */
		inline float getAngle(Vector3 &vec)
		{
			return acosf(((*this) * vec) / (getLength() * vec.getLength()));
		}

	private:

		_DATA_ALIGN union
		{
			struct { float _x, _y, _z, _w; };
			__m128 _data;
		};
	};

}

#endif // IDENTI3D_SRC_MATH_VECTOR_H
