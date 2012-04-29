//
// File: Basetypes.h
// =================
// Basetypes of renderers and render devices.
//

#ifndef IDENTI3D_SRC_RENDERER_BASETYPES_H
#define IDENTI3D_SRC_RENDERER_BASETYPES_H

#include <src/identi3d/General.h>
#include <src/identi3d/Basetypes.h>

namespace Identi3D
{

	typedef struct
	{
		Color diffuse;	// diffuse light.
		Color ambient;	// ambient light.
		Color specular;	// specular light.
		Color emissive;	// emissive light.
		
		float power;	// specular light power.
	} Material;

}

#endif // IDENTI3D_SRC_RENDERER_BASETYPES_H