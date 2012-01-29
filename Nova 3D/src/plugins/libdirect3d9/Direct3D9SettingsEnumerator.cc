//
// File: Direct3D9SettingsEnumerator.cc
// ====================================
//

#include <src/plugins/libdirect3d9/libDirect3D9.h>

namespace Nova3D
{

	inline bool EqualString(const TCHAR *str1, const TCHAR *str2)
	{
		if(_tcscmp(str1, str2) == 0) return true;
		return false;
	}

	Direct3D9SettingsEnumerator::Direct3D9SettingsEnumerator(void)
	{
		setDefaultValue();
		need_update = false;
	}

	Direct3D9SettingsEnumerator::~Direct3D9SettingsEnumerator(void)
	{
	}

	void Direct3D9SettingsEnumerator::setDefaultValue(void)
	{
		width = 800;
		height = 600;
		refresh_rate = 60;
		windowed = true;
		hardware_accel = true;
	}

	void Direct3D9SettingsEnumerator::processEntries(const TCHAR *name, TCHAR *value, UINT buffer_size)
	{
		if(need_update) {
			if(EqualString(name, __T("Width"))) {
				_sntprintf_s(value, buffer_size, buffer_size, __T("%d"), width);
			}else if(EqualString(name, __T("Height"))) {
				_sntprintf_s(value, buffer_size, buffer_size, __T("%d"), height);
			}else if(EqualString(name, __T("RefreshRate"))) {
				_sntprintf_s(value, buffer_size, buffer_size, __T("%d"), refresh_rate);
			}else if(EqualString(name, __T("Windowed"))) {
				_sntprintf_s(value, buffer_size, buffer_size, __T("%s"), (windowed ? __T("true") : __T("false")));
			}else if(EqualString(name, __T("HardwareAcceleration"))) {
				_sntprintf_s(value, buffer_size, buffer_size, __T("%s"),
					(hardware_accel ? __T("true") : __T("false")));
			}
			need_update = false;
		} else {
			if(EqualString(name, __T("Width"))) {
				width = static_cast<UINT>(_tstoi(value));
			}else if(EqualString(name, __T("Height"))) {
				height = static_cast<UINT>(_tstoi(value));
			}else if(EqualString(name, __T("RefreshRate"))) {
				refresh_rate = static_cast<UINT>(_tstoi(value));
			}else if(EqualString(name, __T("Windowed"))) {
				if(*value == __T('t')) windowed = true;
				else windowed = false;
			}else if(EqualString(name, __T("HardwareAcceleration"))) {
				if(*value == __T('t')) hardware_accel = true;
				else hardware_accel = false;
			}
		}
	}

	void Direct3D9SettingsEnumerator::setResolution(UINT new_width, UINT new_height, UINT new_refresh_rate)
	{
		width = new_width;
		height = new_height;
		refresh_rate = new_refresh_rate;
	}

};
