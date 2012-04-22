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
	const int render_plugin_count = 2;
	
	const wchar_t *render_plugin_name[] = { L"Direct3D9",
											L"OpenGL"}

#ifndef _DEBUG
	const wchar_t *render_plugin_path[] = {	L"PluginDirect3D9.dll",
											L"PluginOpenGL.dll" };
#else
	const wchar_t *render_plugin_path[] = {	L"PluginDirect3D9_d.dll",
											L"PluginOpenGL_d.dll" };
#endif

	Renderer::Renderer(DebugManager *debugger)
		: _render_device(NULL), _plugin_handle(NULL), _debugger(debugger), _render_window(NULL)
	{
		_backend_type = RenderBackendType_NoDevice;
		if(System::instance().getSettingsManager())
			_global_option = System::instance().getSettingsManager()->getOptionTree();
	}

	bool Renderer::createDevice(RenderBackendType type)
	{
		int index = static_cast<int>(type);
		CREATERENDERDEVICE createRenderDeviceFunc;
		HRESULT hr;

		if(index >= render_plugin_count) {
			_printMessage(__FILE__, __LINE__, E_RENDERER_INVALID_PLUGIN_TYPE, type);
			return false;
		}
		
		_plugin_handle = LoadLibrary(render_plugin_path[index]);
		if(_plugin_handle == NULL) {
			_printMessage(__FILE__, __LINE__, E_FILE_OPEN_FAILURE, render_plugin_path[index]);
			return false;
		}

		createRenderDeviceFunc = (CREATERENDERDEVICE)GetProcAddress(_plugin_handle, "CreateRenderDevice");
		if(createRenderDeviceFunc == NULL) {
			_printMessage(__FILE__, __LINE__, E_RENDERER_PLUGIN_LOAD_FAILURE, render_plugin_name[index]);
			FreeLibrary(_plugin_handle);
			_plugin_handle = NULL;
			return false;
		}

		hr = createRenderDeviceFunc(_plugin_handle, &_render_device, _debugger);
		if(FAILED(hr)) {
			_printMessage(__FILE__, __LINE__, E_RENDERER_DEVICE_CREATE_FAILURE, render_plugin_name[index]);
			FreeLibrary(_plugin_handle);
			_plugin_handle = NULL;
			return false;
		}

		_backend_type = type;
		_printVerboseMessage(__FILE__, __LINE__, I_RENDERER_PLUGIN_LOADED, render_plugin_name[type]);
		return true;
	}

	void Renderer::releaseDevice(void)
	{
		RELEASERENDERDEVICE releaseRenderDeviceFunc = NULL;

		if(_plugin_handle == NULL || _render_device == NULL) return ;
		releaseRenderDeviceFunc = (RELEASERENDERDEVICE)GetProcAddress(_plugin_handle, "ReleaseRenderDevice");
		if(releaseRenderDevice == NULL) {
			FreeLibrary(_plugin_handle);
			_plugin_handle = NULL;
			return ;
		}

		releaseRenderDeviceFunc(&_render_device);
		_render_device = NULL;
		FreeLibrary(_plugin_handle);
		_plugin_handle = NULL;
		_backend_type = RenderBackendType_NoDevice;
		if(_render_window) {
			_render_window->deassign();
			_render_window = NULL;
		}
	}
	
	bool Renderer::createDefaultDevice(void)
	{
		HRESULT hr;
		RenderBackendType rbt;
		wchar_t value[64];

		if(!_global_option) {
			if(System::instance().getSettingsManager())
				_global_option = System::instance().getSettingsManager()->getOptionTree();
			else
				return false;
		}

		std::wstring result = _global_option->getValue(__T("System.DefaultRenderDevice"));
		if(!result.length()) rbt = RenderBackendType_Direct3D9;
		else {
			for(int i = 0; i < render_plugin_count; i++) {
				if(result == render_plugin_name[i]) rbt = i;
			}
			if(i >= render_plugin_count) rbt = RenderBackendType_Direct3D9;
		}

		if(!createDevice(rbt)) {
			_printMessage(__FILE__, __LINE__, W_RENDERER_SPECIFIED_NOT_AVAILABLE);
			MessageBoxA(NULL, W_RENDERER_SPECIFIED_NOT_AVAILABLE, "Warning", MB_OK);
			for(int i = 0; i < render_plugin_count; i++) {
				if(i != static_cast<int>(rbt)) {
					if(createDevice(i)) break;
				}
			}
			if(i >= render_plugin_count) {
				_printMessage(__FILE__, __line__, E_RENDERER_NO_PLUGIN_AVAILABLE);
				MessageBoxA(NULL, E_RENDERER_NO_PLUGIN_AVAILABLE, "Error", MB_OK);
				return false;
			}
		}

		return true;
	}

	bool Renderer::assignRenderWindow(RenderWindow *window, const wchar_t *window_title)
	{
		HRESULT hr;

		if(!_render_device) {
			_printMessage(__FILE__, __LINE__, E_RENDERER_NO_PLUGIN_AVAILABLE);
			return false;
		}

		hr = window->assign(_render_device, window_title);
		if(FAILED(hr)) {
			_printMessage(__FILE__, __LINE__, E_RENDERER_ASSIGN_WINDOW_FAILURE);
			return false;
		}

		_render_device->init(window, _global_option);
		if(FAILED(hr)) {
			_printMessage(__FILE__, __LINE__, E_RENDERER_ASSIGN_WINDOW_FAILURE);
			window->deassign();
			return FALSE;
		}

		_render_window = window;
		_printVerboseMessage(__FILE, __LINE__, I_RENDERER_ASSIGN_WINDOW_SUCCESS);
		return true;
	}

};