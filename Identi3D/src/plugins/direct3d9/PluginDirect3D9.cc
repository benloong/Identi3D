//
// File: libDirect3D9.cc
// =====================
//

#include <src/plugins/direct3d9/PluginDirect3D9.h>
#include <src/identi3d/IdentiExceptions.h>
#include <src/renderer/RenderWindow.h>
#include <src/utils/DebugManager.h>
#include <src/utils/OptionTree.h>

namespace Identi3D
{

	PluginDirect3D9::PluginDirect3D9(HMODULE plugin, DebugManager *debugger)
		: DebugFrame(debugger)
	{
		this->_render_target = NULL;
		this->_plugin_handle = plugin;

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

	bool PluginDirect3D9::reloadConfig(OptionTree *option)
	{
		// Load configuration.
		if(option) return _settings.read(option);
		else _settings.reset();
		return true;
	}

	bool PluginDirect3D9::init(RenderWindow &target)
	{
		// Only one instance is allowed.
		if(_is_running) return true;

		// Create Direct3D9 object.
		_direct3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(_direct3d == NULL) {
			_printMessage(__FILE__, __LINE__, E_D3D9_INITIALIZE_FAILURE);
			MessageBoxA(NULL, E_D3D9_INITIALIZE_FAILURE, "Error", MB_OK);
			return false;
		}
		
		// Store render target handle.
		_render_target = &target;

		// Determine display format.
		D3DFORMAT format = (_settings._color_depth == 16) ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;

		// Check display device compatibility.
		if(!checkPrerequisite(format)) {
			_printMessage(__FILE__, __LINE__, W_D3D9_HARDWARE_NOT_COMPATIBLE);
			MessageBoxA(NULL, W_D3D9_HARDWARE_NOT_COMPATIBLE, "Warning", MB_OK);
			_settings.reset();
			if(!checkPrerequisite(format)) {
				_printMessage(__FILE__, __LINE__, E_D3D9_PREREQUISITE_NOT_SATISFIED);
				MessageBoxA(NULL, E_D3D9_PREREQUISITE_NOT_SATISFIED, "Error", MB_OK);
				_direct3d->Release();
				_direct3d = NULL;
				return false;
			} else {
				_settings.write();
			}
		}

		// Initialize device.
		if(!run(format)) {
			_printMessage(__FILE__, __LINE__, E_D3D9_INITIALIZE_FAILURE);
			MessageBoxA(NULL, E_D3D9_INITIALIZE_FAILURE, "Error", MB_OK);
			_direct3d->Release();
			_direct3d = NULL;
			return false;
		}

		return true;
	}

	bool PluginDirect3D9::checkPrerequisite(D3DFORMAT format)
	{
		D3DDISPLAYMODE mode;
		UINT i, count = 0;
		HRESULT hr;

		count = _direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, format);
		if(0 == count){
			_printMessage(__FILE__, __LINE__, E_NO_DISPLAY_MODE_AVAILABLE);
			return false;
		}

		_printVerboseMessage(__FILE__, __LINE__, I_BEGIN_ENUMERATE_DISPLAY_MODE, count);

		for(i = 0; i < count; i++){
			hr = _direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, format, i, &mode);
			if(FAILED(hr)){
				_printMessage(__FILE__, __LINE__, E_ENUMERATE_DISPLAY_MODE_FAILURE);
				return false;
			}
			_printVerboseMessage(__FILE__, __LINE__, I_DISPLAY_MODE_FORMAT, mode.Width, mode.Height, mode.RefreshRate);
			if(_settings._screen_width == mode.Width && 
				_settings._screen_height == mode.Height &&
				_settings._refresh_rate == mode.RefreshRate &&
				mode.Format == format){
					_printVerboseMessage(__FILE__, __LINE__, I_DISPLAY_MODE_FOUND);
					break;
			}
		}
		if(i >= count){
			_printMessage(__FILE__, __LINE__, E_NO_SUITABLE_DISPLAY_MODE);
			return false;
		}

		hr = _direct3d->CheckDeviceType(D3DADAPTER_DEFAULT, 
			(_settings._is_hardware_accelerated ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			format, format, _settings._is_windowed);
		if(FAILED(hr)){
			_printMessage(__FILE__, __LINE__, E_NOT_SUPPORTED_MODE);
			return false;
		}

		return true;
	}

