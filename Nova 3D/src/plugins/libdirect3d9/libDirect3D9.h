//
// File: libDirect3D9.h
// ====================
// Direct3D 9 render plugin.
//

#ifndef NOVA3D_SRC_PLUGINS_LIBDIRECT3D9_LIBDIRECT3D9_H
#define NOVA3D_SRC_PLUGINS_LIBDIRECT3D9_LIBDIRECT3D9_H

#include <src/nova-3d/NovaGeneral.h>
#include <src/renderer/RenderDevice.h>
#include <src/utils/SettingsManager.h>

#include <d3d9.h>

namespace Nova3D
{

	//
	// Class: Direct3D9SettingsEnumerator <Inherits from SettingsEnumerator>
	// ======
	// Read graphics settings from SettingsManager, process and store Direct3D configuration.
	//
	class Direct3D9SettingsEnumerator : public SettingsEnumerator
	{
	private:
		bool windowed, hardware_accel;
		UINT width, height, refresh_rate;

		bool need_update;

	protected:
		void processEntries(const TCHAR *name, TCHAR *value, UINT buffer_size);
		void setDefaultValue(void);

	public:

		//
		// Function: Direct3D9SettingsEnumerator(void)
		// =========
		// Class default constructor.
		//
		Direct3D9SettingsEnumerator(void);

		//
		// Function: ~Direct3D9SettingsEnumerator(void)
		// =========
		// Class default destructor.
		//
		~Direct3D9SettingsEnumerator(void);

		//
		// Function: reset(void)
		// =========
		// Reset settings and do update to SettingsManager.
		//
		void reset(void)
		{ 
			setDefaultValue();
			need_update = true; 
		}

		//
		// Function: getWidth(void)
		// =========
		// Get screen width.
		//
		const UINT getWidth(void) const 
		{ 
			return width; 
		}

		//
		// Function: getHeight(void)
		// =========
		// Get screen height.
		//
		const UINT getHeight(void) const 
		{ 
			return height; 
		}

		//
		// Function: getRefreshRate(void)
		// =========
		// Get refresh rate.
		//
		const UINT getRefreshRate(void) const { 
			return refresh_rate; 
		}

		//
		// Function: isWindowed(void)
		// =========
		// Is under windowed mode.
		//
		const bool isWindowed(void) const
		{ 
			return windowed; 
		}

		//
		// Function: isHardwareAccelerated(void)
		// =========
		// Is hardware acceleration enabled.
		//
		const bool isHardwareAccelerated(void) const
		{ 
			return hardware_accel;
		}

		//
		// Function: setResolution(Width, Height, RefreshRate)
		// =========
		// Sets display mode to Width x Height @ RefreshRate.
		// Please update device to take effect.
		//
		void setResolution(UINT new_width, UINT new_height, UINT new_refresh_rate);

		//
		// Function: setWindowed(isWindowed)
		// =========
		// Sets windowed mode to isWindowed.
		// Please update device to take effect.
		//
		void setWindowed(bool is_windowed)
		{
			windowed = is_windowed;
			need_update = true; 
		}

		//
		// Function: setHardwareAccelerated(isAccelerated)
		// =========
		// Sets hardware mode to isAccelerated.
		// Please update device to take effect.
		//
		void setHardwareAccelerated(bool is_accelerated) 
		{ 
			hardware_accel = is_accelerated;
			need_update = true; 
		}

	};

	//
	// Class: PluginDirect3D9 <Inherits from RenderDevice>
	// ======
	// Direct3D 9 render plugins.
	//
	class PluginDirect3D9 : public RenderDevice
	{
	private:
		LPDIRECT3D9				direct3d;
		LPDIRECT3DDEVICE9		direct_device;
		D3DPRESENT_PARAMETERS	present_parameters;
		D3DCOLOR				clear_color;

		DebugManager			*debug_manager;
		Direct3D9SettingsEnumerator	settings_enumerator;

		bool	is_scene_running;
		bool	is_stencil_enabled;

	private:
		HRESULT run(void);
		bool	checkPrerequisite(void);

	public:

		//
		// Function: PluginDirect3D9(DllHandle, DebugManager[optional])
		// =========
		// Class default constructor, creates plugin by DllHandle.
		//
		PluginDirect3D9(HMODULE plugin, DebugManager *debug_manager = NULL);

		//
		// Function: ~PluginDirect3D9(void)
		// =========
		// Class default destructor.
		//
		~PluginDirect3D9(void);

		//
		// Function: init(RenderTarget, MinimumDepth, MinimumStencil)
		// =========
		// Initializes device on RenderTarget.
		// Set MinimunStencil = 0 to disable stencil buffer.
		//
		HRESULT init(RenderTarget *target, UINT min_depth, UINT min_stencil);

		//
		// Function: release(void)
		// =========
		// Releases device. Calling destructor will eventually do this.
		//
		void	release(void);

		//
		// Function: isRunning(void)
		// =========
		// Returns running state.
		//
		bool	isRunning(void);

		//
		// Function: startRendering(ClearPixelBuffer, ClearDepthBuffer, ClearStencilBuffer)
		// =========
		// Starts rendering procedure.
		//
		HRESULT startRendering(bool clear_pixel, bool clear_depth, bool clear_stencil);

		//
		// Function: endRendering(void)
		// =========
		// Stops rendering procedure.
		//
		void	endRendering(void);

		//
		// Function: clear(ClearPixelBuffer, ClearDepthBuffer, ClearStencilBuffer)
		// =========
		// Clears buffers.
		//
		HRESULT	clear(bool clear_pixel, bool clear_depth, bool clear_stencil);

		//
		// Function: setClearColor(Red, Green, Blue)
		// =========
		// Sets clear color to RGB.
		//
		void	setClearColor(float red, float green, float blue);

		//
		// Function: getSettingsEnumerator(void)
		// =========
		// Returns handle to SettingsEnumerator.
		//
		SettingsEnumerator	&getSettingsEnumerator(void);

		//
		// Function: getWidth(void)
		// =========
		// Get screen width.
		//
		inline const UINT	getWidth(void) const 
		{ 
			return settings_enumerator.getWidth();
		}

		//
		// Function: getHeight(void)
		// =========
		// Get screen height.
		//
		inline const UINT	getHeight(void) const
		{
			return settings_enumerator.getHeight();
		}

	};

};

#endif
