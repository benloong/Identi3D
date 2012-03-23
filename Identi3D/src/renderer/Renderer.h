//
// File: Renderer.h
// ================
// Basic renderer for multiple interfaces.
//

#ifndef IDENTI3D_SRC_RENDERER_RENDERER_H
#define IDENTI3D_SRC_RENDERER_RENDERER_H

#include <src/identi3d/General.h>
#include <src/renderer/RenderDevice.h>

namespace Identi3D
{

	/*
	 * Types of render backend.
	 *
	 *	RenderBackendType_Direct3D9:	Microsoft(R) DirectX(R) 9 render backend.
	 *	RenderBackendType_OpenGL:		OpenGL render backend (under implementation)
	 */
	enum RenderBackendType
	{
		RenderBackendType_Direct3D9	= 0,
		RenderBackendType_OpenGL	= 1,

		RenderBackendType_ForceDword= 0xFFFFFFFF
	};

	class Renderer
	{
		friend class System;

	private:
		RenderDevice	*_render_device;
		HMODULE			_plugin_handle;
		DebugManager	*_debugger;

	private:
		Renderer(DebugManager *debugger) : _render_device(NULL), _plugin_handle(NULL), _debugger(debugger) {} ;
		Renderer(Renderer &r);
		~Renderer(void) { releaseDevice(); }

	public:
		/*
		 * Create a render device.
		 */
		HRESULT	createDevice(RenderBackendType type);
		
		/*
		 * Release the device and do some cleaning.
		 */
		void releaseDevice(void);
		
		/*
		 * Assign a render target for the render device.
		 */
		HRESULT assignRenderWindow(RenderWindow *window, const TCHAR *window_title = IDENTI3D_NAME);

		/*
		 * Get current render device.
		 */
		RenderDevice *getDevice(void) { return _render_device; }
		
		/*
		 * Get the handle of plugin file.
		 */
		HMODULE getPluginsHandle(void) { return _plugin_handle; }

	};

};

#endif // IDENTI3D_SRC_RENDERER_RENDERER_H