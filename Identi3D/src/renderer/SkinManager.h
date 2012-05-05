//
// File: SkinManager.h
// ===================
// Skin management.
//

#ifndef IDENTI3D_SRC_RENDERER_SKINMANAGER_H
#define IDENTI3D_SRC_RENDERER_SKINMANAGER_H

#include <src/identi3d/General.h>
#include <src/renderer/Basetypes.h>
#include <src/utils/DebugFrame.h>

#include "Skin.h"

namespace Identi3D
{

	class SkinManager : public DebugFrame
	{
	private:
		SkinDataList _skins;
		MaterialDataList _materials;

		static unsigned _skin_list_limit;
		static unsigned _material_list_limit;

	public:
		SkinManager(void);
		~SkinManager(void);

		/*
		 * Create and return a handle to material.
		 */
		Material *createMaterial(const Color &diffuse,
								 const Color &ambient,
								 const Color &specular,
								 const Color &emissive,
								 float specular_power,
								 float alpha = 1.0f);

		/*
		 * Create a skin from the material.
		 */
		Skin *createSkin(const Material &mat);

		/*
		 * Create a skin from specified parameters.
		 */
		Skin *createSkin(const Color &diffuse,
						 const Color &ambient,
						 const Color &specular,
						 const Color &emissive,
						 float specular_power,
						 float alpha = 1.0f);

		/*
		 * Release material's actual memory.
		 */
		void releaseMaterial(Material &mat);

		/*
		 * Release skin's actual memory.
		 */
		void releaseSkin(Skin &s);

		/*
		 * Release useless resources.
		 */
		void compress(void);

		/*
		 * Reset list limitation.
		 */
		void reset(void);
	};

}

#endif // IDENTI3D_SRC_RENDERER_SKINMANAGER_H