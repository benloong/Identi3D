//
// File: RenderWindow.h
// ====================
// A window render target.
//

#ifndef IDENTI3D_SRC_RENDERER_RENDERWINDOW_H
#define IDENTI3D_SRC_RENDERER_RENDERWINDOW_H

#include <src/identi3d/General.h>
#include <src/identi3d/Basetypes.h>

namespace Identi3D
{

	class RenderWindow
	{
	private:
		HWND			window;
		RenderDevice	*render_device;
		TCHAR			class_name[256];
		DebugManager	*debugger;

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
		HRESULT createWindow(int width, int height, const TCHAR *title);

		/*
		 * Send events to inherited processors.
		 */
		LRESULT dispatchWindowEvent(UINT msg, WPARAM wparam, LPARAM lparam);

	public:

		RenderWindow();
		virtual ~RenderWindow(void);

		/*
		 * Assign a render device for the window.
		 */
		HRESULT assign(RenderDevice *device, const TCHAR *title);

		/*
		 * Release the window and clean up.
		 */
		HRESULT release();

		/*
		 * Message processor.
		 * # Should not be called directly.
		 */
		static	LRESULT WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

		/*
		 * Receive keyboard input message.
		 */
		virtual void onKeyboardInput(KeyType key, UINT repeat_times, bool previous_key_pressed) {};

		/*
		 * Receive render start message.
		 */
		virtual void onStartRendering(void);

		/*
		 * Receive render message.
		 */
		virtual void onRendering(void) = 0;

		/*
		 * Receive render end message.
		 */
		virtual void onEndRendering(void);

		/*
		 * Get the handle of window.
		 */
		HWND getHandle(void) { return window; }

		/*
		 * Get assigned render device.
		 */
		RenderDevice *getRenderDevice(void) { return render_device; }
		
		/*
		 * Set DebugManager handle.
		 */
		void setDebugManager(DebugManager *new_debugger = NULL) { debugger = new_debugger; }
	};

};

#endif // IDENTI3D_SRC_RENDERER_RENDERWINDOW_H
