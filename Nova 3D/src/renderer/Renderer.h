//
// File: Renderer.h
// ================
// Basic renderer for Nova 3D.
//
// Copyright (c) Sparkfire, 2011 - 2012
//

#ifndef NOVA3D_SRC_RENDERER_RENDERER_H
#define NOVA3D_SRC_RENDERER_RENDERER_H

#include "src/nova-3d/NovaGeneral.h"
#include "src/renderer/RenderDevice.h"
#include "src/utils/Singleton.h"

namespace Nova3D
{
	class DebugManager;
	class SettingsManager;

	//
	// Enum: RenderBackendType
	// =======================
	// The type of render backend.
	//
	// Value:
	//	RenderBackendType_Direct3D9: Microsoft(R) DirectX(R) 9 render backend.
	//	RenderBackendType_OpenGL: OpenGL render backend (under implementation)
	//	RenderBackendType_ForceDword: Not used. Forcing the type to be compiled in the size of DWORD.
	//
	enum RenderBackendType
	{
		RenderBackendType_Direct3D9	= 0,
		RenderBackendType_OpenGL	= 1,

		RenderBackendType_ForceDword= 0xFFFFFFFF
	};

	//
	// Class: Renderer <Inherits from Singleton>
	// =========================================
	// A global instance of renderer.
	//
	class Renderer : public Singleton<Renderer>
	{
	private:
		RenderDevice		*render_device;
		HMODULE				plugin_handle;

	public:

		//
		// Function: Renderer(void)
		// ========================
		// Class default constructor.
		//
		Renderer(void);

		//
		// Function: ~Renderer(void)
		// =========================
		// Class default destructor.
		//
		~Renderer(void);

		//
		// Function: createDevice(RenderBackendType)
		// =========================================
		// Create a render device specified by RenderBackendType.
		//
		HRESULT	createDevice(RenderBackendType type);
		
		//
		// Function: releaseDevice(void)
		// =============================
		// Release device and do some cleaning.
		//
		void	releaseDevice(void);

		//
		// Inline Function: getDevice(void)
		// ================================
		// Get render device.
		//
		inline RenderDevice	*getDevice(void) { return render_device; }
		
		//
		// Inline Function: getPluginsHandle(void)
		// =======================================
		// Get DLL handle of plugins.
		//
		inline HMODULE	getPluginsHandle(void) { return plugin_handle; }
	};

};

#endif