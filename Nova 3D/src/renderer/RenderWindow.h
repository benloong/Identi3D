//
// File: RenderWindow.h
// ====================
//

#ifndef NOVA3D_SRC_RENDERER_RENDERWINDOW_H
#define NOVA3D_SRC_RENDERER_RENDERWINDOW_H

#include "src/nova-3d/NovaGeneral.h"
#include "src/renderer/RenderTarget.h"


namespace Nova3D
{

	class RenderDevice;

	class DebugManager;

	class RenderWindow : public RenderTarget
	{
	private:
		HWND window;
		DebugManager *debug_manager;
		TCHAR class_name[256];

	private:
		void	generateClassName(void);
		void	registerClass(void);
		HRESULT createWindow(int width, int height, const TCHAR *title);

	public:
		RenderWindow(DebugManager *dbgmgr);
		virtual ~RenderWindow(void);

		HRESULT assign(HWND wnd);
		HRESULT assign(int width, int height, const TCHAR *title);
		HRESULT assign(const RenderDevice *device, const TCHAR *title);
		HRESULT release();

		int		start(void);

		static	LRESULT WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		LRESULT dispatchWindowEvent(UINT msg, WPARAM wparam, LPARAM lparam);

		inline const HWND getWindowHandle(void) const { return window; }
	};

};

#endif