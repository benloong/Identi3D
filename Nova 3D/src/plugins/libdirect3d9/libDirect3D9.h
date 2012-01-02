//
// File: libDirect3D9.h
// ====================
//

#ifndef NOVA3D_SRC_PLUGINS_LIBDIRECT3D9_LIBDIRECT3D9_H
#define NOVA3D_SRC_PLUGINS_LIBDIRECT3D9_LIBDIRECT3D9_H

#include "src/nova-3d/NovaGeneral.h"
#include "src/renderer/RenderDevice.h"
#include "src/utils/SettingsManager.h"

#include <d3d9.h>

namespace Nova3D
{
	class DebugManager;

	class Direct3D9SettingsEnumerator : SettingsEnumerator
	{
	private:
		bool windowed, hardware_accel;
		UINT width, height, refresh_rate;

		bool need_update;

	protected:
		void processEntries(const TCHAR *name, TCHAR *value, UINT buffer_size);
		void setDefaultValue(void);
		
	public:
		Direct3D9SettingsEnumerator(void);
		~Direct3D9SettingsEnumerator(void);

		inline void reset(void) { setDefaultValue(), need_update = true; }

		inline UINT getWidth(void) { return width; }
		inline UINT getHeight(void) { return height; }
		inline UINT getRefreshRate(void) { return refresh_rate; }
		inline bool isWindowed(void) { return windowed; }
		inline bool isHardwareAccelerated(void) { return hardware_accel; }

		void setResolution(UINT new_width, UINT new_height, UINT new_refresh_rate);
		inline void setWindowed(bool is_windowed) { windowed = is_windowed, need_update = true; }
		inline void setHardwareAccelerated(bool is_accelerated) { hardware_accel = is_accelerated, need_update = true; }

	};

	class PluginDirect3D9 : public RenderDevice
	{
	private:
		LPDIRECT3D9				direct3d;
		LPDIRECT3DDEVICE9		direct_device;
		D3DPRESENT_PARAMETERS	present_parameters;
		D3DCOLOR				clear_color;

		DebugManager				*debug_manager;
		Direct3D9SettingsEnumerator	*settings_enumerator;

		bool	is_scene_running;
		bool	is_stencil_enabled;

	private:
		HRESULT run(void);
		bool	checkPrerequisite(void);

	public:
		PluginDirect3D9(HMODULE plugin, DebugManager *debugmgr = NULL);
		~PluginDirect3D9(void);

		HRESULT init(HWND window, UINT min_depth, UINT min_stencil);
		void	release(void);
		bool	isRunning(void);

		HRESULT startRendering(bool clear_pixel, bool clear_depth, bool clear_stencil);
		void	stopRendering(void);
		HRESULT	clear(bool clear_pixel, bool clear_depth, bool clear_stencil);

		void	setClearColor(float red, float green, float blue);

		SettingsEnumerator	&getSettingsEnumerator(void);
	};

};

#endif
