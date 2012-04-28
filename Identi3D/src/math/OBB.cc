//
// File: OBB.h
// ===========
//

#include <src/math/OBB.h>

namespace Identi3D
{

	void projectOBB(const OrientedBoundingBox &obb, const Vector3 &vec, 
					float *min, float *max)
	{
		float d = vec * obb.getCenter();
		float r = obb.getExtentX() * fabs(vec * obb.getAxisX())
			+ obb.getExtentY() * fabs(vec * obb.getAxisY())
			+ obb.getExtentZ() * fabs(vec * obb.getAxisZ());

		if(min != NULL) *min = d - r;
		if(max != NULL) *max = d + r;
	}

	void projectTriangle(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
						 const Vector3 &vec, float *min, float *max)
	{
		float rmin = vec * v0;
		float rmax = rmin;
		
		float d = vec * v1;
		if(d < rmin) rmin = d;
		else if(d > rmax) rmax = d;

		d = vec * v2;
		if(d < rmin) rmin = d;
		else if(d > rmax) rmax = d;

		if(min != NULL) *min = rmin;
		if(max != NULL) *max = rmax;
	}

	bool OrientedBoundingBox::intersect(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) const
	{
		float min0, max0, min1, max1, d;
		Vector3 vec, edge[3];

		edge[0] = v1 - v0;
		edge[1] = v2 - v0;
		vec.cross(edge[0], edge[1]);

		min0 = vec * v0;
		max0 = min0;

		projectOBB(*this, vec, &min1, &max1);
		if(max1 < min0 || min1 > max0) return false;

		edge[2] = edge[1] - edge[0];
		for(int i = 0; i < 3; i++) 
		{
			vec = _axis[i];
			projectTriangle(v0, v1, v2, vec, &min0, &max0);
			d = vec * _center;
			min1 = d - _extent[i];
			max1 = d + _extent[i];
			if(max1 < min0 || min1 > max0) return false;

			for(int k = 0; k < 3; k++)
			{
				vec.cross(edge[k], _axis[i]);
				projectTriangle(v0, v1, v2, vec, &min0, &max0);
				projectOBB(*this, vec, &min1, &max1);
				if(max1 < min0 || min1 > max0) return false;
			}
		}

		return true;
	}

	OBBCullResult OrientedBoundingBox::cull(PlaneList &planes) const
	{
		Vector3 normal;
		float r, t;
		OBBCullResult result = OBBCullResult_Visible;
		PlaneList::iterator iter;

		for(iter = planes.begin(); iter != planes.end(); ++iter)
		{
			normal = iter->_normal * -1.0f;
			r = fabs(_extent[0] * (normal * _axis[0]))
				+ fabs(_extent[1] * (normal * _axis[1]))
				+ fabs(_extent[2] * (normal * _axis[2]));
			
			t = normal * _center - iter->_dist;
			if(t < -r) return OBBCullResult_Culled;
			if(t < r) result = OBBCullResult_Clipped;
		}

		return result;
	}

};
