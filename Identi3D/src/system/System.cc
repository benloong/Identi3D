//
// File: System.cc
// ===============
//

#include <src/identi3d/System.h>
#include <src/identi3d/EventDispatcher.h>
#include <src/identi3d/IdentiExceptions.h>

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

	bool System::init(const std::wstring &config_name, const SystemStartupProperties &prop)
	{
		HRESULT hr;

		try
		{
			// Already initialized.
			if(_state != SystemState_NotInitialized) return true;

			// Create a DebugManager instance.
			if(!prop.disable_debug_manager) {
				_debugger = new DebugManager();
			}

			_dispatcher = new EventDispatcher(_debugger);
			_confmgr = new SettingsManager(_debugger);

			// Load configuration.
			if(config_name.length() > 0) {
				_DebugPrintV(_debugger, I_SYSTEM_LOAD_CONFIGURATION, config_name);
				if(_confmgr->load(config_name)) {
					// Load successfully.
					_DebugPrintV(_debugger, I_SYSTEM_CONFIGURATION_LOAD_SUCCESS);
					_conf_path = config_name;		// Make a copy of configuration path.
				} else {
					_DebugPrint(_debugger, E_SYSTEM_CONFIGURATION_LOAD_FAIL);
					if(prop.disallow_fallback_config) {
						// If fallback configuration is not acceptable.
						throw InvalidParametersException();
					}
				}
			}

			if(NULL == createRenderer()) throw InitializationFailedException();

			_state = SystemState_Idle;
			_DebugPrintV(_debugger, I_SYSTEM_CREATE_SUCCESS);
		} catch(std::exception &e) {
			if(_debugger) _debugger->print(__FILE__, __LINE__, e);

			// Whoa, crashed. Do all cleanup jobs.
			releaseRenderer();

			delete _confmgr;
			_confmgr = NULL;

			delete _dispatcher;
			_dispatcher = NULL;

			delete _debugger;
			_debugger = NULL;

			return false;
		}
		return true;
	}

	bool System::release(bool save_config)
	{
		// System not initialized.
		if(_state == SystemState_NotInitialized) return true;
		
		_DebugPrintV(_debugger, I_SYSTEM_RELEASING);

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

		// Kill debugger.
		delete _debugger;
		_debugger = NULL;

		_state = SystemState_NotInitialized;
		return S_OK;
	}

	Renderer *System::createRenderer(void)
	{
		try
		{
			if(_renderer) delete _renderer;
			_renderer = new Renderer(_debugger);
		} catch(std::exception &e) {
			if(_debugger) _debugger->print(__FILE__, __LINE__, e);
			delete _renderer;
			_renderer = NULL;
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

			try
			{
				if(_renderer->_render_device->isRunning()) {
					hr = _renderer->_render_device->startRendering(true, true, true);
					if(FAILED(hr)) throw RenderingProcedureFailedException();
					_dispatcher->postEvent(Event_Rendering);
					_renderer->_render_device->endRendering();
				}
			} catch(std::exception &e) {
				_dispatcher->postEvent(Event_RenderExceptionCaught);
				if(_debugger) _debugger->print(__FILE__, __LINE__, e);
			}
		}
		
		_state = SystemState_Idle;
		return static_cast<int>(msg.wParam);
	}

	void System::kill(void)
	{
		if(_renderer && _renderer->_render_window)
			DestroyWindow(_renderer->_render_window->getHandle());
	}

};
