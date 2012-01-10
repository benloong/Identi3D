//
// File: App.cc
// ============
// 

#include "StdAfx.h"
#include "App.h"

using namespace Nova3D;

App::App(void)
{
	device = NULL;
	
	DebugManager::getInstance().createDebugConsole();
	DebugManager::getInstance().setOutputFlag(DebugManager::getInstance().getOutputFlag() | DebugFlag_ConsoleOutput);
	_DebugPrint(__T("Application loaded."));
}

HRESULT App::init(void)
{
	HRESULT hr;

	hr = SettingsManager::getInstance().read(__T(".nova_settings"));
	if(FAILED(hr)) {
		_DebugPrint(__T("Failed to read configuration."));
		return E_FAIL;
	}

	hr = Renderer::getInstance().createDevice(RenderBackendType_Direct3D9);
	if(FAILED(hr)) {
		_DebugPrint(__T("Failed to create render device."));
		return E_FAIL;
	}
	device = Renderer::getInstance().getDevice();
		
	hr = assign(device, __T("Nova 3D Application"));
	if(FAILED(hr)) {
		_DebugPrint(__T("Failed to create render window."));
		return E_FAIL;
	}
	return device->init(static_cast<RenderTarget *>(this), 0, 0);
}

int App::run(void)
{
	int retval;

	device->setClearColor(0, 0, 1.0f);
	retval = start();

	return retval;
}

App::~App(void)
{
	_DebugPrint(__T("Application exited."));
}


void App::onKeyboardInput(KeyType key, UINT repeat_times, bool previous_key_pressed)
{
	_DebugPrint(__T("Keyboard Input Event: %d, %d, %s"), key, repeat_times, previous_key_pressed ? __T("true") : __T("false"));
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
