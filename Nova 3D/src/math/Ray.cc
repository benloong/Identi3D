//
// File: Ray.cc
// ============
//

#include <src/math/Ray.h>

#include <src/math/AABB.h>
#include <src/math/Matrix.h>
#include <src/math/Plane.h>

namespace Nova3D
{

	Ray::Ray(const Vector3 &origin, const Vector3 &direction)
	{ 
		set(origin, direction);
	}

	void Ray::set(const Vector3 &origin, const Vector3 &direction)
	{
		_origin = origin;
		_direction = direction;
		_direction.normalize();

		_inverse.set(1.0f / _direction._x, 
					 1.0f / _direction._y, 
					 1.0f / _direction._z);
		_sign[0] = _inverse._x < 0;
		_sign[1] = _inverse._y < 0;
		_sign[2] = _inverse._z < 0;
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

	bool Ray::intersect(const Plane &p, bool cull_backside, float *distance, Vector3 *hit) const
	{
		float f = p._normal * _direction;
		if(f > -_DEFAULT_EPSILON && (f < _DEFAULT_EPSILON || cull_backside)) return false;
		float t = (-(p._normal * _origin) + p._distance) / f;
		if(t < 0.0f) return false;

		if(distance) *distance = t;
		if(hit) *hit = _origin + _direction * t;
		return true;
	}

	bool Ray::intersect(const AxisAlignedBoundingBox &aabb, float *distance, Vector3 *hit) const
	{
		float tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (aabb._bounds[_sign[0]]._x - _origin._x) * _inverse._x;
		tmax = (aabb._bounds[1 - _sign[0]]._x - _origin._x) * _inverse._x;
		tymin = (aabb._bounds[_sign[1]]._y - _origin._y) * _inverse._y;
		tymax = (aabb._bounds[1 - _sign[1]]._y - _origin._y) * _inverse._y;

		if(tmin > tymax || tymin > tmax) return false;
		if(tymin > tmin) tmin = tymin;
		if(tymax < tmax) tmax = tymax;

		tzmin = (aabb._bounds[_sign[2]]._z - _origin._z) * _inverse._z;
		tzmax = (aabb._bounds[1 - _sign[2]]._z - _origin._z) * _inverse._z;

		if(tmin > tzmax || tzmin > tmax) return false;
		if(tzmin > tmin) tmin = tzmin;
		if(tzmax < tmax) tmax = tymax;

		if(tmax < 0) return false;
		if(hit) {
			if(tmin < 0) {
				*hit = _origin;
			} else {
				*hit = _origin + _direction * tmin;
			}
		}
		if(distance) *distance = (tmin > 0) ? tmin : 0;
		return true;
	}

};
