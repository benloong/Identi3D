//
// File: RenderWindow.h
// ====================
// A window render target.
//

#ifndef IDENTI3D_SRC_RENDERER_RENDERWINDOW_H
#define IDENTI3D_SRC_RENDERER_RENDERWINDOW_H

#include <src/identi3d/General.h>
#include <src/utils/DebugFrame.h>

namespace Identi3D
{

	class RenderWindow : public DebugFrame
	{
		friend class Renderer;

	private:
		HWND			_window;
		RenderDevice	*_render_device;
		wchar_t			_class_name[256];

	private:

		/*
		 * Generate a random class name string.
		 */
		void	generateClassName(void);

		/*
		 * Register the class of window.
		 */
		void	registerClass(void);

		/*
		 * Create window with specified attributes.
		 */
		bool createWindow(int width, int height, const std::wstring &title);
		
		/*
		 * Assign a render device for the window.
		 */
		bool assign(RenderDevice &device, const std::wstring &title);

		/*
		 * Unload render device.
		 */
		inline void deassign(void) 
		{
			release();
		}

	public:

		RenderWindow(DebugManager *debugger = NULL);
		~RenderWindow(void);

		/*
		 * Release the window and clean up.
		 */
		void release();

		/*
		 * Message processor.
		 * # Should not be called directly.
		 */
		static LRESULT WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

		/*
		 * Get the handle of window.
		 */
		HWND getHandle(void) { return _window; }

		/*
		 * Get assigned render device.
		 */
		RenderDevice *getRenderDevice(void) { return _render_device; }

	};

};

#endif // IDENTI3D_SRC_RENDERER_RENDERWINDOW_H
