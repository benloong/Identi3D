//
// File: Matrix.cc
// ===============
//

#include <src/math/Angle.h>
#include <src/math/Matrix.h>
#include <src/utils/CPU.h>

namespace Nova3D
{

	void Matrix::rotateX(const Angle &a)
	{
		float fc = cosf(a._rad), fs = sinf(a._rad);
		clear();
		_b2 = fc, _b3 = fs, _c2 = -fs, _c3 = fc, _a1 = _d4 = 1.0f;
	}

	void Matrix::rotateY(const Angle &a)
	{
		float fc = cosf(a._rad), fs = sinf(a._rad);
		clear();
		_a1 = fc, _a3 = -fs, _c1 = fs, _c3 = fc; _b2 = _d4 = 1.0f;
	}

	void Matrix::rotateZ(const Angle &a)
	{
		float fc = cosf(a._rad), fs = sinf(a._rad);
		clear();
		_a1 = fc, _a2 = fs, _b1 = -fs, _b2 = fc; _c3 = _d4 = 1.0f;
	}

	void Matrix::rotateVector(Vector3 &vec, const Angle &a)
	{
		float fc = cosf(a._rad), fs = sinf(a._rad), delta = 1.0f - fc;
		vec.normalize();
		clear();
		_a1 = (vec._x * vec._x) * delta + fc;
		_a2 = (vec._x * vec._y) * delta - (vec._z * fs);
		_a3 = (vec._x * vec._z) * delta + (vec._y * fs);
		_b1 = (vec._y * vec._x) * delta + (vec._z * fs);
		_b2 = (vec._y * vec._y) * delta + fc;
		_b3 = (vec._y * vec._z) * delta - (vec._x * fs);
		_c1 = (vec._z * vec._x) * delta - (vec._y * fs);
		_c2 = (vec._z * vec._y) * delta + (vec._x * fs);
		_c3 = (vec._z * vec._z) * delta + fc;
		_d4 = 1.0f;
	}

	void Matrix::transpose(const Matrix &m)
	{

#ifndef _SSE_ONLY
		if(!CpuInfo::getInstance().isSSESupported()) {
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 4; j++)
					data[i].m128_f32[j] = m.data[j].m128_f32[i];
			return ;
		}
#endif
		
