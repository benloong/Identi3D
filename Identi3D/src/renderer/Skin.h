//
// File: Skin.h
// ============
// A structure combining a material and textures.
//

#ifndef IDENTI3D_SRC_RENDERER_SKIN_H
#define IDENTI3D_SRC_RENDERER_SKIN_H 

#include <src/identi3d/General.h>

#include "Material.h"

namespace Identi3D
{

	struct SkinData
	{
		Material *mat;
		// TODO: Place texture information here.
		unsigned ref_count;
	};

	typedef std::vector<SkinData *> SkinDataList;

	class Skin
	{
		friend class SkinManager;

	private:

		explicit Skin(void);

		/*
		 * Create skin from data, data should reserve its validity.
		 */
		explicit Skin(SkinData &data) : _data(&data) {} ;

	public:

		/*
		 * Copy constructor.
		 */
		Skin(const Skin &s)
		{
			_data = s._data;
			_data->ref_count++;
		}

		/*
		 * Destructor.
		 */
		~Skin(void)
		{
			if(_data->ref_count) _data->ref_count--;
		}
	
		/*
		 * Assignment operator.
		 */
		inline Skin &operator=(const Skin &s)
		{
			if(this == &s) return *this;
			if(_data->ref_count) _data->ref_count--;
			_data = s._data;
			_data->ref_count++;
			return *this;
		}

		/*
		 * Is equal.
		 */
		inline bool operator ==(const Skin &s) const
		{
			return ((_data == s._data) || (_data->mat == s._data->mat));
		}

		/*
		 * Is unequal.
		 */
		inline bool operator !=(const Skin &s) const
		{
			return !(*this == s);
		}

		/*
		 * Get the material of skin.
		 */
		inline Material *getMaterial(void) const
		{
			return _data->mat;
		}
		
	private:
		mutable SkinData *_data;
	};

}

#endif // IDENTI3D_SRC_RENDERER_SKIN_H