//
// File: Renderer.h
// ================
//

#ifndef NOVA3D_SRC_RENDERER_RENDERER_H
#define NOVA3D_SRC_RENDERER_RENDERER_H

#include "src/nova-3d/NovaGeneral.h"
#include "src/renderer/RenderDevice.h"

namespace Nova3D
{
	class DebugManager;

	class SettingsManager;

	enum RenderBackendType
	{
		RenderBackendType_Direct3D9	= 0,
		RenderBackendType_OpenGL	= 1,

		RenderBackendType_ForceInt	= 65535
	};

	class Renderer
	{
	private:
		RenderDevice		*render_device;
		HMODULE				plugin_handle;

		DebugManager		*debug_manager;
		SettingsManager		*settings_manager;

	public:
		Renderer(SettingsManager *settingsmgr, DebugManager *debugmgr = NULL);
		~Renderer(void);

		HRESULT	createDevice(RenderBackendType type);
		void	releaseDevice(void);

		inline RenderDevice	*getDevice(void) { return render_device; }
		inline HMODULE	getPluginsHandle(void) { return plugin_handle; }
	};

};

#endif