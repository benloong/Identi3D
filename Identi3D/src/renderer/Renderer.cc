//
// File: Renderer.cc
// =================
//

#include <src/renderer/Renderer.h>
#include <src/renderer/RenderDevice.h>
#include <src/renderer/RenderWindow.h>
#include <src/identi3d/System.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{

#ifndef _DEBUG
	const wchar_t *render_plugin_path[] = {	__T("PluginDirect3D9.dll"),
											__T("PluginOpenGL.dll"), };
#else
	const wchar_t *render_plugin_path[] = {	__T("PluginDirect3D9_d.dll"),
											__T("PluginOpenGL_d.dll"), };
#endif

	Renderer::Renderer(DebugManager *debugger)
		: _render_device(NULL), _plugin_handle(NULL), _debugger(debugger), _render_window(NULL)
	{
		_backend_type = RenderBackendType_NoDevice;
		_global_option = System::instance().getSettingsManager()->getOptionTree();
	}

	HRESULT Renderer::createDevice(RenderBackendType type)
	{
		const wchar_t *path;
		CREATERENDERDEVICE createRenderDevice;
		HRESULT hr;

		switch(type)
		{
		case RenderBackendType_Direct3D9:
			path = render_plugin_path[0];
			break;
		case RenderBackendType_OpenGL:
			path = render_plugin_path[1];
			break;
		default:
			_DebugPrint(_debugger, E_INVALID_RENDER_PLUGIN_TYPE, type);
			return E_FAIL;
		}
		
		_plugin_handle = LoadLibrary(path);
		if(_plugin_handle == NULL) {
			_DebugPrint(_debugger, E_FILE_OPEN_FAILURE, path);
			return E_FAIL;
		}

		createRenderDevice = (CREATERENDERDEVICE)GetProcAddress(_plugin_handle, "CreateRenderDevice");
		if(createRenderDevice == NULL) {
			_DebugPrint(_debugger, E_RENDER_PLUGIN_LOAD_FAILURE, path);
			return E_FAIL;
		}

		hr = createRenderDevice(_plugin_handle, &_render_device, _debugger);
		if(FAILED(hr)) {
			_DebugPrint(_debugger, E_RENDER_DEVICE_CREATE_FAILURE, path);
			return E_FAIL;
		}

		_backend_type = type;
		return S_OK;
	}

	void Renderer::releaseDevice(void)
	{
		RELEASERENDERDEVICE releaseRenderDevice = NULL;

		if(_plugin_handle == NULL || _render_device == NULL) return ;
		releaseRenderDevice = (RELEASERENDERDEVICE)GetProcAddress(_plugin_handle, "ReleaseRenderDevice");
		if(releaseRenderDevice == NULL) return ;

		releaseRenderDevice(&_render_device);
		_render_device = NULL;
		FreeLibrary(_plugin_handle);
		_plugin_handle = NULL;
		_backend_type = RenderBackendType_NoDevice;
		_render_window = NULL;
	}
	
	HRESULT Renderer::createDefaultDevice(void)
	{
		HRESULT hr;
		RenderBackendType rbt;
		wchar_t value[64];

		hr = _global_option->getValue(__T("System.DefaultRenderDevice"), value, 64);
		if(FAILED(hr)) rbt = RenderBackendType_Direct3D9;
		else {
			if(_tcscmp(value, __T("opengl")) == 0) {
				rbt = RenderBackendType_OpenGL;
			} else {
				rbt = RenderBackendType_Direct3D9;
			}
		}

		hr = createDevice(rbt);
		if(FAILED(hr)) {
			if(rbt != RenderBackendType_Direct3D9) {
				hr = createDevice(RenderBackendType_Direct3D9);
				if(FAILED(hr)) {
					_DebugPrint(_debugger, E_NO_RENDER_DEVICE_AVAILABLE);
					return E_FAIL;
				}
			} else {
				_DebugPrint(_debugger, E_NO_RENDER_DEVICE_AVAILABLE);
				return E_FAIL;
			}
		}

		return S_OK;
	}

	HRESULT Renderer::assignRenderWindow(RenderWindow *window, const wchar_t *window_title)
	{
		HRESULT hr;

		if(!_render_device) {
			_DebugPrint(_debugger, E_NO_RENDER_DEVICE_AVAILABLE);
			return E_FAIL;
		}

		hr = window->assign(_render_device, window_title);
		if(FAILED(hr)) {
			_DebugPrint(_debugger, E_RENDER_WINDOW_ASSIGN_FAIL);
			return E_FAIL;
		}

		// No need to check system state: A renderer must be and can only be allocated via a valid System.
		_render_device->init(window, _global_option);
		if(FAILED(hr)) {
			_DebugPrint(_debugger, E_RENDER_WINDOW_ASSIGN_FAIL);
			return E_FAIL;
		}

		_render_window = window;
		_DebugPrint(_debugger, E_RENDER_WINDOW_ASSIGN_SUCCESS);
		return S_OK;
	}

};