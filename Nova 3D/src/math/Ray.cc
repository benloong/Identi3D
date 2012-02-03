//
// File: Ray.cc
// ============
//

#include <src/math/Ray.h>

#include <src/math/Matrix.h>

namespace Nova3D
{

	void Ray::set(const Vector3 &origin, const Vector3 &direction)
	{
		_origin = origin;
		_direction = direction;
	}

	void Ray::detransform(const Matrix &m)
	{
		Matrix s = m;
		
		_origin._x -= s._d1;
		_origin._y -= s._d2;
		_origin._z -= s._d3;
		
		s.translate(0.0f, 0.0f, 0.0f);
		s.inverse(s);

		_origin *= s;
		_direction *= s;
	}

	bool Ray::intersect(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, bool cull_backside, float *distance, Vector3 *hit) const
	{
		Vector3 edge1 = v1 - v0, edge2 = v2 - v0;
		Vector3 tvec, pvec, qvec;
		float inv_det, det, u, v;

		pvec.cross(_direction, edge2);
		det = edge1 * pvec;

		if(cull_backside) {
			if(det < _DEFAULT_EPSILON) return false;
			inv_det = 1.0f / det;
			tvec = _origin - v0;
			u = tvec * pvec;
			if(u < 0.0f || u > det) return false;
			qvec.cross(tvec, edge1);
			v = _direction * qvec;
			if(v < 0.0f || u + v > det) return false;

			if(distance) (*distance) = edge2 * qvec * inv_det;
			if(hit) {
				u *= inv_det;
				v *= inv_det;
				(*hit) = v0 * (1 - u - v) + v1 * u + v2 * v;
			}
		} else {
			if(_fzero(det)) return false;
			inv_det = 1.0f / det;
			tvec = _origin - v0;
			u = tvec * pvec * inv_det;
			if(u < 0.0f || u > 1.0f) return false;
			qvec.cross(tvec, edge1);
			v = _direction * qvec * inv_det;
			if(v < 0.0f || u + v > 1.0f) return false;

			if(distance) (*distance) = edge2 * qvec * inv_det;
			if(hit) (*hit) = v0 * (1 - u - v) + v1 * u + v2 * v;
		}
		return true;
	}

};
