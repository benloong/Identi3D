//
// File: Angle.h
// =============
// Accessing to an angle via two types, radian & degree.
//

#ifndef NOVA3D_SRC_MATH_ANGLE_H
#define NOVA3D_SRC_MATH_ANGLE_H

#include <src/nova-3d/NovaGeneral.h>

#ifndef _PI
#define _PI 3.14159265f
#endif

#ifndef _DOUBLE_PI
#define _DOUBLE_PI 6.2831853f
#endif

#ifndef _DEFAULT_EPSILON
#define _DEFAULT_EPSILON 1e-3
#endif

namespace Nova3D
{

	inline bool _fcomp(float a, float b, const float precision = _DEFAULT_EPSILON)
	{
		if(abs(a - b) < precision) return true;
		return false;
	}

	inline bool _fzero(float f)
	{
		return _fcomp(f, 0.0f);
	}

	typedef float Radian;

	class Angle
	{
	public:
		Radian _rad;

		Angle(void) 
			{ _rad = 0.0f; }
		Angle(Radian r) 
			{ _rad = r; validate(); }
		Angle(int degree) 
			{ _rad = static_cast<Radian>(((degree >= 0) ? degree : -degree) % 360) / 360.0f * _DOUBLE_PI; }
		virtual ~Angle(void) {} ;
		
		Radian getRadian(void) const 
			{ return _rad; }
		int roundToNearestDegree(void) const
			{ return static_cast<int>(_rad / _DOUBLE_PI * 360.0f); }
		void validate(void)
			{ while(_rad < 0) _rad += _DOUBLE_PI; while(_rad >= _DOUBLE_PI) _rad -= _DOUBLE_PI; }

		bool operator ==(const Angle &a) const
			{ return (_fcomp(_rad, a._rad)); }
		Angle &operator =(const Angle &a)
			{ _rad = a._rad; return *(this); }
		Angle &operator =(Radian r)
			{ _rad = r; return *(this); }

		const Angle operator +(const Angle &a) const
			{ return Angle(_rad + a._rad); }
		const Angle operator -(const Angle &a) const
			{ return Angle(_rad - a._rad); }
		const Angle operator *(const Angle &a) const
			{ return Angle(_rad * a._rad); }
		const Angle operator /(const Angle &a) const
			{ return Angle(_rad / a._rad); }

		void operator +=(const Angle &a) 
			{ _rad += a._rad; if(_rad > _DOUBLE_PI) _rad -= _DOUBLE_PI; }
		void operator -=(const Angle &a)
			{ if(_rad > a._rad) _rad -= a._rad; else _rad = a._rad - _rad; }
		void operator *=(const Angle &a)
			{ _rad = (_rad * a._rad); validate(); }
		void operator /=(const Angle &a)
			{ _rad = (_rad / a._rad); validate(); }

	};

};

#endif
