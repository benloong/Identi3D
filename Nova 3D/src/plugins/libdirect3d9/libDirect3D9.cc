//
// File: libDirect3D9.cc
// =====================
//

#include "src/plugins/libdirect3d9/libDirect3D9.h"
#include "src/renderer/RenderTarget.h"
#include "src/utils/DebugManager.h"

namespace Nova3D
{

	PluginDirect3D9::PluginDirect3D9(HMODULE plugin, DebugManager *debug_manager)
	{
		this->render_target = NULL;
		this->plugin_handle = plugin;
		this->debug_manager = debug_manager;
		is_running = false;
		
		is_scene_running = false;
		is_stencil_enabled = false;
		this->direct3d = NULL;
		this->direct_device = NULL;
		this->clear_color = RGB(0, 0, 0);
		memset(&(this->present_parameters), 0, sizeof(this->present_parameters));
	}

	PluginDirect3D9::~PluginDirect3D9(void)
	{
		release();
	}

	HRESULT PluginDirect3D9::init(RenderTarget *target, UINT min_depth, UINT min_stencil)
	{
		if(min_stencil > 0) is_stencil_enabled = true;

		direct3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(direct3d == NULL){
			_DebugPrintS(debug_manager, E_DIRECT3D_INIT_FAILURE);
			//debug_manager->print(__FILE__, __LINE__, E_DIRECT3D_INIT_FAILURE);
			return E_FAIL;
		}
		render_target = target;

		if(!checkPrerequisite()) return E_FAIL;
		return run();
	}

	bool PluginDirect3D9::checkPrerequisite(void)
	{
		D3DDISPLAYMODE mode;
		UINT i, count = 0;
		HRESULT hr;

		count = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
		if(0 == count){
			_DebugPrintS(debug_manager, E_NO_DISPLAY_MODE_AVAILABLE);
			return false;
		}

		_DebugPrintSV(debug_manager, I_BEGIN_ENUMERATE_DISPLAY_MODE, count);

		for(i = 0; i < count; i++){
			hr = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &mode);
			if(FAILED(hr)){
				_DebugPrintS(debug_manager, E_ENUMERATE_DISPLAY_MODE_FAILURE);
				return false;
			}
			_DebugPrintSV(debug_manager, I_DISPLAY_MODE_FORMAT, mode.Width, mode.Height, mode.RefreshRate);
			if(settings_enumerator.getWidth() == mode.Width && 
				settings_enumerator.getHeight() == mode.Height &&
				settings_enumerator.getRefreshRate() == mode.RefreshRate &&
				mode.Format == D3DFMT_X8R8G8B8){
					_DebugPrintS(debug_manager, I_DISPLAY_MODE_FOUND);
					break;
			}
		}
		if(i >= count){
			_DebugPrintS(debug_manager, E_NO_SUITABLE_DISPLAY_MODE);
			return false;
		}

		hr = direct3d->CheckDeviceType(D3DADAPTER_DEFAULT, 
			(settings_enumerator.isHardwareAccelerated() ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, settings_enumerator.isWindowed());
		if(FAILED(hr)){
			_DebugPrintS(debug_manager, E_NOT_SUPPORTED_MODE);
			return false;
		}

		return true;
	}

