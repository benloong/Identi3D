//
// File: Vector.cc
// ===============
//

#include "src/math/Vector.h"
#include "src/math/Matrix.h"
#include "src/utils/CPU.h"

#ifndef _FP_TOLERANCE
#define _FP_TOLERANCE 1e-6
#endif

#ifndef _fcomp
#define _fcomp(a, b) ((((a) - (b)) < _FP_TOLERANCE) ? true : false)
#endif

#ifndef _fzero
#define _fzero(f) (_fcomp(f, 0))
#endif

namespace Nova3D
{
	
	Vector3::Vector3(void)
	{
		set(0.0f, 0.0f, 0.0f);
	}

	Vector3::Vector3(float x, float y, float z)
	{
		set(x, y, z);
	}

	Vector3::~Vector3(void)
	{
	}

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

		float *_ptr = (float *)&vec;
		__asm {
			mov ecx, this;
			mov eax, _ptr;
			movss xmm0, [ecx];
			mov edx, m;
			movss xmm1, [ecx + 4];
			shufps xmm0, xmm0, 0;
			mulps xmm0, [edx];
			movss xmm2, [ecx + 8];
			shufps xmm1, xmm1, 0;
			mulps xmm1, [edx + 16];
			addps xmm0, xmm1;
			movss xmm3, [ecx + 12];
			shufps xmm2, xmm2, 0;
			mulps xmm2, [edx + 32];
			addps xmm0, xmm2;
			shufps xmm3, xmm3, 0;
			mulps xmm3, [edx + 48];
			addps xmm0, xmm3;
		
			movaps xmm1, xmm0;
			shufps xmm1, xmm1, 0xFF;
			divps xmm0, xmm1;

			movaps [eax], xmm0;
		}
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
		if(!CpuInfo::getInstance().isSSESupported()) {
			return sqrt(_x * _x + _y * _y + _z * _z);
		}
		float result, *r;
		r = &result;
		_w = 0.0f;
		__asm {
			mov ecx, r;
			mov edx, this;			
			movaps xmm0, [edx];
			mulps xmm0, xmm0;
			movaps xmm1, xmm0;
			shufps xmm1, xmm1, 0B1h;
			addps xmm0, xmm1;
			movaps xmm1, xmm0; 
			shufps xmm1, xmm1, 0Fh;
			addps xmm0, xmm1;
			sqrtss xmm0, xmm0;
			movss [ecx], xmm0;
		}
		_w = 1.0f;
		return result;
	}

	void Vector3::normalize(void)
	{
		if(!CpuInfo::getInstance().isSSESupported()) {
			float f = sqrt(_x * _x + _y * _y + _z * _z);
			if(!_fzero(f)) _x /= f, _y /= f, _z /= f;
			return ;
		}
		_w = 0.0f;
		__asm {
			mov esi, this;
			movaps xmm0, [esi];
			movaps xmm2, xmm0;
			mulps xmm0, xmm0;
			movaps xmm1, xmm0;
			shufps xmm1, xmm1, 0B1h;
			addps xmm0, xmm1;
			movaps xmm1, xmm0; 
			shufps xmm1, xmm1, 0Fh;
			addps xmm0, xmm1;
			rsqrtps xmm0, xmm0;
			movaps [esi], xmm0;
			movaps [esi], xmm2;
			mulps xmm2, xmm0;
			movaps [esi], xmm2;
		}
		_w = 1.0;
	}

	void Vector3::cross(const Vector3 &u, const Vector3 &v)
	{
		if(!CpuInfo::getInstance().isSSESupported()) {
			_x = u._y * v._z - u._z * v._y;
			_y = u._z * v._x - u._x * v._z;
			_z = u._x * v._y - u._y * v._x;
			_w = 1.0f;
			return ;
		}
		__asm {
			mov ecx, u;
			mov edx, v;
			mov esi, this;
			movaps xmm0, [ecx];
			movaps xmm1, [edx];
			movaps xmm2, xmm0;
			shufps xmm1, xmm1, 0C9h;
			mulps xmm0, xmm1;
			shufps xmm2, xmm2, 0D2h;
			mulps xmm2, xmm1;
			shufps xmm0, xmm0, 0C9h;
			subps xmm0, xmm2;
			movaps [esi], xmm0;
		}
		_w = 1.0f;
	}

};
