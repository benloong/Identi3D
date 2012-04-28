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
	_window = NULL;
	_listener = NULL;
	_renderer = NULL;
	_device = NULL;
}

bool App::init(void)
{
	_listener = ntnew Listener;
	if(_listener == NULL) return false;

	SystemStartupProperties prop;
	if(!System::instance().init(L"default.conf", prop)) return false;
	
	System::instance().getEventDispatcher()->RegisterEventListener(*_listener);

	_renderer = System::instance().getRenderer();
	if(!_renderer->createDefaultDevice()) {
		System::instance().release(false);
		delete _listener;
		_listener = NULL;
		return false;
	}
	
	_window = ntnew RenderWindow;
	if(_window == NULL) {
		System::instance().release(false);
		delete _listener;
		_listener = NULL;
		return false;
	}

	if(!_renderer->assignRenderWindow(*_window, __T("Identi3D Test"))) {
		System::instance().release(false);
		delete _window;
		_window = NULL;
		delete _listener;
		_listener = NULL;
		return false;
	}

	_device = _renderer->getDevice();
	_debugger = System::instance().getDebugManager();
	return true;
}

int App::run(void)
{
	_device->setClearColor(0, 0, 1.0f);
	return System::instance().start();
}

App::~App(void)
{
	System::instance().release();
	delete _window;
	delete _listener;
}
