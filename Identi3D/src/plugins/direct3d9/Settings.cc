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

	Direct3D9SettingsManager::Direct3D9SettingsManager(DebugManager *debugger = NULL) 
		: DebugFrame(debugger), _tree(NULL)
	{
		reset();
	}

	void Direct3D9SettingsManager::reset()
	{
		_is_windowed				= false;
		_is_hardware_accelerated	= true;
		_is_stencil_buffer_enabled	= true;

		_screen_width	= 800;
		_screen_height	= 600;
		_color_depth	= 32;
		_refresh_rate	= 60;
	}

	bool Direct3D9SettingsManager::read(OptionTree *tree)
	{
		try
		{
			reset();

			if(tree == NULL) return false;

			_is_windowed = convStringToBool(tree->getValue(std::wstring(L"Graphics.Windowed")));
			_is_hardware_accelerated = convStringToBool(tree->getValue(std::wstring(L"Graphics.HardwareAcceleration")));
			_is_stencil_buffer_enabled = convStringToBool(tree->getValue(std::wstring(L"Graphics.StencilBuffer")));
			swscanf_s(tree->getValue(std::wstring(L"Graphics.ScreenResolution")).c_str(), L"%dx%dx%d@%d",
				&_screen_width, &_screen_height, &_color_depth, &_refresh_rate);
		} catch(std::exception &e) {
			reset();
			_tree = NULL;
			if(_debugger) _debugger->print(__FILE__, __LINE__, e);
			return false;
		}
		
		_tree = tree;
		return true;
	}

	bool Direct3D9SettingsManager::write(void)
	{
		if(_tree == NULL) return false;

		try
		{
			wchar_t tmpstr[64];

			// Create screen resolution string.
			swprintf_s(tmpstr, TEXT("%dx%dx%d@%d"),
				_screen_width, _screen_height, _color_depth, _refresh_rate);

			_tree->addElement(std::wstring(L"Graphics.Windowed"),
				(_is_windowed) ? VALUE_TRUE : VALUE_FALSE);
			_tree->addElement(std::wstring(L"Graphics.HardwareAcceleration"),
				(_is_hardware_accelerated) ? VALUE_TRUE : VALUE_FALSE);
			_tree->addElement(std::wstring(L"Graphics.StencilBuffer"),
				(_is_stencil_buffer_enabled) ? VALUE_TRUE : VALUE_FALSE);
			_tree->addElement(std::wstring(L"Graphics.ScreenResolution"),
				std::wstring(tmpstr));
		} catch(std::exception &e) {
			if(_debugger) _debugger->print(__FILE__, __LINE__, e);
			return false;
		}

		return true;
	}

};