	HRESULT PluginDirect3D9::run(void)
	{
		HRESULT hr;
		DWORD flag;
		D3DCAPS9 caps;

		hr = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT,
			(settings_enumerator.isHardwareAccelerated() ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			&caps);
		if(FAILED(hr)) {
			_DebugPrintS(debug_manager, E_GET_DEVICE_CAPABILITIES_FAILURE);
			return E_FAIL;
		}
		
		flag = 0;
		if(caps.VertexProcessingCaps) {
			flag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		} else {
			flag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
		
		memset(&present_parameters, 0, sizeof(present_parameters));
		present_parameters.hDeviceWindow = render_target->getWindowHandle();
		present_parameters.Windowed = (settings_enumerator.isWindowed()) ? TRUE : FALSE;
		present_parameters.BackBufferCount = 1;
		present_parameters.BackBufferFormat = D3DFMT_X8R8G8B8;		// Temporarily set to D3DFMT_X8R8G8B8
		present_parameters.EnableAutoDepthStencil = TRUE;
		present_parameters.AutoDepthStencilFormat = D3DFMT_D24S8;	// Temporarily set to D3DFMT_D24S8
		present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

		if(!settings_enumerator.isWindowed()) {
			present_parameters.FullScreen_RefreshRateInHz = settings_enumerator.getRefreshRate();
			present_parameters.BackBufferWidth = settings_enumerator.getWidth();
			present_parameters.BackBufferHeight = settings_enumerator.getHeight();
			ShowCursor(FALSE);
		} else {
			present_parameters.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
			present_parameters.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
		}

		// Enable FSAA ( TODO: Replace it by reading configuration. )
		hr = direct3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
			(settings_enumerator.isHardwareAccelerated() ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			D3DFMT_X8R8G8B8,
			(settings_enumerator.isWindowed() ? TRUE : FALSE),
			D3DMULTISAMPLE_2_SAMPLES,
			NULL);
		if(FAILED(hr)) {
			_DebugPrintS(debug_manager, E_FSAA_NOT_AVAILABLE);
			present_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
		} else {
			present_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		}

		hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT,
			(settings_enumerator.isHardwareAccelerated() ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			render_target->getWindowHandle(),
			flag,
			&present_parameters,
			&direct_device);
		if(FAILED(hr)) {
			_DebugPrintS(debug_manager, E_CREATE_DIRECT3D_DEVICE_FAILURE);
			return E_FAIL;
		}

		is_running = true;
		is_scene_running = false;
		return S_OK;
	}

	void PluginDirect3D9::release(void)
	{
		if(direct_device != NULL)
			direct_device->Release();
		if(direct3d != NULL)
			direct3d->Release();
	}

	bool PluginDirect3D9::isRunning(void)
	{
		return is_running;
	}

	HRESULT PluginDirect3D9::startRendering(bool clear_pixel, bool clear_depth, bool clear_stencil)
	{
		DWORD clear_flag = 0;
		HRESULT hr;

		if(clear_pixel) clear_flag |= D3DCLEAR_TARGET;
		if(clear_depth) clear_flag |= D3DCLEAR_ZBUFFER;
		if(clear_stencil && is_stencil_enabled) clear_flag |= D3DCLEAR_STENCIL;

		hr = direct_device->Clear(0, NULL, clear_flag, clear_color, 1.0f, 0);
		if(FAILED(hr)) {
			_DebugPrintS(debug_manager, E_CLEAR_BUFFER_FAILURE);
			return E_FAIL;
		}

		hr = direct_device->BeginScene();
		if(FAILED(hr)) {
			_DebugPrintS(debug_manager, E_BEGIN_SCENE_FAILURE);
			return E_FAIL;
		}

		is_scene_running = true;
		return S_OK;
	}

	void PluginDirect3D9::endRendering(void)
	{
		if(!is_scene_running) return ;
		direct_device->EndScene();
		direct_device->Present(NULL, NULL, NULL, NULL);
		is_scene_running = false;
	}

	HRESULT PluginDirect3D9::clear(bool clear_pixel, bool clear_depth, bool clear_stencil)
	{
		DWORD clear_flag = 0;
		HRESULT hr;

		if(clear_pixel) clear_flag |= D3DCLEAR_TARGET;
		if(clear_depth) clear_flag |= D3DCLEAR_ZBUFFER;
		if(clear_stencil && is_stencil_enabled) clear_flag |= D3DCLEAR_STENCIL;

		if(is_scene_running) {
			hr = direct_device->EndScene();
			if(FAILED(hr)) {
				_DebugPrintS(debug_manager, E_END_SCENE_FAILURE);
				return E_FAIL;
			}
		}

		hr = direct_device->Clear(0, NULL, clear_flag, clear_color, 1.0f, 0);
		if(FAILED(hr)) {
			_DebugPrintS(debug_manager, E_CLEAR_BUFFER_FAILURE);
			return E_FAIL;
		}

		if(is_scene_running) {
			hr = direct_device->BeginScene();
			if(FAILED(hr)) {
				_DebugPrintS(debug_manager, E_BEGIN_SCENE_FAILURE);
				return E_FAIL;
			}
		}
		return S_OK;
	}

	void PluginDirect3D9::setClearColor(float red, float green, float blue)
	{
		BYTE r, g, b;

		r = static_cast<BYTE>(red * 255) & 0xFF;
		g = static_cast<BYTE>(green * 255) & 0xFF;
		b = static_cast<BYTE>(blue * 255) & 0xFF;
		clear_color = static_cast<D3DCOLOR>((0xFF << 24) + (r << 16) + (g << 8) + b);
		_DebugPrintSV(debug_manager, __T("Clear color set to (%d, %d, %d)."), r, g, b);
	}

	SettingsEnumerator &PluginDirect3D9::getSettingsEnumerator(void)
	{
		return settings_enumerator;
	}

};