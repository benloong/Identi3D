//
// File: Matrix.h
// ==============
// Provides matrix computation abilities.
//

#ifndef IDENTI3D_SRC_MATH_MATRIX_H
#define IDENTI3D_SRC_MATH_MATRIX_H

#include <src/identi3d/General.h>
#include <src/utils/CPU.h>

namespace Identi3D
{
	
	class __declspec(dllexport) Matrix
	{
		friend class Vector3;
		friend class Ray;

	public:

		/*
		 * Constructor: Set all value to zero.
		 */
		explicit Matrix(void)
		{
			memset(_data, 0, sizeof(_data));
		}

		/*
		 * Copy constructor.
		 */
		Matrix(const Matrix &m)
		{
			this->operator=(m);
		}

		/*
		 * Constructor: Set all value to input.
		 */
		explicit Matrix(float a1, float a2, float a3, float a4,
						float b1, float b2, float b3, float b4,
						float c1, float c2, float c3, float c4,
						float d1, float d2, float d3, float d4)
		{
			_data[0] = _mm_set_ps(a1, a2, a3, a4);
			_data[1] = _mm_set_ps(b1, b2, b3, b4);
			_data[2] = _mm_set_ps(c1, c2, c3, c4);
			_data[3] = _mm_set_ps(d1, d2, d3, d4);
		}

		/*
		 * Destructor: Do nothing.
		 */
		~Matrix(void) {} ;

		/*
		 * Set values of Matrix.
		 */
		inline void set(float a1, float a2, float a3, float a4,
				 float b1, float b2, float b3, float b4,
				 float c1, float c2, float c3, float c4,
				 float d1, float d2, float d3, float d4)
		{
			_data[0] = _mm_set_ps(a1, a2, a3, a4);
			_data[1] = _mm_set_ps(b1, b2, b3, b4);
			_data[2] = _mm_set_ps(c1, c2, c3, c4);
			_data[3] = _mm_set_ps(d1, d2, d3, d4);
		}
		
		/*
		 * Set specified row.
		 */
		inline void setRow(int n, float n1, float n2, float n3, float n4)
		{
			if(n >= 0 && n < 4) {
				_data[n] = _mm_set_ps(n1, n2, n3, n4);
			}
		}

		/*
		 * Set specified column.
		 */
		inline void setColumn(int n, float an, float bn, float cn, float dn)
		{
			if(n >= 0 && n < 4) {
				_data[0].m128_f32[n] = an;
				_data[1].m128_f32[n] = bn;
				_data[2].m128_f32[n] = cn;
				_data[3].m128_f32[n] = dn;
			}
		}

		/*
		 * Set matrix to 0.
		 */
		inline void clear(void)
		{
			memset(_data, 0, sizeof(_data));
		}

		/*
		 * Build a identify matrix.
		 */
		inline void identify(void)
		{
			
			/*
			 *	1		0		0		0
			 *	0		1		0		0
			 *	0		0		1		0
			 *	0		0		0		1
			 */

			_a1 = _b2 = _c3 = _d4 = 1.0f;
			_a2 = _a3 = _a4 = 
				_b1 = _b3 = _b4 = 
				_c1 = _c2 = _c4 = 
				_d1 = _d2 = _d3 = 0.0f;
		}

		/*
		 * Build a rotation matrix relative to x axis.
		 */
		inline void rotateX(float a)
		{
			float fc = cosf(a), fs = sinf(a);

			/*
			 *	1		0		0		0
			 *	0		cos(a)	sin(a)	0
			 *	0		-sin(a)	cos(a)	0
			 *	0		0		0		1
			 */

			_b2 = fc, _b3 = fs;
			_c2 = -fs, _c3 = fc;
			_a1 = _d4 = 1.0f;
			_a2 = _a3 = _a4 = 
				_b1 = _b4 = 
				_c1 = _c4 = 
				_d1 = _d2 = _d3 = 0.0f;
		}
		
		/*
		 * Build a rotation matrix relative to y axis.
		 */
		inline void rotateY(float a)
		{
			float fc = cosf(a), fs = sinf(a);

			/*
			 *	cos(a)	0		-sin(a)	0
			 *	0		1		0		0
			 *	sin(a)	0		cos(a)	0
			 *	0		0		0		1
			 */

			_a1 = fc, _a3 = -fs, _c1 = fs, _c3 = fc; 
			_b2 = _d4 = 1.0f;
			_a2 = _a4 = 
				_b1 = _b3 = _b4 = 
				_c2 = _c4 = _d1 = 
				_d2 = _d3 = 0.0f;
		}
		
