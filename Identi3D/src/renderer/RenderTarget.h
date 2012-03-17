//
// File: RenderTarget.h
// ====================
// A target for rendering operation to take effect.
//

#ifndef IDENTI3D_SRC_RENDERER_RENDERTARGET_H
#define IDENTI3D_SRC_RENDERER_RENDERTARGET_H

#include <src/identi3d/General.h>

namespace Identi3D
{

	class RenderTarget
	{
	public:
		RenderTarget(void) {}
		virtual ~RenderTarget(void) {};
		
		/*
		 * Get the handle of target.
		 */
		virtual HWND getHandle(void) = 0;
	};

};

#endif // IDENTI3D_SRC_RENDERER_RENDERTARGET_H
