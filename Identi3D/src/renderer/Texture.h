//
// File: Texture.h
// ===============
// Type texture.
//

#ifndef IDENTI3D_SRC_RENDERER_TEXTURE_H
#define IDENTI3D_SRC_RENDERER_TEXTURE_H

#include <src/identi3d/General.h>
#include <src/identi3d/Basetypes.h>

namespace Identi3D
{
	
	struct TextureData
	{
		void *ptr;
		unsigned ref_count;
	};

	class Texture
	{
	private:

		explicit Texture(void);

		/*
		 * Create texture from data, data should reserve its validity.
		 */
		explicit Texture(TextureData &data) : _data(&data) {} ;

	public:
		
		/*
		 * Copy constructor.
		 */
		Texture(const Texture &tex)
		{
			_data = tex._data;
			_data->ref_count++;
		}

		/*
		 * Destructor.
		 */
		virtual ~Texture(void)
		{
			if(_data->ref_count) _data->ref_count--;
		}

		/*
		 * Assignment operator.
		 */
		Texture &operator=(const Texture &tex)
		{
			if(this == &tex) return *this;
			if(_data->ref_count) _data->ref_count--;
			_data = tex._data;
			_data->ref_count++;
			return *this;
		}

		/*
		 * Is equal.
		 */
		bool operator==(const Texture &tex) const
		{
			return (_data == tex._data);
		}

		/*
		 * Is unequal.
		 */
		bool operator!=(const Texture &tex) const
		{
			return (_data != tex._data);
		}

		/*
		 * Get pointer pointing to actual texture data.
		 */
		void *getPointer(void) const
		{
			return _data->ptr;
		}

	private:
		mutable TextureData *_data;
	};

}

#endif // IDENTI3D_SRC_RENDERER_TEXTURE_H