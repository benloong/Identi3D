//
// File: System.cc
// ===============
//

#include <src/system/System.h>
#include <src/system/EventDispatcher.h>
#include <src/identi3d/IdentiExceptions.h>
#include <src/system/SettingsWindow.h>

namespace Identi3D
{

	System::System(void)
	{
		// Initialize debugger.
		_debugger = ntnew DebugManager();
		if(_debugger == NULL) {
			MessageBoxA(NULL, E_SYSTEM_CREATE_DEBUGGER_FAILURE, "Error", MB_ICONERROR | MB_OK);
		}

		_confmgr = NULL;
		_dispatcher = NULL;

		_renderer = NULL;

		_state = SystemState_NotInitialized;
	}

	System::~System(void)
	{
		release();
		delete _debugger;
	}

	bool System::init(const wchar_t *config_name, const SystemStartupProperties &prop)
	{
		// Already initialized.
		if(_state != SystemState_NotInitialized) return true;

		_dispatcher = ntnew EventDispatcher(_debugger);
		if(_dispatcher == NULL) {
			if(_debugger)
				_debugger->print(__FILE__, __LINE__, false, E_SYSTEM_CREATE_DISPATCHER_FAILURE);
			MessageBoxA(NULL, E_SYSTEM_CREATE_DISPATCHER_FAILURE, "Error", MB_ICONERROR | MB_OK);
			delete _debugger;
			_debugger = NULL;
		}

		_confmgr = ntnew SettingsManager(_debugger);
		if(_confmgr == NULL) {
			if(_debugger)
				_debugger->print(__FILE__, __LINE__, false, E_SYSTEM_CREATE_CONFMGR_FAILURE);
			MessageBoxA(NULL, E_SYSTEM_CREATE_CONFMGR_FAILURE, "Error", MB_ICONERROR | MB_OK);
			delete _dispatcher;
			_dispatcher = NULL;
			delete _debugger;
			_debugger = NULL;
		}

		// Load configuration.
		if(config_name) {
			_conf_path = config_name;		// Make a copy of configuration path.
			if(_debugger)
				_debugger->print(__FILE__, __LINE__, true, I_SYSTEM_LOADING_CONFIGURATION, config_name);

			if(_confmgr->load(config_name)) {
				// Load successfully.
				if(_debugger)
					_debugger->print(__FILE__, __LINE__, true, I_SYSTEM_CONFIGURATION_LOAD_SUCCESS);
			} else {
				if(_debugger)
					_debugger->print(__FILE__, __LINE__, false, W_SYSTEM_CONFIGURATION_LOAD_FAILURE);
				MessageBoxA(NULL, W_SYSTEM_CONFIGURATION_LOAD_FAILURE, "Warning", MB_ICONWARNING | MB_OK);
				if(prop.disallow_fallback_config) {
					// If fallback configuration is not acceptable.
					if(_debugger)
						_debugger->print(__FILE__, __LINE__, false, E_FATAL_ERROR);
					MessageBoxA(NULL, E_FATAL_ERROR, "Error", MB_ICONERROR | MB_OK);
					delete _confmgr;
					_confmgr = NULL;
					delete _dispatcher;
					_dispatcher = NULL;
					delete _debugger;
					_debugger = NULL;
					return false;
				}
			}
		}
		
		switch(SettingsWindow::show(*_confmgr->getOptionTree()))
		{
		case SettingsWindowResult_Modified:
			_confmgr->save(_conf_path);
			break;
		case SettingsWindowResult_NoModification:
			break;
		case SettingsWindowResult_Cancelled:
		default:
			delete _confmgr;
			_confmgr = NULL;
			delete _dispatcher;
			_dispatcher = NULL;
			delete _debugger;
			_debugger = NULL;
			return false;
		}

		if(NULL == createRenderer()) {
			if(_debugger)
				_debugger->print(__FILE__, __LINE__, false, E_FATAL_ERROR);
			MessageBoxA(NULL, E_FATAL_ERROR, "Error", MB_ICONERROR | MB_OK);
			delete _confmgr;
			_confmgr = NULL;
			delete _dispatcher;
			_dispatcher = NULL;
			delete _debugger;
			_debugger = NULL;
			return false;
		}

		_state = SystemState_Idle;
		if(_debugger)
			_debugger->print(__FILE__, __LINE__, true, I_SYSTEM_CREATE_SUCCESS);

		return true;
	}

	bool System::release(bool save_config)
	{
		// System not initialized.
		if(_state == SystemState_NotInitialized) return true;
		
		_state = SystemState_NotInitialized;
		if(_debugger)
			_debugger->print(__FILE__, __LINE__, true, I_SYSTEM_RELEASING);

		// Stop listener.
		if(_state == SystemState_Listening) kill();

		// Kill renderer.
		releaseRenderer();

		// Write configuration back.
		if(save_config)	{
			_confmgr->save(_conf_path);
		}
		delete _confmgr;
		_confmgr = NULL;

		// Kill event dispatcher.
		delete _dispatcher;
		_dispatcher = NULL;

		return S_OK;
	}

	Renderer *System::createRenderer(void)
	{
		if(_renderer) delete _renderer;
		_renderer = ntnew Renderer(_debugger);
		if(_renderer == NULL) {
			if(_debugger)
				_debugger->print(__FILE__, __LINE__, false, E_SYSTEM_CREATE_RENDERER_FAILURE);
			MessageBoxA(NULL, E_SYSTEM_CREATE_RENDERER_FAILURE, "Error", MB_ICONERROR | MB_OK);
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

		if(_state != SystemState_Idle) return -255;

		_state = SystemState_Listening;

		while(true) 
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if(msg.message == WM_QUIT) break;

			if(_renderer->getDevice()->isRunning()) {
				if(!_renderer->getDevice()->startRendering(true, true, true)) {
					kill();
				} else {
					_dispatcher->postEvent(Event_Rendering);
					_renderer->getDevice()->endRendering();
				}
			}
		}
		
		_state = SystemState_Idle;
		return static_cast<int>(msg.wParam);
	}

	void System::kill(void)
	{
		if(_renderer && _renderer->getWindow())
			_renderer->getWindow()->release();
	}

};
