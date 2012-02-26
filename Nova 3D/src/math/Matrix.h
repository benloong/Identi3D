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

		/*
		 * Default constructor: Set all values to zero.
		 */
		Matrix(void);

		/*
		 * Set values of matrix.
		 */
		Matrix(float a1, float a2, float a3, float a4,
			   float b1, float b2, float b3, float b4,
			   float c1, float c2, float c3, float c4,
			   float d1, float d2, float d3, float d4);

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
		 * Build a identify matrix.
		 */
		void identify(void);

		/*
		 * Build a rotation matrix according to x, y or z axis.
		 */
		void rotateX(const Angle &a); 
		void rotateY(const Angle &a);
		void rotateZ(const Angle &a);

		/*
		 * Build a rotation matrix according to specified vector.
		 * Vector must be normalized first.
		 */
		void rotateVector(const Vector3 &vec, const Angle &a);

		/*
		 * Build a translate matrix.
		 */
		void translate(float dx, float dy, float dz);
		
		/*
		 * Transpose the matrix.
		 */
		void transpose(const Matrix &m);

		/*
		 * Inverse the matrix.
		 */
		void inverse(const Matrix &m);
		
		/*
		 * Basic matrix operator.
		 */
		Matrix &operator =(const Matrix &m);

		const Matrix operator *(const Matrix &m) const;
		const Vector3 operator *(const Vector3 &v) const;

		friend std::ostream &operator <<(std::ostream &out, const Matrix &m);

		/*
		 * Get element directly.
		 * Calls with i >= 4 or j >= 4 is equivalent to (0, 0).
		 */
		float operator ()(unsigned int i, unsigned int j) const 
		{ return (i < 4 && j < 4) ? _data[i].m128_f32[j] : _data[0].m128_f32[0]; };
		
	private:
		/*
		 * Data should be aligned to 16 bit for fast SSE computation.
		 */
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
