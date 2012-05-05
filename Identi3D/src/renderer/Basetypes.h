//
// File: Basetypes.h
// =================
// Basetypes of renderers and render devices.
//

#ifndef IDENTI3D_SRC_RENDERER_BASETYPES_H
#define IDENTI3D_SRC_RENDERER_BASETYPES_H

#include <src/identi3d/General.h>
#include <src/identi3d/Basetypes.h>
#include <src/math/AlignedAllocator.h>

namespace Identi3D
{
	
	typedef unsigned int TEXTURE_ID;
	typedef unsigned int MATERIAL_ID;
	typedef std::vector<TEXTURE_ID> TextureList;

	class Material
	{
	private:

		/*
		 * Constructor: Create a empty material.
		 */
		Material(MATERIAL_ID id) : _id(id), _power(0.0f) {};

		/*
		 * Constructor: Set values of material.
		 */
		Material(MATERIAL_ID id, 
			const Color &diffuse, 
			const Color &ambient, 
			const Color &specular, 
			const Color &emissive, 
			float power) : 
				_id(id),
				_diffuse(diffuse),
				_ambient(ambient),
				_specular(specular),
				_emissive(emissive),
				_power(power) {} ;

		~Material(void) {} ;

	public:

		/*
		 * Set values of material.
		 */
		inline void set(const Color &diffuse,
						const Color &ambient, 
						const Color &specular, 
						const Color &emissive, 
						float power)
		{
			_diffuse = diffuse, _ambient = ambient;
			_specular = specular, _emissive = emissive;
			_power = power;
		}

		/*
		 * Set color of diffuse light.
		 */
		inline void setDiffuseColor(const Color &diffuse)
		{
			_diffuse = diffuse;
		}

		/*
		 * Set color of ambient light.
		 */
		inline void setAmbientColor(const Color &ambient)
		{
			_ambient = ambient;
		}

		/*
		 * Set color of specular light.
		 */
		inline void setSpecularColor(const Color &specular, float power)
		{
			_specualr = specular;
			_power = power;
		}

		/*
		 * Set color of emissive light.
		 */
		inline void setEmissiveColor(const Color &emissive)
		{
			_emissive = emissive;
		}
		
		/*
		 * Get color of diffuse light.
		 */
		inline const Color &getDiffuseColor(void) const
		{
			return _diffuse;
		}

		/*
		 * Get color of ambient light.
		 */
		inline const Color &getAmbientColor(void) const
		{
			return _ambient;
		}

		/*
		 * Get color of specular light.
		 */
		inline const Color &getSpecularColor(void) const
		{
			return _specular;
		}

		/*
		 * Get color of emissive light.
		 */
		inline const Color &getEmissiveColor(void) const
		{
			return _emissive;
		}

		/*
		 * Get power of specular light.
		 */
		inline float getSpecularPower(void) const
		{
			return _power;
		}

		/*
		 * Is equal.
		 */
		inline bool operator==(const Material &mat) const
		{
			return ((_id == mat._id) ||
				((_diffuse == mat._diffuse) &&
				 (_ambient == mat._ambient) &&
				 (_specular == mat._specular) &&
				 (_emissive == mat._emissive) &&
				 (_power == mat._power)));
		}

		/*
		 * Is inequal.
		 */
		inline bool operator!=(const Material &mat) const
		{
			return !((*this) == mat);
		}

	private:
		MATERIAL_ID	_id;

		Color _diffuse;		// diffuse light.
		Color _ambient;		// ambient light.
		Color _specular;	// specular light.
		Color _emissive;	// emissive light.
		float _power;		// specular light power.
	};

	typedef std::vector<Color, AlignedAllocator<Color>> ColorKeyList;

	class Texture
	{
	private:

		/*
		 * Constructor: Initialize values of texture.
		 */
		Texture(TEXTURE_ID id, const Resource &res, float alpha = 1.0f)
			: _id(id), _alpha(alpha)
		{
				_res = ntnew Resource(res);
				if(_res == NULL) throw std::bad_alloc();
		}

		/*
		 * Destructor.
		 */
		~Texture(void)
		{
			delete _res;
		}

	private:
		TEXTURE_ID _id;
		
		Resource *_res;			// texture data.
		float _alpha;			// texture transparency.
		ColorKeyList color_keys;// texture color keys.
	};

	struct Skin
	{
		float		alpha;		// skin transparency.
		MATERIAL_ID	material;	// skin material.
		TextureList	textures;	// skin textures.
	};
}

#endif // IDENTI3D_SRC_RENDERER_BASETYPES_H