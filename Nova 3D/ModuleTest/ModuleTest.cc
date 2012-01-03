//
// File: ModuleTest.cpp
// ====================
//

#include "stdafx.h"
#include "ModuleTest.h"

#include "src/nova-3d/Nova.h"
#include "src/utils/SettingsManager.h"
#include "src/renderer/RenderWindow.h"

#pragma comment (lib, "utils_d.lib")

using namespace Nova3D;

const TCHAR *class_name		= __T("MODULE_TEST");
const TCHAR *app_title		= __T("Module Test");
DebugManager *dbgmgr;
SettingsManager *settingsmgr;
HWND hWnd;

const LONG window_style	= (	WS_OVERLAPPED	| \
							WS_CAPTION		| \
							WS_SYSMENU		| \
							WS_MINIMIZEBOX);

class testclass : public SettingsEnumerator
{
protected:
	void processEntries(const TCHAR *name, TCHAR *value, UINT buffer_size)
	{
		_DEBUGPRINT(dbgmgr, __T("\"%s\" = \"%s\""), name, value);
		_stprintf_s(value, buffer_size, __T("DEFAULT_VALUE_HERE"));
	}
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	int retval;
	//MSG msg;
	Renderer *renderer;
	RenderDevice *device;
	RenderWindow *rw;
	HRESULT hr;

	dbgmgr = new DebugManager();
	dbgmgr->createDebugConsole();
	dbgmgr->setOutputFlag(dbgmgr->getOutputFlag() | DebugFlag_ConsoleOutput);
	dbgmgr->print(__FILE__, __LINE__, __T("Program started."));

	settingsmgr = new SettingsManager();
	if(FAILED(settingsmgr->read(__T(".nova_settings")))) {
		_DEBUGPRINT(dbgmgr, __T("Failed to read configuration!"));
		delete settingsmgr;
		delete dbgmgr;
		return -255;
	}

	renderer = new Renderer(settingsmgr, dbgmgr);
	
	hr = renderer->createDevice(RenderBackendType_Direct3D9);
	if(FAILED(hr)) {
		_DEBUGPRINT(dbgmgr, E_FATAL_ERROR);
		delete renderer;
		delete settingsmgr;
		delete dbgmgr;
		return -255;
	}
	device = renderer->getDevice();

	rw = new RenderWindow(dbgmgr);
	hr = rw->assign(device, __T("Nova3D Test"));
	if(FAILED(hr)) {
		_DEBUGPRINT(dbgmgr, E_FATAL_ERROR);
		delete rw;
		delete renderer;
		delete settingsmgr;
		delete dbgmgr;
		return -255;
	}
	device->init(rw->getWindowHandle(), 0, 0);
	
	//registerClass(hInstance);
	//if (!initInstance(hInstance, nCmdShow)) 
	//	return -255;
	
	retval = rw->start();

	//while(GetMessage(&msg, NULL, 0, 0)) {
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	
	settingsmgr->write(__T(".nova_settings"));
	delete rw;
	delete renderer;
	delete settingsmgr;
	delete dbgmgr;
	return retval;
}

void registerClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= class_name;
	wcex.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wcex);
}

bool initInstance(HINSTANCE hInstance, int nCmdShow)
{

   hWnd = CreateWindow(class_name, app_title, window_style,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (hWnd == NULL)
	   return false;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(Msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return 0;
}
