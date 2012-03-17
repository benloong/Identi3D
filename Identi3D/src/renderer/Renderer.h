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
	private:
		RenderDevice	*render_device;
		HMODULE			plugin_handle;
		DebugManager	*debugger;

	public:
		
		Renderer(void) : render_device(NULL), plugin_handle(NULL), debugger(NULL) {} ;
		~Renderer(void) { releaseDevice(); }

		/*
		 * Create a render device.
		 */
		HRESULT	createDevice(RenderBackendType type);
		
		/*
		 * Release the device and do some cleaning.
		 */
		void releaseDevice(void);

		/*
		 * Get current render device.
		 */
		RenderDevice *getDevice(void) { return render_device; }
		
		/*
		 * Get the handle of plugin file.
		 */
		HMODULE getPluginsHandle(void) { return plugin_handle; }

		/*
		 * Set DebugManager handle.
		 */
		void setDebugManager(DebugManager *new_debugger = NULL) { debugger = new_debugger; }
	};

};

#endif // IDENTI3D_SRC_RENDERER_RENDERER_H