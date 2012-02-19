//
// File: Matrix.h
// ==============
// Provides matrix computation abilities.
//

#ifndef NOVA3D_SRC_MATH_MATRIX_H
#define NOVA3D_SRC_MATH_MATRIX_H

#include <src/nova-3d/NovaGeneral.h>
#include <src/math/Vector.h>

namespace Nova3D
{
	
	class Matrix
	{
		friend class Vector3;

	public:
		Matrix(void) :
		  _a1(0.0f), _a2(0.0f), _a3(0.0f), _a4(0.0f),
		  _b1(0.0f), _b2(0.0f), _b3(0.0f), _b4(0.0f),
		  _c1(0.0f), _c2(0.0f), _c3(0.0f), _c4(0.0f),
		  _d1(0.0f), _d2(0.0f), _d3(0.0f), _d4(0.0f) {} ;
		Matrix(float a1, float a2, float a3, float a4,
			   float b1, float b2, float b3, float b4,
			   float c1, float c2, float c3, float c4,
			   float d1, float d2, float d3, float d4) :
		  _a1(a1), _a2(a2), _a3(a3), _a4(a4),
		  _b1(b1), _b2(b2), _b3(b3), _b4(b4),
		  _c1(c1), _c2(c2), _c3(c3), _c4(c4),
		  _d1(d1), _d2(d2), _d3(d3), _d4(d4) {};

		/*
		 * Set values of Matrix.
		 */
		void set(float a1, float a2, float a3, float a4,
				 float b1, float b2, float b3, float b4,
				 float c1, float c2, float c3, float c4,
				 float d1, float d2, float d3, float d4);
		void setRow(int n, float n1, float n2, float n3, float n4);
		void setColumn(int n, float an, float bn, float cn, float dn);

		/*
		 * Set matrix to 0.
		 */
		void clear(void) { memset(_data, 0, sizeof(_data)); }

		/*
		 * Create a identify matrix.
		 */
		void identify(void);

		/*
		 * Create a rotation matrix according to the given axis.
		 */
		void rotateX(const Angle &a); 
		void rotateY(const Angle &a);
		void rotateZ(const Angle &a);

		/*
		 * Create a rotation matrix according to specified vector.
		 * Vector must be normalized first.
		 */
		void rotateVector(Vector3 &vec, const Angle &a);

		void translate(float dx, float dy, float dz) 
			{ _d1 = dx, _d2 = dy, _d3 = dz; }
		void transpose(const Matrix &m);
		void inverse(const Matrix &m);
		
		Matrix &operator =(const Matrix &m);
		float &operator ()(unsigned int i, unsigned int j) 
			{ return _data[i].m128_f32[j]; }
		const Matrix operator *(const Matrix &m) const;
		const Vector3 operator *(const Vector3 &v) const;
		
	private:
		_DATA_ALIGN union
		{
			struct
			{
				float _a1, _a2, _a3, _a4;
				float _b1, _b2, _b3, _b4;
				float _c1, _c2, _c3, _c4;
				float _d1, _d2, _d3, _d4;
			};
			__m128 _data[4];
		};
	};

}

#endif
