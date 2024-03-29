//
// File: Renderer.h
// ================
// Basic renderer for multiple interfaces.
//

#ifndef IDENTI3D_SRC_RENDERER_RENDERER_H
#define IDENTI3D_SRC_RENDERER_RENDERER_H

#include <src/identi3d/General.h>
#include <src/renderer/RenderDevice.h>
#include <src/utils/DebugFrame.h>

namespace Identi3D
{

	/*
	 * Types of render backend.
	 *
	 *	RenderBackendType_Direct3D9:	Microsoft DirectX 9 render backend.
	 *	RenderBackendType_OpenGL:		OpenGL render backend (to be implemented)
	 */
	enum RenderBackendType
	{
		RenderBackendType_Direct3D9	= 0,
		RenderBackendType_OpenGL	= 1,

		RenderBackendType_NoDevice	= 0xFFFF
	};

	class Renderer : public DebugFrame
	{
	private:
		RenderBackendType _backend_type;
		RenderDevice	*_render_device;
		HMODULE			_plugin_handle;
		OptionTree		*_global_option;
		RenderWindow	*_render_window;

	private:
		Renderer(Renderer &r);
		Renderer &operator =(const Renderer &rhs);

	public:

		Renderer(DebugManager *debugger = NULL);
		~Renderer(void) { releaseDevice(); }

		/*
		 * Create a render device.
		 */
		bool createDevice(RenderBackendType type);

		/*
		 * Create the default render device.
		 */
		bool createDefaultDevice(void);
		
		/*
		 * Release the device and do some cleaning.
		 */
		void releaseDevice(void);
		
		/*
		 * Assign a render target for the render device.
		 */
		bool assignRenderWindow(RenderWindow &window, const std::wstring &window_title);

		/*
		 * Get current render device.
		 */
		RenderDevice *getDevice(void) { return _render_device; }

		/*
		 * Get assigned render window.
		 */
		RenderWindow *getWindow(void) { return _render_window; }

	};

};

#endif // IDENTI3D_SRC_RENDERER_RENDERER_H