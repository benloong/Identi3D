//
// File: System.cc
// ===============
//

#include <src/identi3d/System.h>

namespace Identi3D
{

	System::System(void)
	{
		_isload = false;

		_debugger = NULL;
		_confmgr = NULL;

		_renderer = NULL;
	}

	System::~System(void)
	{
		release();
	}

	HRESULT System::init(DWORD flag, const TCHAR *config_name)
	{
		HRESULT hr;

		if(_isload) {
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

		_confmgr = new (std::nothrow) SettingsManager;
		if(_confmgr == NULL) {
			delete _debugger;
			_debugger = NULL;
			_DebugPrint(_debugger, E_OUT_OF_MEMORY);
			return E_FAIL;
		}

		if(config_name != NULL) {
			if(_tcslen(config_name) >= 260) goto LOADFAIL;
			_DebugPrintV(_debugger, I_SYSTEM_LOAD_CONFIGURATION, config_name);
			hr = _confmgr->load(config_name);
			if(SUCCEEDED(hr)) {
				_DebugPrintV(_debugger, I_SYSTEM_CONFIGURATION_LOAD_SUCCESS);
				_tcscpy_s(_conf_path, config_name);
			} else {
	LOADFAIL:
			{
				_DebugPrint(_debugger, E_SYSTEM_CONFIGURATION_LOAD_FAIL);
				if((flag & SystemFlag_AllowFallbackConfig) == NULL) {
					delete _debugger;
					_debugger = NULL;
					delete _confmgr;
					_confmgr = NULL;
					return E_FAIL;
				}
			}

			}
		}

		if(flag & SystemFlag_CreateRenderer) {
			createRenderer(flag & SystemFlag_CreateDefaultDevice);
		} else {
			_renderer = NULL;
		}

		_isload = true;
		_DebugPrintV(_debugger, I_SYSTEM_CREATE_SUCCESS);
		return S_OK;
	}

	HRESULT System::release(bool save_config)
	{
		if(!_isload) return S_FALSE;

		if(save_config)
		{
			_confmgr->save(_conf_path);
		}

		releaseRenderer();
		delete _confmgr;
		_confmgr = NULL;

		_DebugPrintV(_debugger, I_SYSTEM_RELEASE_SCHEDULED);
		delete _debugger;
		_debugger = NULL;

		_isload = false;
		return S_OK;
	}

	Renderer *System::createRenderer(DWORD flag)
	{
		TCHAR value[64];
		HRESULT hr;
		DebugManager *backup;

		if(!_isload) return NULL;
		
		backup = _debugger;
		if(flag & SystemFlag_DisableDebugManager) {
			_debugger = NULL;
		}

		_renderer = new (std::nothrow) Renderer(backup);
		if(_renderer == NULL) {
			_DebugPrint(_debugger, E_OUT_OF_MEMORY);
			_debugger = backup;
			return NULL;
		}

		if(flag & SystemFlag_CreateDefaultDevice) {
			hr = _confmgr->getOptionTree()->getValue(__T("System.DefaultRenderDevice"), value, 64);
			if(FAILED(hr)) goto LOAD_DEFAULT_DEVICE;
			if(_tcscmp(value, __T("opengl")) == 0) {
				hr = _renderer->createDevice(RenderBackendType_OpenGL);
				if(FAILED(hr)) goto LOAD_DEFAULT_DEVICE;
			} else {
	LOAD_DEFAULT_DEVICE:
			{
				hr = _renderer->createDevice(RenderBackendType_Direct3D9);
				if(FAILED(hr)) {
					_DebugPrint(_debugger, E_SYSTEM_NO_DEVICE_AVAILABLE);
				}
			}

			}
		} else {
			_DebugPrint(_debugger, E_SYSTEM_NO_DEVICE_AVAILABLE);
		}

		_debugger = backup;
		return _renderer;
	}

	void System::releaseRenderer(void)
	{
		_renderer->releaseDevice();
		delete _renderer;
		_renderer = NULL;
	}
	
	int System::start(void)
	{
		MSG msg;

		while(true) 
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if(msg.message == WM_QUIT) break;
		}

		return static_cast<int>(msg.wParam);
	}
};
