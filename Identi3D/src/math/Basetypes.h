//
// File: Basetype.h
// ================
// Basetypes and assistant function of floating point.
//

#ifndef IDENTI3D_SRC_MATH_BASETYPES_H
#define IDENTI3D_SRC_MATH_BASETYPES_H

namespace Identi3D
{

	static const float IDF_EPSILON = 0.001f;

	inline bool _fzero(float _f)
	{
		if(_f < IDF_EPSILON && _f > -IDF_EPSILON) return true;
		return false;
	}

	inline bool _fequal(float _a, float _b)
	{
		if(_fzero(_a - _b)) return true;
		return false;
	}

}

#endif // IDENTI3D_SRC_MATH_BASETYPES_H