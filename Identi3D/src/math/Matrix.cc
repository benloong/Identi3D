//
// File: Matrix.cc
// ===============
//

#include <src/math/Angle.h>
#include <src/math/Matrix.h>
#include <src/utils/CPU.h>

namespace Identi3D
{

	Matrix::Matrix(void)
	{
		_data[0] = _mm_setzero_ps();
		_data[1] = _mm_setzero_ps();
		_data[2] = _mm_setzero_ps();
		_data[3] = _mm_setzero_ps();
	}

	Matrix::Matrix(float a1, float a2, float a3, float a4,
				   float b1, float b2, float b3, float b4,
				   float c1, float c2, float c3, float c4,
				   float d1, float d2, float d3, float d4)
	{
		_data[0] = _mm_set_ps(a1, a2, a3, a4);
		_data[1] = _mm_set_ps(b1, b2, b3, b4);
		_data[2] = _mm_set_ps(c1, c2, c3, c4);
		_data[3] = _mm_set_ps(d1, d2, d3, d4);
	}

	void Matrix::set(float a1, float a2, float a3, float a4,
					 float b1, float b2, float b3, float b4,
					 float c1, float c2, float c3, float c4,
					 float d1, float d2, float d3, float d4)
	{
		_data[0] = _mm_set_ps(a1, a2, a3, a4);
		_data[1] = _mm_set_ps(b1, b2, b3, b4);
		_data[2] = _mm_set_ps(c1, c2, c3, c4);
		_data[3] = _mm_set_ps(d1, d2, d3, d4);
	}

	void Matrix::setRow(int n, float n1, float n2, float n3, float n4)
	{
		if(n >= 0 && n < 4) {
			_data[n] = _mm_set_ps(n1, n2, n3, n4);
		}
	}

	void Matrix::setColumn(int n, float an, float bn, float cn, float dn)
	{
		if(n >= 0 && n < 4) {
			_data[0].m128_f32[n] = an;
			_data[1].m128_f32[n] = bn;
			_data[2].m128_f32[n] = cn;
			_data[3].m128_f32[n] = dn;
		}
	}

	void Matrix::identify(void)
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

