//
// File: App.cc
// ============
// 

#include "StdAfx.h"
#include "App.h"
#include <src/utils/CPU.h>

using namespace Identi3D;

App::App(void)
{
	_debugger = NULL;
}

HRESULT App::init(void)
{
	HRESULT hr;
	
	hr = System::getInstance().init(NULL, __T("default.conf"));
	if(FAILED(hr)) return E_FAIL;

	hr = System::getInstance().assignRenderWindow(this, 
		SystemFlag_CreateRenderer | SystemFlag_CreateDefaultDevice, __T("Identi3D Test"));
	if(FAILED(hr)) return E_FAIL;

	_device = System::getInstance().getRenderer()->getDevice();
	_debugger = System::getInstance().getDebugManager();
	return S_OK;
}

int App::run(void)
{
	int retval;

	_device->setClearColor(0, 0, 1.0f);
	retval = start();

	return retval;
}

App::~App(void)
{
	System::getInstance().release();
}


void App::onKeyboardInput(KeyType key, UINT repeat_times, bool previous_key_pressed)
{
	_DebugPrintV(_debugger, __T("Keyboard Input Event: %d, %d, %s"), key, repeat_times, previous_key_pressed ? __T("true") : __T("false"));
	switch(key)
	{
	case KeyType_Escape:
		release();
		return ;
	}
}

void App::onRendering(void)
{
	return ;
}
