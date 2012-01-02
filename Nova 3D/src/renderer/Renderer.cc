//
// File: Renderer.cc
// =================
//

#include "src/renderer/Renderer.h"
#include "src/renderer/RenderDevice.h"

#include "src/utils/DebugManager.h"
#include "src/utils/SettingsManager.h"

#include <exception>

#define _THROW_FATAL_ERROR(error_string) \
	{\
		char tmp[256];\
		sprintf_s(tmp, "%ls", error_string);\
		throw std::exception(tmp);\
	}

namespace Nova3D
{

#ifndef _DEBUG
	const TCHAR *render_plugin_path[] = {	__T("libdirect3d9.dll"),
											__T("libopengl.dll"), };
#else
	const TCHAR *render_plugin_path[] = {	__T("libdirect3d9_d.dll"),
											__T("libopengl_d.dll"), };
#endif

	const TCHAR *render_settings_group_name = __T("Graphics");

	Renderer::Renderer(SettingsManager *settingsmgr, DebugManager *debugmgr)
	{
		if(settingsmgr == NULL) {
			_DEBUGPRINT(debugmgr, E_INVALID_PARAMETERS);
			_THROW_FATAL_ERROR(E_INVALID_PARAMETERS);
		}
		settings_manager = settingsmgr;
		debug_manager = debugmgr;

		render_device = NULL;
		plugin_handle = NULL;

		_DEBUGPRINT(debug_manager, I_RENDERER_INITIALIZED);
	}

	Renderer::~Renderer(void)
	{
		releaseDevice();
	}

	HRESULT Renderer::createDevice(RenderBackendType type)
	{
		const TCHAR *path;
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
			_DEBUGPRINT(debug_manager, E_INVALID_RENDER_PLUGIN_TYPE, type);
			return E_FAIL;
		}
		
		plugin_handle = LoadLibrary(path);
		if(plugin_handle == NULL) {
			_DEBUGPRINT(debug_manager, E_FILE_OPEN_FAILURE, path);
			return E_FAIL;
		}

		createRenderDevice = (CREATERENDERDEVICE)GetProcAddress(plugin_handle, "CreateRenderDevice");
		if(createRenderDevice == NULL) {
			_DEBUGPRINT(debug_manager, E_RENDER_PLUGIN_LOAD_FAILURE, path);
			return E_FAIL;
		}

		hr = createRenderDevice(plugin_handle, &render_device, debug_manager);
		if(FAILED(hr)) {
			_DEBUGPRINT(debug_manager, E_RENDER_DEVICE_CREATE_FAILURE, path);
			return E_FAIL;
		}

		render_device->getSettingsEnumerator().enumerateSettings(render_settings_group_name, settings_manager);
		return S_OK;
	}

	void Renderer::releaseDevice(void)
	{
		RELEASERENDERDEVICE releaseRenderDevice = NULL;

		if(plugin_handle == NULL || render_device == NULL) return ;
		releaseRenderDevice = (RELEASERENDERDEVICE)GetProcAddress(plugin_handle, "ReleaseRenderDevice");
		if(releaseRenderDevice == NULL) return ;
		releaseRenderDevice(&render_device);

		render_device = NULL;
		FreeLibrary(plugin_handle);
		plugin_handle = NULL;
	}

};