	bool PluginDirect3D9::run(D3DFORMAT format)
	{
		HRESULT hr;
		DWORD flag;
		D3DCAPS9 caps;

		// Get device capabilities
		hr = _direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT,
			(_settings._is_hardware_accelerated ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			&caps);
		if(FAILED(hr)) {
			_printMessage(__FILE__, __LINE__, E_GET_DEVICE_CAPABILITIES_FAILURE);
			return false;
		}
		
		// Set hardware acceleration.
		flag = 0;
		if(caps.VertexProcessingCaps) {
			flag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		} else {
			flag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
		
		// Set present parameters.
		memset(&_present_parameters, 0, sizeof(_present_parameters));
		_present_parameters.hDeviceWindow = _render_target->getHandle();
		_present_parameters.Windowed = (_settings._is_windowed) ? TRUE : FALSE;
		_present_parameters.BackBufferCount = 1;
		_present_parameters.BackBufferFormat = format;
		_present_parameters.EnableAutoDepthStencil = TRUE;
		_present_parameters.AutoDepthStencilFormat = D3DFMT_D24S8;	// TODO: Temporarily set to D3DFMT_D24S8
		_present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

		// Set screen resolution.
		_present_parameters.BackBufferWidth = _settings._screen_width;
		_present_parameters.BackBufferHeight = _settings._screen_height;
		if(!_settings._is_windowed) {
			_present_parameters.FullScreen_RefreshRateInHz = _settings._refresh_rate;
			ShowCursor(FALSE);
		}

		// Enable FSAA ( TODO: Replace it by reading configuration. )
		hr = _direct3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
			(_settings._is_hardware_accelerated ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF),
			format, (_settings._is_windowed ? TRUE : FALSE),
			D3DMULTISAMPLE_2_SAMPLES,
			NULL);
		if(FAILED(hr)) {
			_printMessage(__FILE__, __LINE__, W_FSAA_NOT_AVAILABLE);
			_present_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
		} else {
			_present_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		}

		// Create device.
		hr = _direct3d->CreateDevice(D3DADAPTER_DEFAULT,
			(_settings._is_hardware_accelerated ? D3DDEVTYPE_HAL : D3DDEVTYPE_SW),
			_render_target->getHandle(), flag,
			&_present_parameters,
			&_direct_device);
		if(FAILED(hr)) {
			_printMessage(__FILE__, __LINE__, E_CREATE_DIRECT3D_DEVICE_FAILURE);
			_direct_device = NULL;
			return false;
		}

		_is_running = true;
		_is_scene_running = false;
		return true;
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

	bool PluginDirect3D9::startRendering(bool clear_pixel, bool clear_depth, bool clear_stencil)
	{
		DWORD clear_flag = 0;
		HRESULT hr;

		if(clear_pixel) clear_flag |= D3DCLEAR_TARGET;
		if(clear_depth) clear_flag |= D3DCLEAR_ZBUFFER;
		if(clear_stencil && _settings._is_stencil_buffer_enabled) clear_flag |= D3DCLEAR_STENCIL;

		hr = _direct_device->Clear(0, NULL, clear_flag, _clear_color, 1.0f, 0);
		if(FAILED(hr)) return false;

		hr = _direct_device->BeginScene();
		if(FAILED(hr)) return false;

		_is_scene_running = true;
		return true;
	}

	void PluginDirect3D9::endRendering(void)
	{
		if(!_is_scene_running) return ;
		_is_scene_running = false;
		_direct_device->EndScene();
		_direct_device->Present(NULL, NULL, NULL, NULL);
	}

	bool PluginDirect3D9::clear(bool clear_pixel, bool clear_depth, bool clear_stencil)
	{
		DWORD clear_flag = 0;
		HRESULT hr;

		if(clear_pixel) clear_flag |= D3DCLEAR_TARGET;
		if(clear_depth) clear_flag |= D3DCLEAR_ZBUFFER;
		if(clear_stencil && _settings._is_stencil_buffer_enabled) clear_flag |= D3DCLEAR_STENCIL;

		if(_is_scene_running) {
			_direct_device->EndScene();
		}

		hr = _direct_device->Clear(0, NULL, clear_flag, _clear_color, 1.0f, 0);
		if(FAILED(hr)) {
			if(_is_scene_running) _is_scene_running = false;
			return false;
		}

		if(_is_scene_running) {
			hr = _direct_device->BeginScene();
			if(FAILED(hr)) {
				_is_scene_running = false;
			}
		}

		return true;
	}

	void PluginDirect3D9::setClearColor(float red, float green, float blue)
	{
		BYTE r, g, b;

		r = static_cast<BYTE>(red * 255) & 0xFF;
		g = static_cast<BYTE>(green * 255) & 0xFF;
		b = static_cast<BYTE>(blue * 255) & 0xFF;
		_clear_color = static_cast<D3DCOLOR>((0xFF << 24) + (r << 16) + (g << 8) + b);
	}

};