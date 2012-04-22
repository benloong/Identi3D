//
// File: Settings.cc
// =================
//

#include <src/plugins/direct3d9/Settings.h>

#include <src/utils/OptionTree.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{

	const std::wstring VALUE_TRUE = L"true";
	const std::wstring VALUE_FALSE = L"false";
	
	inline bool convStringToBool(const std::wstring &str)
	{
		return (str == VALUE_TRUE);
	}

	void Direct3D9SettingsManager::reset()
	{
		_is_windowed				= D3DSETTINGS_DEFAULT_WINDOWED;
		_is_hardware_accelerated	= D3DSETTINGS_DEFAULT_HARDWAREACCELERATED;
		_is_stencil_buffer_enabled	= D3DSETTINGS_DEFAULT_STENCILBUFFER;

		_screen_width	= D3DSETTINGS_DEFAULT_SCREEN_WIDTH;
		_screen_height	= D3DSETTINGS_DEFAULT_SCREEN_HEIGHT;
		_color_depth	= D3DSETTINGS_DEFAULT_COLOR_DEPTH;
		_refresh_rate	= D3DSETTINGS_DEFAULT_REFRESH_RATE;
	}

	bool Direct3D9SettingsManager::read(OptionTree *tree)
	{
		if(tree == NULL) return false;

		std::wstring tmp;

		tmp = tree->getValue(std::wstring(L"Graphics.Windowed"));
		if(tmp.length()) _is_windowed = convStringToBool(tmp);
		else _is_windowed = D3DSETTINGS_DEFAULT_WINDOWED;

		tmp = tree->getValue(std::wstring(L"Graphics.HardwareAcceleration"));
		if(tmp.length()) _is_hardware_accelerated = convStringToBool(tmp);
		else _is_hardware_accelerated = D3DSETTINGS_DEFAULT_HARDWAREACCELERATED;

		tmp = tree->getValue(std::wstring(L"Graphics.StencilBuffer"));
		if(tmp.length()) _is_stencil_buffer_enabled = convStringToBool(tmp);
		else _is_stencil_buffer_enabled = D3DSETTINGS_DEFAULT_STENCILBUFFER;

		if(swscanf_s(tree->getValue(std::wstring(L"Graphics.ScreenResolution")).c_str(), L"%dx%dx%d@%d",
			&_screen_width, &_screen_height, &_color_depth, &_refresh_rate) == 0) {
				_screen_width	= D3DSETTINGS_DEFAULT_SCREEN_WIDTH;
				_screen_height	= D3DSETTINGS_DEFAULT_SCREEN_HEIGHT;
				_color_depth	= D3DSETTINGS_DEFAULT_COLOR_DEPTH;
				_refresh_rate	= D3DSETTINGS_DEFAULT_REFRESH_RATE;
		}

		if(_color_depth != 16 && _color_depth != 32) 
			_color_depth = D3DSETTINGS_DEFAULT_COLOR_DEPTH;

		_tree = tree;
		return true;
	}

	bool Direct3D9SettingsManager::write(void)
	{
		if(_tree == NULL) return false;

		// Create screen resolution string.
		wchar_t tmpstr[64];
		swprintf_s(tmpstr, TEXT("%dx%dx%d@%d"),
			_screen_width, _screen_height, _color_depth, _refresh_rate);

		if(NULL == _tree->addElement(std::wstring(L"Graphics.Windowed"),
			(_is_windowed) ? VALUE_TRUE : VALUE_FALSE)) return false;
		if(NULL == _tree->addElement(std::wstring(L"Graphics.HardwareAcceleration"),
			(_is_hardware_accelerated) ? VALUE_TRUE : VALUE_FALSE)) return false;
		if(NULL == _tree->addElement(std::wstring(L"Graphics.StencilBuffer"),
			(_is_stencil_buffer_enabled) ? VALUE_TRUE : VALUE_FALSE)) return false;
		if(NULL == _tree->addElement(std::wstring(L"Graphics.ScreenResolution"),
			std::wstring(tmpstr))) return false;

		return true;
	}

};
