//
// File: Renderer.cc
// =================
//

#include <src/renderer/Renderer.h>
#include <src/renderer/RenderDevice.h>
#include <src/renderer/RenderWindow.h>
#include <src/system/System.h>

namespace Identi3D
{
	const int render_plugin_count = 2;
	
	const wchar_t *render_plugin_name[] = { L"Direct3D9",
											L"OpenGL"};

#ifndef _DEBUG
	const wchar_t *render_plugin_path[] = {	L"PluginDirect3D9.dll",
											L"PluginOpenGL.dll" };
#else
	const wchar_t *render_plugin_path[] = {	L"PluginDirect3D9_d.dll",
											L"PluginOpenGL_d.dll" };
#endif

	Renderer::Renderer(DebugManager *debugger)
		: DebugFrame(debugger), _render_device(NULL), _plugin_handle(NULL), _render_window(NULL)
	{
		_backend_type = RenderBackendType_NoDevice;
		if(System::instance().getSettingsManager())
			_global_option = System::instance().getSettingsManager()->getOptionTree();
	}

	bool Renderer::createDevice(RenderBackendType type)
	{
		int index = static_cast<int>(type);
		CREATERENDERDEVICE createRenderDeviceFunc;

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

		if(!createRenderDeviceFunc(_plugin_handle, &_render_device, _debugger)) {
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
		if(releaseRenderDeviceFunc == NULL) {
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
		int i;
		RenderBackendType rbt;

		if(!_global_option) {
			if(System::instance().getSettingsManager())
				_global_option = System::instance().getSettingsManager()->getOptionTree();
			else
				return false;
		}

		std::wstring result = _global_option->getValue(__T("System.DefaultRenderDevice"));
		if(result.length() == 0) rbt = RenderBackendType_Direct3D9;
		else {
			for(i = 0; i < render_plugin_count; i++) {
				if(result == render_plugin_name[i]) rbt = static_cast<RenderBackendType>(i);
			}
			if(i >= render_plugin_count) rbt = RenderBackendType_Direct3D9;
		}

		if(!createDevice(rbt)) {
			_printMessage(__FILE__, __LINE__, W_RENDERER_SPECIFIED_NOT_AVAILABLE);
			MessageBoxA(NULL, W_RENDERER_SPECIFIED_NOT_AVAILABLE, "Warning", MB_ICONWARNING | MB_OK);
			for(i = 0; i < render_plugin_count; i++) {
				if(i != rbt) {
					if(createDevice(static_cast<RenderBackendType>(i))) break;
				}
			}
			if(i >= render_plugin_count) {
				_printMessage(__FILE__, __LINE__, E_RENDERER_NO_PLUGIN_AVAILABLE);
				MessageBoxA(NULL, E_RENDERER_NO_PLUGIN_AVAILABLE, "Error", MB_ICONERROR | MB_OK);
				return false;
			}
		}

		return true;
	}

	bool Renderer::assignRenderWindow(RenderWindow &window, const std::wstring &window_title)
	{
		if(!_render_device) {
			_printMessage(__FILE__, __LINE__, E_RENDERER_NO_PLUGIN_AVAILABLE);
			return false;
		}

		if(!window.assign(*_render_device, window_title)) {
			_printMessage(__FILE__, __LINE__, E_RENDERER_ASSIGN_WINDOW_FAILURE);
			return false;
		}

		if(!_render_device->init(window, _global_option)) {
			_printMessage(__FILE__, __LINE__, E_RENDERER_ASSIGN_WINDOW_FAILURE);
			window.deassign();
			return FALSE;
		}

		_render_window = &window;
		_printVerboseMessage(__FILE__, __LINE__, I_RENDERER_ASSIGN_WINDOW_SUCCESS);
		return true;
	}

};