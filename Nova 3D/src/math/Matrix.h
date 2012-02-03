//
// File: Matrix.h
// ==============
// Provides matrix computation abilities.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_SRC_MATH_MATRIX_H
#define NOVA3D_SRC_MATH_MATRIX_H

#include <src/nova-3d/NovaGeneral.h>
#include <src/math/Vector.h>

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

		void clear(void) 
			{ memset(data, 0, sizeof(data)); }
		void identify(void) 
			{ clear(); _a1 = _b2 = _c3 = _d4 = 1.0f; }

		void rotateX(const Angle &a); 
		void rotateY(const Angle &a);
		void rotateZ(const Angle &a);
		void rotateVector(Vector3 &vec, const Angle &a);
		void translate(float dx, float dy, float dz) 
			{ _d1 = dx, _d2 = dy, _d3 = dz; }
		void transpose(const Matrix &m);
		void inverse(const Matrix &m);
		
		Matrix &operator =(const Matrix &m);
		float &operator ()(unsigned int i, unsigned int j) 
			{ return data[i].m128_f32[j]; }
		const Matrix operator *(const Matrix &m) const;
		const Vector3 operator *(const Vector3 &v) const;
	};

}

#endif
