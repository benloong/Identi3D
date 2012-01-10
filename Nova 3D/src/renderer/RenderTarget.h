//
// File: RenderTarget.h
// ====================
//

#ifndef NOVA3D_SRC_RENDERER_RENDERTARGET_H
#define NOVA3D_SRC_RENDERER_RENDERTARGET_H

#include "src/nova-3d/NovaGeneral.h"

namespace Nova3D
{
	class DebugManager;

	class RenderTarget
	{
	public:
		RenderTarget(void) {}
		virtual ~RenderTarget(void) {};

		virtual HWND getWindowHandle(void) = 0;
	};

};

#endif