	void Matrix::rotateX(const Angle &a)
	{
		float fc = cosf(a._rad), fs = sinf(a._rad);

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

	void Matrix::rotateY(const Angle &a)
	{
		float fc = cosf(a._rad), fs = sinf(a._rad);

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

	void Matrix::rotateZ(const Angle &a)
	{
		float fc = cosf(a._rad), fs = sinf(a._rad);

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

	void Matrix::rotateVector(const Vector3 &vec, const Angle &a)
	{
		float fc = cosf(a._rad), fs = sinf(a._rad), delta = 1.0f - fc;
		float fsx = vec._z * fs, fsy = vec._y * fs, fsz = vec._z * fs;
		__m128 delta_array = {delta, delta, delta, delta};

		/*
		 *	x*x*d+cos(a)	x*y*d-z*sin(a)	x*z*d+y*sin(a)	0
		 *	y*x*d+z*sin(a)	y*y*d+cos(a)	y*z*d-x*sin(a)	0
		 *	z*x*d-y*sin(a)	z*y*d+x*sin(a)	z*z*d+cos(a)	0
		 *	0				0				0				1
		 */

#if !defined(_SSE_ONLY)
		if(!CpuInfo::getInstance().isSSESupported()) {
			_a1 = (vec._x * vec._x) * delta + fc;
			_a2 = (vec._x * vec._y) * delta - fsz;
			_a3 = (vec._x * vec._z) * delta + fsy;
			_b1 = (vec._y * vec._x) * delta + fsz;
			_b2 = (vec._y * vec._y) * delta + fc;
			_b3 = (vec._y * vec._z) * delta - fsx;
			_c1 = (vec._z * vec._x) * delta - fsy;
			_c2 = (vec._z * vec._y) * delta + fsx;
			_c3 = (vec._z * vec._z) * delta + fc;

			_d4 = 1.0f;
			_a4 = _b4 = _c4 = _d1 = _d2 = _d3 = 0.0f;
			return ;
		}
#endif // !defined(_SSE_ONLY)

		_data[0] = _mm_mul_ps(
			_mm_mul_ps(vec._data, _mm_shuffle_ps(vec._data, vec._data, _MM_SHUFFLE(0, 0, 0, 0))),
			delta_array);
		_data[1] = _mm_mul_ps(
			_mm_mul_ps(vec._data, _mm_shuffle_ps(vec._data, vec._data, _MM_SHUFFLE(1, 1, 1, 1))),
			delta_array);
		_data[2] = _mm_mul_ps(
			_mm_mul_ps(vec._data, _mm_shuffle_ps(vec._data, vec._data, _MM_SHUFFLE(2, 2, 2, 2))),
			delta_array);

		_a1 += fc, _a2 -= fsz, _a3 += fsy;
		_b1 += fsz, _b2 += fc, _b3 -= fsx;
		_c1 -= fsy, _c2 += fsx, _c3 += fc;
		_d4 = 1.0f;
		_a4 = _b4 = _c4 = _d1 = _d2 = _d3 = 0.0f;
	}

	void Matrix::translate(float dx, float dy, float dz)
	{
		/*
		 *	0	0	0	dx
		 *	0	0	0	dy
		 *	0	0	0	dz
		 *	0	0	0	0
		 */
		_d1 = dx, _d2 = dy, _d3 = dz;
	}

	void Matrix::transpose(const Matrix &m)
	{
		/*
		 * Reflect the matrix over its main diagonal.
		 */

#if !defined(_SSE_ONLY)
		if(!CpuInfo::getInstance().isSSESupported()) {
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 4; j++)
					_data[i].m128_f32[j] = m._data[j].m128_f32[i];
			return ;
		}
#endif // !defined(_SSE_ONLY)

		memcpy(_data, m._data, sizeof(_data));
		_MM_TRANSPOSE4_PS(_data[0], _data[1], _data[2], _data[3]); // Use SSE macro to get result directly.
	}

	/*
	 * Assistant function: Swap a & b.
	 */
	template <typename T>
	void swap(T &a, T &b)
	{
		register T s = a;
		a = b;
		b = s;
	}

	void Matrix::inverse(const Matrix &m)
	{
		Matrix cof;

		/*
		 * Calculate M^-1 where M * M^-1 = I
		 * by Cramer's Rule: M^-1 = cof(M)^T / det(M).
		 */
		
#if !defined(_SSE_ONLY)
		if(!CpuInfo::getInstance().isSSESupported()) {
			float factor[6];
			cof.transpose(m);

			factor[0] = cof._c3 * cof._d4 - cof._c4 * cof._d3;
			factor[1] = cof._c2 * cof._d4 - cof._c4 * cof._d2;
			factor[2] = cof._c2 * cof._d3 - cof._c3 * cof._d2;
			factor[3] = cof._c1 * cof._d4 - cof._c4 * cof._d1;
			factor[4] = cof._c3 * cof._d1 - cof._c1 * cof._d3;
			factor[5] = cof._c1 * cof._d2 - cof._c2 * cof._d1;
		
			_a1	 = cof._b2 * factor[0] - cof._b3 * factor[1] + cof._b4 * factor[2];
			_a2  = -cof._b1 * factor[0] + cof._b3 * factor[3] + cof._b4 * factor[4];
			_a3  = cof._b1 * factor[1] - cof._b2 * factor[3] + cof._b4 * factor[5];
			_a4  = -cof._b1 * factor[2] - cof._b2 * factor[4] - cof._b3 * factor[5];
		
			_b1  = -cof._a2 * factor[0] + cof._a3 * factor[1] - cof._a4 * factor[2];
			_b2  = cof._a1 * factor[0] - cof._a3 * factor[3] - cof._a4 * factor[4];
			_b3  = -cof._a1 * factor[1] + cof._a2 * factor[3] - cof._a4 * factor[5];
			_b4  = cof._a1 * factor[2] + cof._a2 * factor[4] + cof._a3 * factor[5];
		
			factor[0] = cof._a3 * cof._b4 - cof._a4 * cof._b3;
			factor[1] = cof._a2 * cof._b4 - cof._a4 * cof._b2;
			factor[2] = cof._a2 * cof._b3 - cof._a3 * cof._b2;
			factor[3] = cof._a1 * cof._b4 - cof._a4 * cof._b1;
			factor[4] = cof._a3 * cof._b1 - cof._a1 * cof._b3;
			factor[5] = cof._a1 * cof._b2 - cof._a2 * cof._b1;
		
			_c1	 = factor[0] * cof._d2 - factor[1] * cof._d3 + factor[2] * cof._d4;
			_c2  = -factor[0] * cof._d1 + factor[3] * cof._d3 + factor[4] * cof._d4;
			_c3  = factor[1] * cof._d1 - factor[3] * cof._d2 + factor[5] * cof._d4;
			_c4  = -factor[2] * cof._d1 - factor[4] * cof._d2 - factor[5] * cof._d3;
		
			_d1  = -factor[0] * cof._c2 + factor[1] * cof._c3 - factor[2] * cof._c4;
			_d2  = factor[0] * cof._c1 - factor[3] * cof._c3 - factor[4] * cof._c4;
			_d3  = -factor[1] * cof._c1 + factor[3] * cof._c2 - factor[5] * cof._c4;
			_d4  = factor[2] * cof._c1 + factor[4] * cof._c2 + factor[5] * cof._c3;

			float inv_det = 1.0f / (_a1 * cof._a1 + _a2 * cof._a2 + _a3 * cof._a3 + _a4 * cof._a4);
			_a1 *= inv_det, _a2 *= inv_det, _a3 *= inv_det, _a4 *= inv_det;
			_b1 *= inv_det, _b2 *= inv_det, _b3 *= inv_det, _b4 *= inv_det;
			_c1 *= inv_det, _c2 *= inv_det, _c3 *= inv_det, _c4 *= inv_det;
			_d1 *= inv_det, _d2 *= inv_det, _d3 *= inv_det, _d4 *= inv_det;

			return ;
		}
#endif // !defined(_SSE_ONLY)

		// SSE Accelerated code from Intel documentation.
		__m128 det, tmp;
		float *p = (float*)&m;

		tmp = _mm_setzero_ps();

		tmp = _mm_loadh_pi(_mm_loadl_pi(tmp, (__m64*)p), (__m64*)(p + 4));
		cof._data[1] = _mm_loadh_pi(_mm_loadl_pi(cof._data[1], (__m64*)(p+8)), (__m64*)(p+12));

		cof._data[0] = _mm_shuffle_ps(tmp, cof._data[1], 0x88);
		cof._data[1] = _mm_shuffle_ps(cof._data[1], tmp, 0xDD);

		tmp = _mm_loadh_pi(_mm_loadl_pi(tmp, (__m64*)(p+2)), (__m64*)(p+6));
		cof._data[3] = _mm_loadh_pi(_mm_loadl_pi(tmp, (__m64*)(p+10)), (__m64*)(p+14));

		cof._data[2] = _mm_shuffle_ps(tmp, cof._data[3], 0x88);
		cof._data[3] = _mm_shuffle_ps(cof._data[3], tmp, 0xDD);

		tmp = _mm_mul_ps(cof._data[2], cof._data[3]);
		tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

		_data[0] = _mm_mul_ps(cof._data[1], tmp);
		_data[1] = _mm_mul_ps(cof._data[0], tmp);
		
		tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

		_data[0] = _mm_sub_ps(_mm_mul_ps(cof._data[1], tmp), _data[0]);
		_data[1] = _mm_sub_ps(_mm_mul_ps(cof._data[0], tmp), _data[1]);
		_data[1] = _mm_shuffle_ps(_data[1], _data[1], 0x4E);

		tmp = _mm_mul_ps(cof._data[1], cof._data[2]);
		tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

		_data[0] = _mm_add_ps(_mm_mul_ps(cof._data[3], tmp), _data[0]);
		_data[3] = _mm_mul_ps(cof._data[0], tmp);

		tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

		_data[0] = _mm_sub_ps(_data[0], _mm_mul_ps(cof._data[3], tmp));
		_data[3] = _mm_sub_ps(_mm_mul_ps(cof._data[0], tmp), _data[3]);
		_data[3] = _mm_shuffle_ps(_data[3], _data[3], 0x4E);

		tmp = _mm_mul_ps(_mm_shuffle_ps(cof._data[1], cof._data[1], 0x4E), cof._data[3]);
		tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
		cof._data[2] = _mm_shuffle_ps(cof._data[2], cof._data[2], 0x4E);

		_data[0] = _mm_add_ps(_mm_mul_ps(cof._data[2], tmp), _data[0]);
		_data[2] = _mm_mul_ps(cof._data[0], tmp);

		tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

		_data[0] = _mm_sub_ps(_data[0], _mm_mul_ps(cof._data[2], tmp));
		_data[2] = _mm_sub_ps(_mm_mul_ps(cof._data[0], tmp), _data[2]);
		_data[2] = _mm_shuffle_ps(_data[2], _data[2], 0x4E);

		tmp = _mm_mul_ps(cof._data[0], cof._data[1]);
		tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

		_data[2] = _mm_add_ps(_mm_mul_ps(cof._data[3], tmp), _data[2]);
		_data[3] = _mm_sub_ps(_mm_mul_ps(cof._data[2], tmp), _data[3]);

		tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

		_data[2] = _mm_sub_ps(_mm_mul_ps(cof._data[3], tmp), _data[2]);
		_data[3] = _mm_sub_ps(_data[3], _mm_mul_ps(cof._data[2], tmp));

		tmp = _mm_mul_ps(cof._data[0], cof._data[3]);
		tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

		_data[1] = _mm_sub_ps(_data[1], _mm_mul_ps(cof._data[2], tmp));
		_data[2] = _mm_add_ps(_mm_mul_ps(cof._data[1], tmp), _data[2]);

		tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

		_data[1] = _mm_add_ps(_mm_mul_ps(cof._data[2], tmp), _data[1]);
		_data[2] = _mm_sub_ps(_data[2], _mm_mul_ps(cof._data[1], tmp));

		tmp = _mm_mul_ps(cof._data[0], cof._data[2]);
		tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

		_data[1] = _mm_add_ps(_mm_mul_ps(cof._data[3], tmp), _data[1]);
		_data[3] = _mm_sub_ps(_data[3], _mm_mul_ps(cof._data[1], tmp));

		tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

		_data[1] = _mm_sub_ps(_data[1], _mm_mul_ps(cof._data[3], tmp));
		_data[3] = _mm_add_ps(_mm_mul_ps(cof._data[1], tmp), _data[3]);

		det = _mm_mul_ps(cof._data[0], _data[0]);
		det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
		det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
		tmp = _mm_rcp_ss(det);

		det = _mm_sub_ss(_mm_add_ss(tmp, tmp), _mm_mul_ss(det, _mm_mul_ss(tmp, tmp)));
		det = _mm_shuffle_ps(det, det , 0x00);

		_data[0] = _mm_mul_ps(det, _data[0]);
		_data[1] = _mm_mul_ps(det, _data[1]);
		_data[2] = _mm_mul_ps(det, _data[2]);
		_data[3] = _mm_mul_ps(det, _data[3]);
	}

	Matrix &Matrix::operator =(const Matrix &m)
	{
		if(this != &m) memcpy(_data, m._data, sizeof(_data));
		return (*this);
	}

	const Matrix Matrix::operator *(const Matrix &m) const
	{
		Matrix result;

#ifndef _SSE_ONLY
		if(!CpuInfo::getInstance().isSSESupported()) {
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

	const Vector3 Matrix::operator *(const Vector3 &v) const
	{
		/*
		 * Call vector operator to do this.
		 */
		return v.operator*(*this);
	}

	std::ostream &operator <<(std::ostream &out, const Matrix &m)
	{
		out.setf(std::ios::fixed);
		out.precision(4);
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++)
				out << m._data[i].m128_f32[j] << "\t";
			out << std::endl;
		}
		return out;
	}

};
