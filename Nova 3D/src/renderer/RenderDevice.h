//
// File: RenderDevice.h
// ====================
// 

#ifndef NOVA3D_SRC_RENDERER_RENDERDEVICE_H
#define NOVA3D_SRC_RENDERER_RENDERDEVICE_H

#include "src/nova-3d/NovaGeneral.h"

namespace Nova3D
{
	class DebugManager;

	class RenderDevice 
	{
	protected:
		HWND render_window;				// Render window
		HINSTANCE plugin_handle;		// Handle of plugins

		bool is_running;				// Is running or not

	public:
		RenderDevice(void) {};
		virtual ~RenderDevice(void) {};

		virtual HRESULT init(HWND window, UINT min_depth, UINT min_stencil) = 0;
		virtual void	release(void) = 0;
		virtual bool	isRunning(void) = 0;

		virtual HRESULT startRendering(bool clear_pixel, bool clear_depth, bool clear_stencil) = 0;
		virtual void	stopRendering(void) = 0;
		virtual HRESULT	clear(bool clear_pixel, bool clear_depth, bool clear_stencil) = 0;

		virtual void	setClearColor(float red, float green, float blue) = 0;
	};

	typedef class RenderDevice *RENDERDEVICE;

	extern "C"
	{
		HRESULT CreateRenderDevice(HMODULE plugin, RenderDevice **device, DebugManager *debug_manager);
		typedef HRESULT (*CREATERENDERDEVICE)(HMODULE plugin, RenderDevice **device, DebugManager *debug_manager);

		void ReleaseRenderDevice(RenderDevice **device);
		typedef void (*RELEASERENDERDEVICE)(RenderDevice **device);
	}

};

#endif
