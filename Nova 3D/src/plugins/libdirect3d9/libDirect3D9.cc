//
// File: libDirect3D9.cc
// =====================
//

#include "src/plugins/libdirect3d9/libDirect3D9.h"

#include "src/utils/DebugManager.h"

namespace Nova3D
{

	PluginDirect3D9::PluginDirect3D9(HMODULE plugin, DebugManager *debugmgr)
	{
		render_window = NULL;
		plugin_handle = plugin;
		debug_manager = debugmgr;
		is_running = false;

		direct3d = NULL;
		direct_device = NULL;
		clear_color = RGB(0, 0, 0);
		memset(&present_parameters, 0, sizeof(present_parameters));
	}

	PluginDirect3D9::~PluginDirect3D9(void)
	{
		release();
	}

	HRESULT PluginDirect3D9::init(HWND window, UINT min_depth, UINT min_stencil)
	{
		if(min_stencil > 0) is_stencil_enabled = true;

		direct3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(direct3d == NULL){
			_DEBUGPRINT(debug_manager, E_DIRECT3D_INIT_FAILURE);
			return E_FAIL;
		}
		render_window = window;

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
			_DEBUGPRINT(debug_manager, E_NO_DISPLAY_MODE_AVAILABLE);
			return false;
		}

		_DEBUGPRINT(debug_manager, I_BEGIN_ENUMERATE_DISPLAY_MODE, count);

		for(i = 0; i < count; i++){
			hr = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &mode);
			if(FAILED(hr)){
				_DEBUGPRINT(debug_manager, E_ENUMERATE_DISPLAY_MODE_FAILURE);
				return false;
			}
			_DEBUGPRINT(debug_manager, I_DISPLAY_MODE_FORMAT, mode.Width, mode.Height, mode.RefreshRate);
			if(settings_enumerator.getWidth() == mode.Width && 
				settings_enumerator.getHeight() == mode.Height &&
				settings_enumerator.getRefreshRate() == mode.RefreshRate &&
				mode.Format == D3DFMT_X8R8G8B8){
					_DEBUGPRINT(debug_manager, I_DISPLAY_MODE_FOUND);
					break;
			}
		}
		if(i >= count){
			_DEBUGPRINT(debug_manager, E_NO_SUITABLE_DISPLAY_MODE);
			return false;
		}

		hr = direct3d->CheckDeviceType(D3DADAPTER_DEFAULT, 
			(settings_enumerator.isHardwareAccelerated() ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, settings_enumerator.isWindowed());
		if(FAILED(hr)){
			_DEBUGPRINT(debug_manager, E_NOT_SUPPORTED_MODE);
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
			_DEBUGPRINT(debug_manager, E_GET_DEVICE_CAPABILITIES_FAILURE);
			return E_FAIL;
		}
		
		flag = 0;
		if(caps.VertexProcessingCaps) {
			flag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		} else {
			flag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		memset(&present_parameters, 0, sizeof(present_parameters));
		present_parameters.Windowed = settings_enumerator.isWindowed();
		present_parameters.BackBufferWidth = settings_enumerator.getWidth();
		present_parameters.BackBufferHeight = settings_enumerator.getHeight();
		present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		if(!settings_enumerator.isWindowed()) {
			present_parameters.FullScreen_RefreshRateInHz = settings_enumerator.getRefreshRate();
		}
		present_parameters.BackBufferFormat = D3DFMT_X8R8G8B8;
		present_parameters.hDeviceWindow = render_window;

		// Enable FSAA ( TODO: Replace it by reading configuration. )
		hr = direct3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
			(settings_enumerator.isHardwareAccelerated() ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			D3DFMT_X8R8G8B8,
			(settings_enumerator.isWindowed() ? TRUE : FALSE),
			D3DMULTISAMPLE_2_SAMPLES,
			NULL);
		if(FAILED(hr)) {
			_DEBUGPRINT(debug_manager, E_FSAA_NOT_AVAILABLE);
			present_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
		} else {
			present_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		}

		hr = direct3d->CreateDevice(D3DADAPTER_DEFAULT,
			(settings_enumerator.isHardwareAccelerated() ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			render_window,
			flag,
			&present_parameters,
			&direct_device);
		if(FAILED(hr)) {
			_DEBUGPRINT(debug_manager, E_CREATE_DIRECT3D_DEVICE_FAILURE);
			return E_FAIL;
		}

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
		// TODO: add detailed code here.
		return S_OK;
	}

	void PluginDirect3D9::stopRendering(void)
	{
		// TODO: add detailed code here.
	}

	HRESULT PluginDirect3D9::clear(bool clear_pixel, bool clear_depth, bool clear_stencil)
	{
		// TODO: add detailed code here.
		return S_OK;
	}

	void PluginDirect3D9::setClearColor(float red, float green, float blue)
	{
		clear_color = (D3DCOLOR)((((int)red & 0xFF) << 24) + (((int)green & 0xFF) << 16) +
			((int)blue & 0xFF));
	}

	SettingsEnumerator &PluginDirect3D9::getSettingsEnumerator(void)
	{
		return settings_enumerator;
	}

};