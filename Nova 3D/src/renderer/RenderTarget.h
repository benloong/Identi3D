//
// File: RenderTarget.h
// ====================
//

#ifndef NOVA3D_SRC_RENDERER_RENDERTARGET_H
#define NOVA3D_SRC_RENDERER_RENDERTARGET_H

#include <src/nova-3d/NovaGeneral.h>

namespace Nova3D
{

	//
	// Class: RenderTarget
	// ======
	// Target for rendering outputs.
	//
	class RenderTarget
	{
	public:
		RenderTarget(void) {}
		virtual ~RenderTarget(void) {};

		//
		// Function: getHandle(void)
		// =========
		// Retrive handle of render target.
		//
		virtual HWND getHandle(void) = 0;
	};

};

#endif
