//
// File: RenderDeviceInterface.cc
// ==============================
//

#include <src/plugins/direct3d9/PluginDirect3D9.h>
#include <src/utils/DebugManager.h>

namespace Corn3D
{

	HRESULT CreateRenderDevice(HMODULE plugin, RenderDevice **device, DebugManager *debugger)
	{
		if(plugin == NULL || device == NULL)
			return E_FAIL;

		*device = new (std::nothrow) PluginDirect3D9(plugin, debugger);
		if(*device == NULL) {
			_DebugPrint(debugger, E_OUT_OF_MEMORY);
			return E_FAIL;
		}
		return S_OK;
	}

	void ReleaseRenderDevice(RenderDevice **device)
	{
		delete *device;
	}

};
