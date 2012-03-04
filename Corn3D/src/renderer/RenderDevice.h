//
// File: RenderDevice.h
// ====================
// 

#ifndef CORN3D_SRC_RENDERER_RENDERDEVICE_H
#define CORN3D_SRC_RENDERER_RENDERDEVICE_H

#include <src/corn3d/General.h>

namespace Corn3D
{

	class RenderDevice 
	{
	public:
		RenderDevice(void) {};
		virtual ~RenderDevice(void) {};

		/*
		 * Initialize specified device.
		 */
		virtual HRESULT init(RenderTarget *target, OptionTree *option = NULL) = 0;

		/*
		 * Release specified device.
		 */
		virtual void	release(void) = 0;

		/*
		 * Is device running.
		 */
		virtual bool	isRunning(void) = 0;

		/*
		 * Start rendering of a frame.
		 */
		virtual HRESULT startRendering(bool clear_pixel, bool clear_depth, bool clear_stencil) = 0;

		/*
		 * End rendering of a frame.
		 */
		virtual void	endRendering(void) = 0;

		/*
		 * Clear the screen.
		 */
		virtual HRESULT	clear(bool clear_pixel, bool clear_depth, bool clear_stencil) = 0;

		/*
		 * Set color to fill the blank screen.
		 */
		virtual void	setClearColor(float red, float green, float blue) = 0;

		/*
		 * Get screen width.
		 */
		virtual const UINT	getWidth(void) const = 0;

		/*
		 * Get screen height.
		 */
		virtual const UINT	getHeight(void) const = 0;
	};

	typedef class RenderDevice *RENDERDEVICE;

	extern "C"
	{
		HRESULT CreateRenderDevice(HMODULE plugin, RenderDevice **device, DebugManager *debugger);
		typedef HRESULT (*CREATERENDERDEVICE)(HMODULE plugin, RenderDevice **device, DebugManager *debugger);

		void ReleaseRenderDevice(RenderDevice **device);
		typedef void (*RELEASERENDERDEVICE)(RenderDevice **device);
	}

};

#endif // CORN3D_SRC_RENDERER_RENDERDEVICE_H
