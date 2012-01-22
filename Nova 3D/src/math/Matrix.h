//
// File: Matrix.h
// ==============
// Provides matrix computation abilities.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_SRC_MATH_MATRIX_H
#define NOVA3D_SRC_MATH_MATRIX_H

#include "src/nova-3d/NovaGeneral.h"
#include <cmath>
#include <xmmintrin.h>

namespace Nova3D
{

	class Matrix
	{
	public:
		__declspec(align(16)) union
		{
			__m128 data[4];
			struct
			{
				float _a1, _a2, _a3, _a4;
				float _b1, _b2, _b3, _b4;
				float _c1, _c2, _c3, _c4;
				float _d1, _d2, _d3, _d4;
			};
		};
	};

}

#endif