		/*
		 * Build a rotation matrix relative to z axis.
		 */
		inline void rotateZ(float a)
		{
			float fc = cosf(a), fs = sinf(a);

			/*
			 *	cos(a)	sin(a)	0		0
			 *	-sin(a)	cos(a)	0		0
			 *	0		0		1		0
			 *	0		0		0		1
			 */

			clear();
			_a1 = fc, _a2 = fs, _b1 = -fs, _b2 = fc; 
			_c3 = _d4 = 1.0f;
			_a3 = _a4 = 
				_b3 = _b4 = 
				_c1 = _c2 = _c4 = 
				_d1 = _d2 = _d3 = 0.0f;
		}

		/*
		 * Build a rotation matrix according to specified vector.
		 * Vector must be normalized first.
		 */
		void rotateVector(const Vector3 &vec, float angle);

		/*
		 * Build a translate matrix.
		 */
		inline void translate(float dx, float dy, float dz)
		{
			/*
			 *	0	0	0	dx
			 *	0	0	0	dy
			 *	0	0	0	dz
			 *	0	0	0	0
			 */
			_d1 = dx, _d2 = dy, _d3 = dz;
		}
		
		/*
		 * Transpose the matrix (Reflect the matrix over its main diagonal).
		 */
		inline void transpose(const Matrix &m)
		{
#if !defined(_SSE_ONLY)
			if(!CpuInfo::instance().isSSESupported()) {
				for(int i = 0; i < 4; i++)
					for(int j = 0; j < 4; j++)
						_data[i].m128_f32[j] = m._data[j].m128_f32[i];
				return ;
			}
#endif // !defined(_SSE_ONLY)

			// Use SSE macro to get result directly.
			memcpy(_data, m._data, sizeof(_data));
			_MM_TRANSPOSE4_PS(_data[0], _data[1], _data[2], _data[3]);
		}

		/*
		 * Inverse the matrix.
		 */
		void inverse(const Matrix &m);
		
		/*
		 * Copy the matrix.
		 */
		inline Matrix &operator=(const Matrix &m)
		{
			if(this != &m) memcpy(_data, m._data, sizeof(_data));
			return (*this);
		}