		memcpy(data, m.data, sizeof(data));
		_MM_TRANSPOSE4_PS(data[0], data[1], data[2], data[3]);
	}

	template <typename T>
	void swap(T &a, T &b)
	{
		register T s = a;
		a = b;
		b = s;
	}

	void Matrix::inverse(const Matrix &m)
	{
		short int dx[4], dy[4], i, j, k;

		this->operator=(m);
		for(k = 0; k < 4; k++) {
			float fmax = 0.0f;
			for(i = k; i < 4; i++) {
				for(j = k; j < 4; j++) {
					float f = abs(data[i].m128_f32[j]);
					if(f > fmax) {
						fmax = f;
						dx[k] = i;
						dy[k] = j;
					}
				}
			}
			if(_fzero(fmax)) return ;
			if(dx[k] != k) {
				swap<float>(data[k].m128_f32[0], data[dx[k]].m128_f32[0]);
				swap<float>(data[k].m128_f32[1], data[dx[k]].m128_f32[1]);
				swap<float>(data[k].m128_f32[2], data[dx[k]].m128_f32[2]);
				swap<float>(data[k].m128_f32[3], data[dx[k]].m128_f32[3]);
			}
			if(dy[k] != k) {
				swap<float>(data[0].m128_f32[k], data[0].m128_f32[dy[k]]);
				swap<float>(data[1].m128_f32[k], data[1].m128_f32[dy[k]]);
				swap<float>(data[2].m128_f32[k], data[2].m128_f32[dy[k]]);
				swap<float>(data[3].m128_f32[k], data[3].m128_f32[dy[k]]);
			}

			data[k].m128_f32[k] = 1.0f / data[k].m128_f32[k];
			for(j = 0; j < 4; j++) {
				if(j != k) data[k].m128_f32[j] *= data[k].m128_f32[k];
			}

			for(i = 0; i < 4; i++) {
				if(i != k) {
					for(j = 0; j < 4; j++) {
						if(j != k) data[i].m128_f32[j] -= data[i].m128_f32[k] * data[k].m128_f32[j];
					}
				}
			}

			for(i = 0; i < 4; i++) {
				if(i != k) data[i].m128_f32[k] *= -data[k].m128_f32[k];
			}
		}

		for(k = 3; k >= 0; k--){
			if(dy[k] != k) {
				swap<float>(data[k].m128_f32[0], data[dy[k]].m128_f32[0]);
				swap<float>(data[k].m128_f32[1], data[dy[k]].m128_f32[1]);
				swap<float>(data[k].m128_f32[2], data[dy[k]].m128_f32[2]);
				swap<float>(data[k].m128_f32[3], data[dy[k]].m128_f32[3]);
			}
			if(dx[k] != k) {
				swap<float>(data[0].m128_f32[k], data[0].m128_f32[dx[k]]);
				swap<float>(data[1].m128_f32[k], data[1].m128_f32[dx[k]]);
				swap<float>(data[2].m128_f32[k], data[2].m128_f32[dx[k]]);
				swap<float>(data[3].m128_f32[k], data[3].m128_f32[dx[k]]);
			}
		}
	}

	Matrix &Matrix::operator =(const Matrix &m)
	{
		if(this != &m) for(int i = 0; i < 4; i++) data[i] = m.data[i];
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
#endif

		result.data[0] = _mm_add_ps(_mm_add_ps(_mm_add_ps(
			_mm_mul_ps(_mm_shuffle_ps(data[0], data[0], _MM_SHUFFLE(0, 0, 0, 0)), m.data[0]),
			_mm_mul_ps(_mm_shuffle_ps(data[0], data[0], _MM_SHUFFLE(1, 1, 1, 1)), m.data[1])),
			_mm_mul_ps(_mm_shuffle_ps(data[0], data[0], _MM_SHUFFLE(2, 2, 2, 2)), m.data[2])),
			_mm_mul_ps(_mm_shuffle_ps(data[0], data[0], _MM_SHUFFLE(3, 3, 3, 3)), m.data[3]));
		result.data[1] = _mm_add_ps(_mm_add_ps(_mm_add_ps(
			_mm_mul_ps(_mm_shuffle_ps(data[1], data[1], _MM_SHUFFLE(0, 0, 0, 0)), m.data[0]),
			_mm_mul_ps(_mm_shuffle_ps(data[1], data[1], _MM_SHUFFLE(1, 1, 1, 1)), m.data[1])),
			_mm_mul_ps(_mm_shuffle_ps(data[1], data[1], _MM_SHUFFLE(2, 2, 2, 2)), m.data[2])),
			_mm_mul_ps(_mm_shuffle_ps(data[1], data[1], _MM_SHUFFLE(3, 3, 3, 3)), m.data[3]));
		result.data[2] = _mm_add_ps(_mm_add_ps(_mm_add_ps(
			_mm_mul_ps(_mm_shuffle_ps(data[2], data[2], _MM_SHUFFLE(0, 0, 0, 0)), m.data[0]),
			_mm_mul_ps(_mm_shuffle_ps(data[2], data[2], _MM_SHUFFLE(1, 1, 1, 1)), m.data[1])),
			_mm_mul_ps(_mm_shuffle_ps(data[2], data[2], _MM_SHUFFLE(2, 2, 2, 2)), m.data[2])),
			_mm_mul_ps(_mm_shuffle_ps(data[2], data[2], _MM_SHUFFLE(3, 3, 3, 3)), m.data[3]));
		result.data[3] = _mm_add_ps(_mm_add_ps(_mm_add_ps(
			_mm_mul_ps(_mm_shuffle_ps(data[3], data[3], _MM_SHUFFLE(0, 0, 0, 0)), m.data[0]),
			_mm_mul_ps(_mm_shuffle_ps(data[3], data[3], _MM_SHUFFLE(1, 1, 1, 1)), m.data[1])),
			_mm_mul_ps(_mm_shuffle_ps(data[3], data[3], _MM_SHUFFLE(2, 2, 2, 2)), m.data[2])),
			_mm_mul_ps(_mm_shuffle_ps(data[3], data[3], _MM_SHUFFLE(3, 3, 3, 3)), m.data[3]));
		return result;
	}

	const Vector3 Matrix::operator *(const Vector3 &v) const
	{
		// Consider left/right multiply as the same thing.
		return v * (*this);
	}

};
