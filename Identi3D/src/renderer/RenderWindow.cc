//
// File: RenderWindow.cc
// =====================
//

#include <src/renderer/RenderWindow.h>
#include <src/renderer/RenderDevice.h>
#include <src/system/System.h>
#include <src/system/EventDispatcher.h>

namespace Identi3D
{
	
	const LONG window_style	= (	WS_OVERLAPPED	| \
								WS_CAPTION		| \
								WS_SYSMENU		| \
								WS_MINIMIZEBOX);

	RenderWindow::RenderWindow(DebugManager *debugger) 
		: DebugFrame(debugger), _window(NULL)
	{
		generateClassName();
		registerClass();
	}

	RenderWindow::~RenderWindow(void)
	{
		release();
	}

	bool RenderWindow::assign(RenderDevice &device, const std::wstring &title)
	{
		release();
		if(!createWindow(device.getWidth(), device.getHeight(), title)) {
			_printMessage(__FILE__, __LINE__, E_WINDOW_CREATE_FAILURE, GetLastError());
			return false;
		}

		_printVerboseMessage(__FILE__, __LINE__, I_WINDOW_CREATE_SUCCESS, title);
		_render_device = &device;
		return true;
	}

	void RenderWindow::release(void)
	{
		if(_window != NULL) {
			DestroyWindow(_window);
			_window = NULL;
		}
		_render_device = NULL;
	}

	void RenderWindow::generateClassName(void)
	{
		const wchar_t random_table[37] = __T("abcdefghijklmnopqrstuvwxyz0123456789");
		const UINT random_length = 7;

		_tcscpy_s(_class_name, __T("IRW"));
		wchar_t *p = _class_name + 3;
		srand((unsigned int)time(NULL));
		for(int i = 0; i < random_length; i++) {
			*p = random_table[rand() % 36];
			p++;
		}
		*p = __T('\0');

		return ;
	}

	void RenderWindow::registerClass(void)
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WindowProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= GetModuleHandle(NULL);
		wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= _class_name;
		wcex.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

		RegisterClassEx(&wcex);
	}

	bool RenderWindow::createWindow(int width, int height, const std::wstring &title)
	{
		_window = CreateWindow(
			_class_name, title.c_str(), window_style, 
			CW_USEDEFAULT, 0, width, height, 
			NULL, NULL, GetModuleHandle(0), NULL);
		
		if(_window == NULL) return false;

		ShowWindow(_window, SW_NORMAL);
		UpdateWindow(_window);
		return true;
	}

	LRESULT RenderWindow::WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return System::instance().getEventDispatcher()->postWindowMessage(wnd, msg, wparam, lparam);
	}

};