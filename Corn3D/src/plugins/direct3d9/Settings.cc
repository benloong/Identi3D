//
// File: Settings.cc
// =================
//

#include <src/plugins/direct3d9/Settings.h>
#include <src/utils/OptionTree.h>

namespace Corn3D
{

	void Direct3D9SettingsManager::reset()
	{
		_is_windowed = false;
		_is_hardware_accelerated = true;
		_is_stencil_buffer_enabled = true;

		_screen_width = 800;
		_screen_height = 600;
		_refresh_rate = 60;
	}

	HRESULT Direct3D9SettingsManager::read(OptionTree *tree)
	{
		void *ptr;
		HRESULT hr;

		if(tree == NULL) return E_FAIL;
		hr = tree->getKey(__T("Graphics.Windowed"), OptionType_Bool, &ptr);
		if(SUCCEEDED(hr)) _is_windowed = *((bool *)ptr);
		hr = tree->getKey(__T("Graphics.HardwareAcceleration"), OptionType_Bool, &ptr);
		if(SUCCEEDED(hr)) _is_hardware_accelerated = *((bool *)ptr);
		hr = tree->getKey(__T("Graphics.StencilBuffer"), OptionType_Bool, &ptr);
		if(SUCCEEDED(hr)) _is_stencil_buffer_enabled = *((bool *)ptr);

		hr = tree->getKey(__T("Graphics.ScreenResolution"), OptionType_String, &ptr);
		if(SUCCEEDED(hr)) {
			_stscanf_s((TCHAR *)ptr, __T("%dx%dx%d@%d"), 
				&_screen_width, &_screen_height, &_color_depth, &_refresh_rate);
		}

		return S_OK;
	}

	HRESULT Direct3D9SettingsManager::write(OptionTree *tree)
	{
		HRESULT hr;
		OptionElement *p, *s;
		TCHAR tmp[32];

		hr = tree->getSpecifiedElement(__T("Graphics"), &p);
		if(FAILED(hr)) return E_FAIL;

		hr = tree->setKey(__T("Graphics.Windowed"), 
			tree->BoolToString(_is_windowed));
		if(FAILED(hr)) {
			hr = tree->addElement(p, __T("Windowed"), OptionType_Bool, &s);
			if(SUCCEEDED(hr)) *((bool *)s->value) = _is_windowed;
		}

		hr = tree->setKey(__T("Graphics.HardwareAcceleration"),
			tree->BoolToString(_is_hardware_accelerated));
		if(FAILED(hr)) {
			hr = tree->addElement(p, __T("HardwareAcceleration"), OptionType_Bool, &s);
			if(SUCCEEDED(hr)) *((bool *)s->value) = _is_hardware_accelerated;
		}

		hr = tree->setKey(__T("Graphics.StencilBuffer"),
			tree->BoolToString(_is_stencil_buffer_enabled));
		if(FAILED(hr)) {
			hr = tree->addElement(p, __T("StencilBuffer"), OptionType_Bool, &s);
			if(SUCCEEDED(hr)) *((bool *)s->value) = _is_stencil_buffer_enabled;
		}

		_sntprintf_s(tmp, 32, __T("%dx%dx%d@%d"), 
			_screen_width, _screen_height, _color_depth, _refresh_rate);
		hr = tree->setKey(__T("Graphics.ScreenResolution"), tmp);
		if(FAILED(hr)) {
			hr = tree->addElement(p, __T("ScreenResoultion"), OptionType_Bool, &s);
			if(SUCCEEDED(hr)) {
				s->value = new TCHAR[32];
				_tcscpy_s((TCHAR *)s->value, 32, tmp);
			}
		}

		return S_OK;
	}

};