		/*
		 * A matrix multiplies a matrix (inline it or otherwise performance issues will occur).
		 */
		inline const Matrix operator*(const Matrix &m) const
		{
			Matrix result;

#ifndef _SSE_ONLY
			if(!CpuInfo::instance().isSSESupported()) {
				result._a1 = _a1 * m._a1 + _a2 * m._b1 + _a3 * m._c1 + _a4 * m._d1;
				result._a2 = _a1 * m._a2 + _a2 * m._b2 + _a3 * m._c2 + _a4 * m._d2;
				result._a3 = _a1 * m._a3 + _a2 * m._b3 + _a3 * m._c3 + _a4 * m._d3;
				result._a4 = _a1 * m._a4 + _a2 * m._b4 + _a3 * m._c4 + _a4 * m._d4;
				result._b1 = _b1 * m._a1 + _b2 * m._b1 + _b3 * m._c1 + _b4 * m._d1;
				result._b2 = _b1 * m._a2 + _b2 * m._b2 + _b3 * m._c2 + _b4 * m._d2;
				result._b3 = _b1 * m._a3 + _b2 * m._b3 + _b3 * m._c3 + _b4 * m._d3;
				result._b4 = _b1 * m._a4 + _b2 * m._b4 + _b3 * m._c4 + _b4 * m._d4;
				result._c1 = _c1 * m._a1 + _c2 * m._b1 + _c3 * m._c1 + _c4 * m._d1;
				result._c2 = _c1 * m._a2 + _c2 * m._b2 + _c3 * m._c2 + _c4 * m._d2;
				result._c3 = _c1 * m._a3 + _c2 * m._b3 + _c3 * m._c3 + _c4 * m._d3;
				result._c4 = _c1 * m._a4 + _c2 * m._b4 + _c3 * m._c4 + _c4 * m._d4;
				result._d1 = _d1 * m._a1 + _d2 * m._b1 + _d3 * m._c1 + _d4 * m._d1;
				result._d2 = _d1 * m._a2 + _d2 * m._b2 + _d3 * m._c2 + _d4 * m._d2;
				result._d3 = _d1 * m._a3 + _d2 * m._b3 + _d3 * m._c3 + _d4 * m._d3;
				result._d4 = _d1 * m._a4 + _d2 * m._b4 + _d3 * m._c4 + _d4 * m._d4;
			}
#endif // !defined(_SSE_ONLY)

			result._data[0] = _mm_add_ps(_mm_add_ps(_mm_add_ps(
				_mm_mul_ps(_mm_shuffle_ps(_data[0], _data[0], _MM_SHUFFLE(0, 0, 0, 0)), m._data[0]),
				_mm_mul_ps(_mm_shuffle_ps(_data[0], _data[0], _MM_SHUFFLE(1, 1, 1, 1)), m._data[1])),
				_mm_mul_ps(_mm_shuffle_ps(_data[0], _data[0], _MM_SHUFFLE(2, 2, 2, 2)), m._data[2])),
				_mm_mul_ps(_mm_shuffle_ps(_data[0], _data[0], _MM_SHUFFLE(3, 3, 3, 3)), m._data[3]));
			result._data[1] = _mm_add_ps(_mm_add_ps(_mm_add_ps(
				_mm_mul_ps(_mm_shuffle_ps(_data[1], _data[1], _MM_SHUFFLE(0, 0, 0, 0)), m._data[0]),
				_mm_mul_ps(_mm_shuffle_ps(_data[1], _data[1], _MM_SHUFFLE(1, 1, 1, 1)), m._data[1])),
				_mm_mul_ps(_mm_shuffle_ps(_data[1], _data[1], _MM_SHUFFLE(2, 2, 2, 2)), m._data[2])),
				_mm_mul_ps(_mm_shuffle_ps(_data[1], _data[1], _MM_SHUFFLE(3, 3, 3, 3)), m._data[3]));
			result._data[2] = _mm_add_ps(_mm_add_ps(_mm_add_ps(
				_mm_mul_ps(_mm_shuffle_ps(_data[2], _data[2], _MM_SHUFFLE(0, 0, 0, 0)), m._data[0]),
				_mm_mul_ps(_mm_shuffle_ps(_data[2], _data[2], _MM_SHUFFLE(1, 1, 1, 1)), m._data[1])),
				_mm_mul_ps(_mm_shuffle_ps(_data[2], _data[2], _MM_SHUFFLE(2, 2, 2, 2)), m._data[2])),
				_mm_mul_ps(_mm_shuffle_ps(_data[2], _data[2], _MM_SHUFFLE(3, 3, 3, 3)), m._data[3]));
			result._data[3] = _mm_add_ps(_mm_add_ps(_mm_add_ps(
				_mm_mul_ps(_mm_shuffle_ps(_data[3], _data[3], _MM_SHUFFLE(0, 0, 0, 0)), m._data[0]),
				_mm_mul_ps(_mm_shuffle_ps(_data[3], _data[3], _MM_SHUFFLE(1, 1, 1, 1)), m._data[1])),
				_mm_mul_ps(_mm_shuffle_ps(_data[3], _data[3], _MM_SHUFFLE(2, 2, 2, 2)), m._data[2])),
				_mm_mul_ps(_mm_shuffle_ps(_data[3], _data[3], _MM_SHUFFLE(3, 3, 3, 3)), m._data[3]));
			return result;
		}

		/*
		 * A matrix multiplies a matrix.
		 */
		Matrix &operator*=(const Matrix &m)
		{
			this->operator=(this->operator*(m));
			return (*this);
		}

		/*
		 * Write the matrix to stream.
		 */
		inline friend std::ostream __declspec(dllexport) &operator <<(std::ostream &out, const Matrix &m)
		{
			out.setf(std::ios::fixed);
			out.precision(3);
			for(int i = 0; i < 4; i++) {
				for(int j = 0; j < 4; j++)
					out << m._data[i].m128_f32[j] << "\t";
				out << std::endl;
			}
			return out;
		}

		/*
		 * Get element directly.
		 * Calls with i >= 4 or j >= 4 is equivalent to (0, 0).
		 */
		inline float &operator ()(unsigned int i, unsigned int j)
		{
			return (i < 4 && j < 4) ? _data[i].m128_f32[j] : _data[0].m128_f32[0];
		}
		
		/*
		 * Get element directly (const version).
		 * Calls with i >= 4 or j >= 4 is equivalent to (0, 0).
		 */
		inline const float &operator ()(unsigned int i, unsigned int j) const
		{
			return (i < 4 && j < 4) ? _data[i].m128_f32[j] : _data[0].m128_f32[0];
		}

	public:
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
