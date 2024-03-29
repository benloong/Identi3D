//
// File: Settings.h
// ================
// Direct 3D 9 settings management.
//

#ifndef IDENTI3D_SRC_PLUGINS_DIRECT3D9_SETTINGS_H
#define IDENTI3D_SRC_PLUGINS_DIRECT3D9_SETTINGS_H

#include <src/identi3d/General.h>
#include <src/utils/DebugManager.h>

#define D3DSETTINGS_DEFAULT_WINDOWED			true
#define D3DSETTINGS_DEFAULT_HARDWAREACCELERATED	true
#define D3DSETTINGS_DEFAULT_STENCILBUFFER		true

#define D3DSETTINGS_DEFAULT_SCREEN_WIDTH		800
#define D3DSETTINGS_DEFAULT_SCREEN_HEIGHT		600
#define D3DSETTINGS_DEFAULT_REFRESH_RATE		60
#define D3DSETTINGS_DEFAULT_COLOR_DEPTH			32

namespace Identi3D
{
	
	class Direct3D9SettingsManager
	{
	private:
		OptionTree	*_tree;

	private:

		/*
		 * Convert from string to bool.
		 */

	public:
		bool _is_windowed;
		bool _is_hardware_accelerated;
		bool _is_stencil_buffer_enabled;
		UINT _screen_width;
		UINT _screen_height;
		UINT _refresh_rate;
		UINT _color_depth;

	public:
		
		Direct3D9SettingsManager(void) { reset(); };
		~Direct3D9SettingsManager(void) {} ;

		/*
		 * Reset and load default values.
		 */
		void reset(void);

		/*
		 * Read configuration from OptionTree.
		 */
		bool read(OptionTree *tree);

		/*
		 * Write configuration to OptionTree.
		 */
		bool write(void);
	};

};

#endif // IDENTI3D_SRC_PLUGINS_DIRECT3D9_SETTINGS_H
