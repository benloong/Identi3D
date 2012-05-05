//
// File: Material.h
// ================
// Type material.
//

#ifndef IDENTI3D_SRC_RENDERER_MATERIAL_H
#define IDENTI3D_SRC_RENDERER_MATERIAL_H

#include <src/identi3d/General.h>
#include <src/identi3d/Basetypes.h>

namespace Identi3D
{

	typedef unsigned int MATERIAL_ID;

	struct MaterialData
	{
		Color diffuse;	// diffuse light.
		Color ambient;	// ambient light.
		Color specular;	// specular light.
		Color emissive;	// emissive light.
		float power;	// specular light power.
		float alpha;	// material transparency.

		unsigned ref_count;	// reference count.
	};

	typedef std::vector<MaterialData *> MaterialDataList;

	class Material
	{
	private:
		explicit Material(void);

		/*
		 * Constructor: Initialize material by ID and data.
		 */
		explicit Material(MaterialData &data) : _data(&data) {} ;

	public:

		/*
		 * Copy constructor.
		 */
		Material(const Material &mat)
		{
			_data = mat._data;
			_data->ref_count++;
		}

		/*
		 * Destructor
		 */
		~Material(void)
		{
			if(_data->ref_count) _data->ref_count--;
		}

		/*
		 * Set values of material.
		 */
		inline void set(const Color &diffuse,
						const Color &ambient, 
						const Color &specular, 
						const Color &emissive, 
						float specular_power)
		{
			_data->diffuse = diffuse;
			_data->ambient = ambient;
			_data->specular = specular;
			_data->emissive = emissive;
			_data->power = specular_power;
		}

		/*
		 * Set color of diffuse light.
		 */
		inline void setDiffuseColor(const Color &diffuse)
		{
			_data->diffuse = diffuse;
		}

		/*
		 * Set color of ambient light.
		 */
		inline void setAmbientColor(const Color &ambient)
		{
			_data->ambient = ambient;
		}

		/*
		 * Set color of specular light.
		 */
		inline void setSpecularColor(const Color &specular, float power)
		{
			_data->specular = specular;
			_data->power = power;
		}

		/*
		 * Set color of emissive light.
		 */
		inline void setEmissiveColor(const Color &emissive)
		{
			_data->emissive = emissive;
		}
		
		/*
		 * Get color of diffuse light.
		 */
		inline const Color &getDiffuseColor(void) const
		{
			return _data->diffuse;
		}

		/*
		 * Get color of ambient light.
		 */
		inline const Color &getAmbientColor(void) const
		{
			return _data->ambient;
		}

		/*
		 * Get color of specular light.
		 */
		inline const Color &getSpecularColor(void) const
		{
			return _data->specular;
		}

		/*
		 * Get color of emissive light.
		 */
		inline const Color &getEmissiveColor(void) const
		{
			return _data->emissive;
		}

		/*
		 * Get power of specular light.
		 */
		inline float getSpecularPower(void) const
		{
			return _data->power;
		}

		/*
		 * Is equal.
		 */
		inline bool operator==(const Material &mat) const
		{
			return ((_data == mat._data) ||
				((_data->diffuse == mat._data->diffuse) &&
				 (_data->ambient == mat._data->ambient) &&
				 (_data->specular == mat._data->specular) &&
				 (_data->emissive == mat._data->emissive) &&
				 (_data->power == mat._data->power)));
		}

		/*
		 * Is unequal.
		 */
		inline bool operator!=(const Material &mat) const
		{
			return !((*this) == mat);
		}

		/*
		 * Assignment operator.
		 */
		inline Material &operator=(const Material &mat)
		{
			if(this == &mat) return *this;
			if(_data->ref_count) _data->ref_count--;
			_data = mat._data;
			_data->ref_count++;
			return *this;
		}

	private:
		mutable MaterialData *_data;
	};

}

#endif // IDENTI3D_SRC_RENDERER_MATERIAL_H
