//
// File: Types.h
// =============
// Defines types for material & skin etc.
//

#ifndef NOVA3D_SRC_BASE_SKIN_H
#define NOVA3D_SRC_BASE_SKIN_H

#include <src/nova-3d/NovaGeneral.h>

namespace Nova3D
{

	typedef unsigned int MATERIAL_ID;
	typedef unsigned int TEXTURE_ID;

	class Material
	{
	public:
		Color	_diffuse;
		Color	_ambient;
		Color	_specular;
		Color	_emissive;
		float	_power;
	};

	class Texture
	{
	public:
		float	_alpha;
		TCHAR	_filename;
		void	*_data;

		Color	*_colorkey;
		DWORD	_keynum;
	};

	class Skin
	{
	public:
		bool		_alpha;
		MATERIAL_ID	_material;
		TEXTURE_ID	_texture;
	};

};

#endif
