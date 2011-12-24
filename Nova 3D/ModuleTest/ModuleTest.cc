//
// File: ModuleTest.cpp
// ====================
//

#include "stdafx.h"
#include "ModuleTest.h"

const TCHAR *class_name		= TEXT("MODULE_TEST");
const TCHAR *app_title		= TEXT("Module Test");

const LONG window_style	= (	WS_OVERLAPPED	| \
							WS_CAPTION		| \
							WS_SYSMENU		| \
							WS_MINIMIZEBOX);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;

	registerClass(hInstance);
	if (!initInstance(hInstance, nCmdShow)) 
		return -255;

	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
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
   HWND hWnd;

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
