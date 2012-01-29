//
// File: RenderWindow.h
// ====================
//

#ifndef NOVA3D_SRC_RENDERER_RENDERWINDOW_H
#define NOVA3D_SRC_RENDERER_RENDERWINDOW_H

#include <src/nova-3d/NovaGeneral.h>
#include <src/renderer/RenderTarget.h>
#include <src/utils/KeyDefinition.h>

namespace Nova3D
{

	//
	// Class: RenderWindow <Inherits from RenderTarget>
	// ======
	// A window handle wrapper for rendering.
	//
	class RenderWindow : public RenderTarget
	{
	private:
		HWND window;
		RenderDevice *render_device;
		TCHAR class_name[256];

	private:
		void	generateClassName(void);
		void	registerClass(void);
		HRESULT createWindow(int width, int height, const TCHAR *title);

	public:
		RenderWindow();
		virtual ~RenderWindow(void);

		HRESULT assign(RenderDevice *device, const TCHAR *title);
		HRESULT release();

		int		start(void);

		static	LRESULT WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		LRESULT dispatchWindowEvent(UINT msg, WPARAM wparam, LPARAM lparam);

		virtual void onKeyboardInput(KeyType key, UINT repeat_times, bool previous_key_pressed) {};

		virtual void onStartRendering(void);
		virtual void onRendering(void) = 0;
		virtual void onEndRendering(void);

		HWND getHandle(void) { return window; }
		RenderDevice *getRenderDevice(void) { return render_device; }
	};

};

#endif