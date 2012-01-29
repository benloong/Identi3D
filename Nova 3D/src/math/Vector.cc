//
// File: Vector.cc
// ===============
//

#include <src/math/Vector.h>
#include <src/math/Matrix.h>
#include <src/utils/CPU.h>

namespace Nova3D
{
	
	bool Vector3::operator ==(const Vector3 &vec)
	{
		register float k = _y / vec._y;
		return (_fcomp(_x / vec._x, k) && _fcomp(k, _z / vec._z));
	}

	Vector3 &Vector3::operator =(const Vector3 &vec)
	{
		if((&vec) == this) return (*this);
		data = vec.data;
		return (*this);
	}

	void Vector3::operator +=(const Vector3 &vec)
	{
		_x += vec._x;
		_y += vec._y;
		_z += vec._z;
	}

	void Vector3::operator -=(const Vector3 &vec)
	{
		_x -= vec._x;
		_y -= vec._y;
		_z -= vec._z;
	}

	void Vector3::operator *=(float f)
	{
		_x *= f;
		_y *= f;
		_z *= f;
	}

	void Vector3::operator *=(const Matrix &m)
	{
		(*this) = (*this) * m;
	}

	void Vector3::operator /=(float f)
	{
		if(_fzero(f)) return ;
		_x /= f;
		_y /= f;
		_z /= f;
	}

	const Vector3 Vector3::operator +(const Vector3 &vec) const
	{
		return Vector3(_x + vec._x, _y + vec._y, _z + vec._z);
	}

	const Vector3 Vector3::operator -(const Vector3 &vec) const
	{
		return Vector3(_x - vec._x, _y - vec._y, _z - vec._z);
	}

	const Vector3 Vector3::operator *(float f) const
	{
		return Vector3(_x * f, _y * f, _z * f);
	}

	const Vector3 Vector3::operator *(const Matrix &m) const
	{
		Vector3 vec;

#ifndef _SSE_ONLY
		if(!CpuInfo::getInstance().isSSESupported()) {
			vec._x = _x * m._a1 + _y * m._b1 + _z * m._c1 + m._d1;
			vec._y = _x * m._a2 + _y * m._b2 + _z * m._c2 + m._d2;
			vec._z = _x * m._a3 + _y * m._b3 + _z * m._c3 + m._d3;
			vec._w = _x * m._a4 + _y * m._b4 + _z * m._c4 + m._d4;

			vec._x /= vec._w;
			vec._y /= vec._w;
			vec._z /= vec._w;
			vec._w = 1.0f;
			return vec;
		}
#endif

		vec.data = _mm_add_ps(_mm_add_ps(_mm_add_ps(
			_mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(0, 0, 0, 0)), m.data[0]),
			_mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(1, 1, 1, 1)), m.data[1])),
			_mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(2, 2, 2, 2)), m.data[2])),
			m.data[3]);
		vec.data = _mm_div_ps(vec.data, _mm_shuffle_ps(vec.data, vec.data, _MM_SHUFFLE(3, 3, 3, 3)));
		return vec;
	}
	
	float Vector3::operator *(const Vector3 &vec) const
	{
		return (_x * vec._x + _y * vec._y + _z * vec._z);
	}
	
	const Vector3 Vector3::operator /(float f) const
	{
		if(_fzero(f)) return (*this);
		return Vector3(_x / f, _y / f, _z / f);
	}

	float Vector3::getLength(void)
	{
		
#ifndef _SSE_ONLY
		if(!CpuInfo::getInstance().isSSESupported()) {
			return sqrt(_x * _x + _y * _y + _z * _z);
		}
#endif
		
		__m128 s;
		float result;
		_w = 0.0f;
		s = _mm_mul_ps(data, data);
		s = _mm_add_ps(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 3, 0, 1)));
		s = _mm_add_ps(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 3, 3)));
		result = _mm_sqrt_ss(s).m128_f32[0];
		_w = 1.0f;
		return result;
	}

	void Vector3::normalize(void)
	{

#ifndef _SSE_ONLY
		if(!CpuInfo::getInstance().isSSESupported()) {
			float f = sqrt(_x * _x + _y * _y + _z * _z);
			if(!_fzero(f)) _x /= f, _y /= f, _z /= f;
			return ;
		}
#endif

		__m128 s;
		_w = 0.0f;
		s = _mm_mul_ps(data, data);
		s = _mm_add_ps(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 3, 0, 1)));
		s = _mm_add_ps(s, _mm_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 3, 3)));
		s = _mm_rsqrt_ps(s);
		data = _mm_mul_ps(s, data);
		_w = 1.0;
	}

	void Vector3::cross(const Vector3 &u, const Vector3 &v)
	{

#ifndef _SSE_ONLY
		if(!CpuInfo::getInstance().isSSESupported()) {
			_x = u._y * v._z - u._z * v._y;
			_y = u._z * v._x - u._x * v._z;
			_z = u._x * v._y - u._y * v._x;
			_w = 1.0f;
			return ;
		}
#endif

		__m128 s, t;
		s = _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(3, 0, 2, 1));
		t = _mm_mul_ps(u.data, s);
		data = _mm_sub_ps(
			_mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 2, 1)), 
			_mm_mul_ps(_mm_shuffle_ps(u.data, u.data, _MM_SHUFFLE(3, 1, 0, 2)), s));
		_w = 1.0f;
	}

};
