//
// File: libDirect3D9.cc
// =====================
//

#include <src/plugins/direct3d9/PluginDirect3D9.h>

#include <src/renderer/RenderWindow.h>
#include <src/utils/DebugManager.h>
#include <src/utils/OptionTree.h>

namespace Identi3D
{

	PluginDirect3D9::PluginDirect3D9(HMODULE plugin, DebugManager *debugger)
	{
		this->_render_target = NULL;
		this->_plugin_handle = plugin;
		this->_debugger = debugger;

		_is_running = false;
		_is_scene_running = false;

		this->_direct3d = NULL;
		this->_direct_device = NULL;
		this->_clear_color = RGB(0, 0, 0);
		memset(&(this->_present_parameters), 0, sizeof(this->_present_parameters));
	}

	PluginDirect3D9::~PluginDirect3D9(void)
	{
		release();
	}

	HRESULT PluginDirect3D9::init(RenderWindow *target, OptionTree *option)
	{
		_settings.read(option);

		_direct3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(_direct3d == NULL){
			_DebugPrint(_debugger, E_DIRECT3D_INIT_FAILURE);
			//_debugger->print(__FILE__, __LINE__, E_DIRECT3D_INIT_FAILURE);
			return E_FAIL;
		}
		_render_target = target;

		if(!checkPrerequisite()) return E_FAIL;
		return run();
	}

	bool PluginDirect3D9::checkPrerequisite(void)
	{
		D3DDISPLAYMODE mode;
		UINT i, count = 0;
		HRESULT hr;

		count = _direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
		if(0 == count){
			_DebugPrint(_debugger, E_NO_DISPLAY_MODE_AVAILABLE);
			return false;
		}

		_DebugPrintV(_debugger, I_BEGIN_ENUMERATE_DISPLAY_MODE, count);

		for(i = 0; i < count; i++){
			hr = _direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &mode);
			if(FAILED(hr)){
				_DebugPrint(_debugger, E_ENUMERATE_DISPLAY_MODE_FAILURE);
				return false;
			}
			_DebugPrintV(_debugger, I_DISPLAY_MODE_FORMAT, mode.Width, mode.Height, mode.RefreshRate);
			if(_settings._screen_width == mode.Width && 
				_settings._screen_height == mode.Height &&
				_settings._refresh_rate == mode.RefreshRate &&
				mode.Format == D3DFMT_X8R8G8B8){
					_DebugPrint(_debugger, I_DISPLAY_MODE_FOUND);
					break;
			}
		}
		if(i >= count){
			_DebugPrint(_debugger, E_NO_SUITABLE_DISPLAY_MODE);
			return false;
		}

		hr = _direct3d->CheckDeviceType(D3DADAPTER_DEFAULT, 
			(_settings._is_hardware_accelerated ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, _settings._is_windowed);
		if(FAILED(hr)){
			_DebugPrint(_debugger, E_NOT_SUPPORTED_MODE);
			return false;
		}

		return true;
	}

