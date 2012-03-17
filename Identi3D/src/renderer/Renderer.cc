//
// File: Renderer.cc
// =================
//

#include <src/renderer/Renderer.h>
#include <src/renderer/RenderDevice.h>

#include <src/utils/DebugManager.h>

namespace Identi3D
{

#ifndef _DEBUG
	const TCHAR *render_plugin_path[] = {	__T("PluginDirect3D9.dll"),
											__T("PluginOpenGL.dll"), };
#else
	const TCHAR *render_plugin_path[] = {	__T("PluginDirect3D9_d.dll"),
											__T("PluginOpenGL_d.dll"), };
#endif

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
			_DebugPrint(debugger, E_INVALID_RENDER_PLUGIN_TYPE, type);
			return E_FAIL;
		}
		
		plugin_handle = LoadLibrary(path);
		if(plugin_handle == NULL) {
			_DebugPrint(debugger, E_FILE_OPEN_FAILURE, path);
			return E_FAIL;
		}

		createRenderDevice = (CREATERENDERDEVICE)GetProcAddress(plugin_handle, "CreateRenderDevice");
		if(createRenderDevice == NULL) {
			_DebugPrint(debugger, E_RENDER_PLUGIN_LOAD_FAILURE, path);
			return E_FAIL;
		}

		hr = createRenderDevice(plugin_handle, &render_device, debugger);
		if(FAILED(hr)) {
			_DebugPrint(debugger, E_RENDER_DEVICE_CREATE_FAILURE, path);
			return E_FAIL;
		}

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