//
// File: Settings.cc
// =================
//

#include <src/plugins/direct3d9/Settings.h>

#include <src/utils/OptionTree.h>
#include <src/utils/DebugManager.h>

#define OPTION_WINDOWED		0
#define OPTION_HAL			1
#define OPTION_STENCIL		2
#define OPTION_RESOLUTION	3

namespace Identi3D
{

	ULONG item_hash[4];
	const int max_hash_item = 4;

	bool Direct3D9SettingsManager::convStringToBool(const TCHAR *str)
	{
		if(_tcscmp(str, __T("true")) == 0) return true;
		return false;
	}

	const TCHAR *Direct3D9SettingsManager::convBoolToString(bool value)
	{
		if(value) return __T("true");
		return __T("false");
	}

	Direct3D9SettingsManager::Direct3D9SettingsManager(void) : _debugger(NULL)
	{
		item_hash[0] = OptionTree::hashString(__T("Windowed"));
		item_hash[1] = OptionTree::hashString(__T("HardwareAcceleration"));
		item_hash[2] = OptionTree::hashString(__T("StencilBuffer"));
		item_hash[3] = OptionTree::hashString(__T("ScreenResolution"));
		reset();
	}

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
		if(tree == NULL) {
			_DebugPrint(_debugger, E_INVALID_PARAMETERS);
			return E_FAIL;
		}
		OptionIterator iter(tree->getElement(__T("Graphics"))->child);

		while(!iter.end()) {
			for(int i = 0; i < max_hash_item; i++) {
				if(item_hash[i] == (*iter).hash) {
					switch(i)
					{
					case OPTION_WINDOWED: 
						_is_windowed = convStringToBool((*iter).value);
						break;
					case OPTION_HAL:
						_is_hardware_accelerated = convStringToBool((*iter).value);
						break;
					case OPTION_STENCIL:
						_is_stencil_buffer_enabled = convStringToBool((*iter).value);
						break;
					case OPTION_RESOLUTION:
						_stscanf_s((*iter).value, __T("%dx%dx%d@%d"), 
							&_screen_width, &_screen_height, &_color_depth, &_refresh_rate);
						break;
					}
				}
			}
			++iter;
		}
		return S_OK;
	}

	HRESULT Direct3D9SettingsManager::write(OptionTree *tree)
	{
		OptionElement *p;
		TCHAR tmpstr[256];
		long s;

		p = tree->getElement(__T("Graphics"));
		if(p == NULL) {
			p = tree->addElement(NULL, __T("Graphics"));
			if(p == NULL) {
				_DebugPrint(_debugger, E_D3D9_OPTION_GROUP_CREATE_FAILED, __T("Graphics"));
				return E_FAIL;
			}
		}
		_stprintf_s(tmpstr, __T("%dx%dx%d@%d"), 
			_screen_width, _screen_height, _color_depth, _refresh_rate);
		s  = (long)tree->addElement(p, __T("Windowed"), 
			convBoolToString(_is_windowed));
		s &= (long)tree->addElement(p, __T("HardwareAcceleration"), 
			convBoolToString(_is_hardware_accelerated));
		s &= (long)tree->addElement(p, __T("StencilBuffer"), 
			convBoolToString(_is_stencil_buffer_enabled));
		s &= (long)tree->addElement(p, __T("ScreenResolution"), tmpstr);

		if(s == NULL) {
			_DebugPrint(_debugger, E_D3D9_OPTION_WRITE_FAILED);
			return E_FAIL;
		}
		return S_OK;
	}

};
