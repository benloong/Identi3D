//
// File: ModuleTest.h
// ==================
//

#ifndef NOVA3D_MODULETEST_MODULETEST_H
#define NOVA3D_MODULETEST_MODULETEST_H

void registerClass(HINSTANCE hInstance);
bool initInstance(HINSTANCE, int);

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif