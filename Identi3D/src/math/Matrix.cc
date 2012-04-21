//
// File: Matrix.cc
// ===============
//

#include <src/math/Matrix.h>
#include <src/utils/CPU.h>
#include <src/math/Vector.h>

namespace Identi3D
{

	void Matrix::rotateVector(const Vector3 &vec, float a)
	{
		float fc = cosf(a), fs = sinf(a), delta = 1.0f - fc;
		float fsx = vec._z * fs, fsy = vec._y * fs, fsz = vec._z * fs;
		__m128 delta_array = {delta, delta, delta, delta};

		/*
		 *	x*x*d+cos(a)	x*y*d-z*sin(a)	x*z*d+y*sin(a)	0
		 *	y*x*d+z*sin(a)	y*y*d+cos(a)	y*z*d-x*sin(a)	0
		 *	z*x*d-y*sin(a)	z*y*d+x*sin(a)	z*z*d+cos(a)	0
		 *	0				0				0				1
		 */

#if !defined(_SSE_ONLY)
		if(!CpuInfo::instance().isSSESupported()) {
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

	void Matrix::inverse(const Matrix &m)
	{

		/*
		 * Calculate M^-1 where M * M^-1 = I
		 * by Cramer's Rule: M^-1 = cof(M)^T / det(M).
		 */
		
#if !defined(_SSE_ONLY)
		if(!CpuInfo::instance().isSSESupported()) {
			float factor[6];
			Matrix cof;
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
		__m128 det = {0}, tmp1 = {0};
		__m128 minor0 = {0}, minor1 = {0}, minor2 = {0}, minor3 = {0};
		__m128 row0 = {0}, row1 = {0}, row2 = {0}, row3 = {0};
		const float *src = reinterpret_cast<const float *>(m._data);

		// Matrix transposition.
		tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
		row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));
		row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
		row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
		tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
		row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));
		row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
		row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);

		// Cofactor calculation.
		tmp1 = _mm_mul_ps(row2, row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0 = _mm_mul_ps(row1, tmp1);
		minor1 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
		minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
		minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
		// -----------------------------------------------
		tmp1 = _mm_mul_ps(row1, row2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
		minor3 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
		minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
		minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
		// -----------------------------------------------
		tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		row2 = _mm_shuffle_ps(row2, row2, 0x4E);
		minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
		minor2 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
		minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
		minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
		// -----------------------------------------------
		tmp1 = _mm_mul_ps(row0, row1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
		// -----------------------------------------------
		tmp1 = _mm_mul_ps(row0, row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
		minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
		minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
		// -----------------------------------------------
		tmp1 = _mm_mul_ps(row0, row2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
		minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
		
		// Determinant evaluation.
		det = _mm_mul_ps(row0, minor0);
		det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
		det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
		tmp1 = _mm_rcp_ss(det);
		det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
		det = _mm_shuffle_ps(det, det, 0x00);

		_data[0] = _mm_mul_ps(det, minor0);
		_data[1] = _mm_mul_ps(det, minor1);
		_data[2] = _mm_mul_ps(det, minor2);
		_data[3] = _mm_mul_ps(det, minor3);
	}

	//std::ostream &operator <<(std::ostream &out, const Matrix &m)
	//{
	//	out.setf(std::ios::fixed);
	//	out.precision(4);
	//	for(int i = 0; i < 4; i++) {
	//		for(int j = 0; j < 4; j++)
	//			out << m._data[i].m128_f32[j] << "\t";
	//		out << std::endl;
	//	}
	//	return out;
	//}

};
