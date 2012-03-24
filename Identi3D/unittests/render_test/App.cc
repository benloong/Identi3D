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
	
	listener = new (std::nothrow) Listener;
	if(listener == NULL) return E_FAIL;

	hr = System::instance().init(NULL, __T("default.conf"));
	if(FAILED(hr)) return E_FAIL;
	
	System::instance().getEventDispatcher()->RegisterEventListener(listener);

	_renderer = System::instance().getRenderer();
	hr = _renderer->createDefaultDevice();
	if(FAILED(hr)) return E_FAIL;
	
	_window = new (std::nothrow) RenderWindow;
	if(_window == NULL) return E_FAIL;

	hr = _renderer->assignRenderWindow(_window, __T("Identi3D Test"));
	if(FAILED(hr)) return E_FAIL;

	_device = _renderer->getDevice();
	_debugger = System::instance().getDebugManager();
	return S_OK;
}

int App::run(void)
{
	int retval;

	_device->setClearColor(0, 0, 1.0f);
	retval = System::instance().start();

	return retval;
}

App::~App(void)
{
	System::instance().release();
	delete listener;
}