	HRESULT PluginDirect3D9::run(void)
	{
		HRESULT hr;
		DWORD flag;
		D3DCAPS9 caps;

		hr = _direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT,
			(_settings._is_hardware_accelerated ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			&caps);
		if(FAILED(hr)) {
			_DebugPrint(_debugger, E_GET_DEVICE_CAPABILITIES_FAILURE);
			return E_FAIL;
		}
		
		flag = 0;
		if(caps.VertexProcessingCaps) {
			flag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		} else {
			flag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
		
		memset(&_present_parameters, 0, sizeof(_present_parameters));
		_present_parameters.hDeviceWindow = _render_target->getHandle();
		_present_parameters.Windowed = (_settings._is_windowed) ? TRUE : FALSE;
		_present_parameters.BackBufferCount = 1;
		_present_parameters.BackBufferFormat = D3DFMT_X8R8G8B8;		// Temporarily set to D3DFMT_X8R8G8B8
		_present_parameters.EnableAutoDepthStencil = TRUE;
		_present_parameters.AutoDepthStencilFormat = D3DFMT_D24S8;	// Temporarily set to D3DFMT_D24S8
		_present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

		if(!_settings._is_windowed) {
			_present_parameters.FullScreen_RefreshRateInHz = _settings._refresh_rate;
			_present_parameters.BackBufferWidth = _settings._screen_width;
			_present_parameters.BackBufferHeight = _settings._screen_height;
			ShowCursor(FALSE);
		} else {
			_present_parameters.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
			_present_parameters.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
		}

		// Enable FSAA ( TODO: Replace it by reading configuration. )
		hr = _direct3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
			(_settings._is_hardware_accelerated ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			D3DFMT_X8R8G8B8,
			(_settings._is_windowed ? TRUE : FALSE),
			D3DMULTISAMPLE_2_SAMPLES,
			NULL);
		if(FAILED(hr)) {
			_DebugPrint(_debugger, E_FSAA_NOT_AVAILABLE);
			_present_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
		} else {
			_present_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		}

		hr = _direct3d->CreateDevice(D3DADAPTER_DEFAULT,
			(_settings._is_hardware_accelerated ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			_render_target->getHandle(),
			flag,
			&_present_parameters,
			&_direct_device);
		if(FAILED(hr)) {
			_DebugPrint(_debugger, E_CREATE_DIRECT3D_DEVICE_FAILURE);
			return E_FAIL;
		}

		_is_running = true;
		_is_scene_running = false;
		return S_OK;
	}

	void PluginDirect3D9::release(void)
	{
		if(_direct_device != NULL)
			_direct_device->Release();
		if(_direct3d != NULL)
			_direct3d->Release();
	}

	bool PluginDirect3D9::isRunning(void)
	{
		return _is_running;
	}

	HRESULT PluginDirect3D9::startRendering(bool clear_pixel, bool clear_depth, bool clear_stencil)
	{
		DWORD clear_flag = 0;
		HRESULT hr;

		if(clear_pixel) clear_flag |= D3DCLEAR_TARGET;
		if(clear_depth) clear_flag |= D3DCLEAR_ZBUFFER;
		if(clear_stencil && _settings._is_stencil_buffer_enabled) clear_flag |= D3DCLEAR_STENCIL;

		hr = _direct_device->Clear(0, NULL, clear_flag, _clear_color, 1.0f, 0);
		if(FAILED(hr)) {
			_DebugPrint(_debugger, E_CLEAR_BUFFER_FAILURE);
			return E_FAIL;
		}

		hr = _direct_device->BeginScene();
		if(FAILED(hr)) {
			_DebugPrint(_debugger, E_BEGIN_SCENE_FAILURE);
			return E_FAIL;
		}

		_is_scene_running = true;
		return S_OK;
	}

	void PluginDirect3D9::endRendering(void)
	{
		if(!_is_scene_running) return ;
		_direct_device->EndScene();
		_direct_device->Present(NULL, NULL, NULL, NULL);
		_is_scene_running = false;
	}

	HRESULT PluginDirect3D9::clear(bool clear_pixel, bool clear_depth, bool clear_stencil)
	{
		DWORD clear_flag = 0;
		HRESULT hr;

		if(clear_pixel) clear_flag |= D3DCLEAR_TARGET;
		if(clear_depth) clear_flag |= D3DCLEAR_ZBUFFER;
		if(clear_stencil && _settings._is_stencil_buffer_enabled) clear_flag |= D3DCLEAR_STENCIL;

		if(_is_scene_running) {
			hr = _direct_device->EndScene();
			if(FAILED(hr)) {
				_DebugPrint(_debugger, E_END_SCENE_FAILURE);
				return E_FAIL;
			}
		}

		hr = _direct_device->Clear(0, NULL, clear_flag, _clear_color, 1.0f, 0);
		if(FAILED(hr)) {
			_DebugPrint(_debugger, E_CLEAR_BUFFER_FAILURE);
			return E_FAIL;
		}

		if(_is_scene_running) {
			hr = _direct_device->BeginScene();
			if(FAILED(hr)) {
				_DebugPrint(_debugger, E_BEGIN_SCENE_FAILURE);
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
		_clear_color = static_cast<D3DCOLOR>((0xFF << 24) + (r << 16) + (g << 8) + b);
		_DebugPrintV(_debugger, __T("Clear color set to (%d, %d, %d)."), r, g, b);
	}

};