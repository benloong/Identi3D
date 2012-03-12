//
// File: Settings.h
// ================
// Direct 3D 9 settings management.
//

#ifndef CORN3D_SRC_PLUGINS_DIRECT3D9_SETTINGS_H
#define CORN3D_SRC_PLUGINS_DIRECT3D9_SETTINGS_H

#include <src/corn3d/General.h>

namespace Corn3D
{
	
	class Direct3D9SettingsManager
	{
	private:
		DebugManager *_debugger;

	private:
		bool convStringToBool(const TCHAR *str);
		const TCHAR *convBoolToString(bool value);

	public:
		bool _is_windowed;
		bool _is_hardware_accelerated;
		bool _is_stencil_buffer_enabled;
		UINT _screen_width;
		UINT _screen_height;
		UINT _refresh_rate;
		UINT _color_depth;

	public:
		
		Direct3D9SettingsManager(void);
		~Direct3D9SettingsManager(void) {} ;

		/*
		 * Reset and load default values.
		 */
		void reset(void);

		/*
		 * Read configuration from OptionTree.
		 */
		HRESULT read(OptionTree *tree);

		/*
		 * Write configuration to OptionTree.
		 */
		HRESULT write(OptionTree *tree);

		/*
		 * Set a debug manager.
		 */
		void setDebugManager(DebugManager *new_debugger = NULL) { _debugger = new_debugger; }
	};

};

#endif // CORN3D_SRC_PLUGINS_DIRECT3D9_SETTINGS_H
