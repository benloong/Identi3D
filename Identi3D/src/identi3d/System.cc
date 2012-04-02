//
// File: System.cc
// ===============
//

#include <src/identi3d/System.h>
#include <src/identi3d/EventDispatcher.h>

namespace Identi3D
{

	System::System(void)
	{
		_debugger = NULL;
		_confmgr = NULL;
		_dispatcher = NULL;

		_renderer = NULL;

		_state = SystemState_NotInitialized;
	}

	System::~System(void)
	{
		release();
	}

	HRESULT System::init(DWORD flag, const wchar_t *config_name)
	{
		HRESULT hr;

		if(_state != SystemState_NotInitialized) {
			_DebugPrint(_debugger, E_SYSTEM_INITIALIZED);
			return E_FAIL;
		}

		if((flag & SystemFlag_DisableDebugManager) == 0) {
			_debugger = new (std::nothrow) DebugManager;
			if(_debugger == NULL) {
				_DebugPrint(_debugger, E_OUT_OF_MEMORY);
				return E_FAIL;
			}
		} else {
			_debugger = NULL;
		}

		_dispatcher = new (std::nothrow) EventDispatcher;
		if(_dispatcher == NULL) {
			delete _debugger;
			_debugger = NULL;
			_DebugPrint(_debugger, E_OUT_OF_MEMORY);
		}

		_confmgr = new (std::nothrow) SettingsManager(_debugger);
		if(_confmgr == NULL) {
			delete _dispatcher;
			_dispatcher = NULL;
			delete _debugger;
			_debugger = NULL;
			_DebugPrint(_debugger, E_OUT_OF_MEMORY);
			return E_FAIL;
		}

		if(config_name != NULL) {
			if(_tcslen(config_name) < 260) {
				_DebugPrintV(_debugger, I_SYSTEM_LOAD_CONFIGURATION, config_name);
				hr = _confmgr->load(config_name);
				if(SUCCEEDED(hr)) {
					_DebugPrintV(_debugger, I_SYSTEM_CONFIGURATION_LOAD_SUCCESS);
					_tcscpy_s(_conf_path, config_name);
				} else {
					_DebugPrint(_debugger, E_SYSTEM_CONFIGURATION_LOAD_FAIL);
					if((flag & SystemFlag_AllowFallbackConfig) == NULL) {
						delete _confmgr;
						_confmgr = NULL;
						delete _dispatcher;
						_dispatcher = NULL;
						delete _debugger;
						_debugger = NULL;
						return E_FAIL;
					}
				}
			} else {
				_DebugPrint(_debugger, E_SYSTEM_CONFIGURATION_LOAD_FAIL);
				if((flag & SystemFlag_AllowFallbackConfig) == NULL) {
					delete _confmgr;
					_confmgr = NULL;
					delete _dispatcher;
					_dispatcher = NULL;
					delete _debugger;
					_debugger = NULL;
					return E_FAIL;
				}
			}
		}

		if(NULL == createRenderer()) {
			delete _confmgr;
			_confmgr = NULL;
			delete _dispatcher;
			_dispatcher = NULL;
			delete _debugger;
			_debugger = NULL;
			_DebugPrint(_debugger, E_SYSTEM_RENDERER_CREATE_FAILED);
			return E_FAIL;
		}

		_state = SystemState_Idle;
		_DebugPrintV(_debugger, I_SYSTEM_CREATE_SUCCESS);
		return S_OK;
	}

	HRESULT System::release(bool save_config)
	{
		if(_state == SystemState_NotInitialized) return S_FALSE;

		releaseRenderer();

		if(save_config)
		{
			_confmgr->save(_conf_path);
		}
		delete _confmgr;
		_confmgr = NULL;

		delete _dispatcher;

		_DebugPrintV(_debugger, I_SYSTEM_RELEASE_SCHEDULED);
		delete _debugger;
		_debugger = NULL;

		_state = SystemState_NotInitialized;
		return S_OK;
	}

	Renderer *System::createRenderer(void)
	{
		if(_renderer) delete _renderer;

		_renderer = new (std::nothrow) Renderer(_debugger);
		if(_renderer == NULL) {
			_DebugPrint(_debugger, E_OUT_OF_MEMORY);
			return NULL;
		}

		return _renderer;
	}

	void System::releaseRenderer(void)
	{
		delete _renderer;
		_renderer = NULL;
	}
	
	int System::start(void)
	{
		MSG msg;
		HRESULT hr;

		if(_state != SystemState_Idle) return -255;

		_state = SystemState_Listening;

		while(true) 
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if(msg.message == WM_QUIT) break;

			if(_renderer->_render_device->isRunning()) {
				hr = _renderer->_render_device->startRendering(true, true, true);
				if(FAILED(hr)) {
					_DebugPrint(_debugger, E_START_RENDERING_FAILURE);
				}
				_dispatcher->postEvent(Event_Rendering);
				_renderer->_render_device->endRendering();
			}
		}
		
		_state = SystemState_Idle;
		return static_cast<int>(msg.wParam);
	}

	void System::kill(void)
	{
		if(_renderer->_render_window)
			DestroyWindow(_renderer->_render_window->getHandle());
	}

};
