//
// File: Polygon.h
// ===============
// Provide polygon calculation.
//

#ifndef IDENTI3D_SRC_MATH_POLYGON_H
#define IDENTI3D_SRC_MATH_POLYGON_H

#include <src/identi3d/General.h>
#include <src/math/AlignedAllocator.h>
#include <src/math/Vector.h>
#include <src/math/Plane.h>
#include <src/math/AABB.h>

namespace Identi3D
{

	typedef std::vector<unsigned int> IndexList;
	typedef std::vector<Vector3, AlignedAllocator<Vector3, 16>> VertexList;

	class __declspec(dllexport) Polygon
	{
	private:
		Plane		_plane;
		VertexList	_vertices;
		IndexList	_indices;
		
		AxisAlignedBoundingBox _aabb;

	public:

		/*
		 * Constructor: Do nothing.
		 */
		Polygon(void) {};

		/*
		 * Destructor: Do nothing.
		 */
		~Polygon(void) {};

		/*
		 * Set vertices and indices of polygon.
		 */
		inline void set(const VertexList &vertices, const IndexList &indices)
		{
			Vector3 edge0, edge1;

			_vertices = vertices;
			_indices = indices;
			edge0 = vertices[_indices[1]] - vertices[_indices[0]];
			for(int i = 2; i < indices.size(); i++) {
				edge1 = vertices[indices[i]] - vertices[indices[0]];
				if(fabs(edge0.getAngle(edge1)) > 1e-3) break;
			}
			_plane._normal.cross(edge0, edge1);
			_plane._normal.normalize();
			_plane._dist = -(_plane._normal * vertices[0]);
			_plane._point = vertices[0];

			calculateBoundingBox();
		}
		
	private:
		void calculateBoundingBox(void)
		{
			VertexList::iterator iter;
			_aabb._bounds[0] = _aabb._bounds[1] = _vertices[0];
			for(iter = _vertices.begin(); iter != _vertices.end(); ++iter) {
				if(_aabb._bounds[0].getX() > iter->getX())
					_aabb._bounds[0].setX(iter->getX());
				else if(_aabb._bounds[1].getX() < iter->getX())
					_aabb._bounds[1].setX(iter->getX());
				if(_aabb._bounds[0].getY() > iter->getY())
					_aabb._bounds[0].setX(iter->getY());
				else if(_aabb._bounds[1].getY() < iter->getY())
					_aabb._bounds[1].setX(iter->getY());
				if(_aabb._bounds[0].getZ() > iter->getZ())
					_aabb._bounds[0].setX(iter->getZ());
				else if(_aabb._bounds[1].getZ() < iter->getZ())
					_aabb._bounds[1].setX(iter->getZ());
			}
		}
	};

};

#endif
