//
// File: RenderTarget.h
// ====================
// A target for rendering operation to take effect.
//

#ifndef CORN3D_SRC_RENDERER_RENDERTARGET_H
#define CORN3D_SRC_RENDERER_RENDERTARGET_H

#include <src/corn3d/General.h>

namespace Corn3D
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

#endif // CORN3D_SRC_RENDERER_RENDERTARGET_H
