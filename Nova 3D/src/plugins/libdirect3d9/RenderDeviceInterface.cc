//
// File: RenderDeviceInterface.cc
// ==============================
//

#include "src/plugins/libdirect3d9/libDirect3D9.h"
#include "src/utils/DebugManager.h"

namespace Nova3D
{

	HRESULT CreateRenderDevice(HMODULE plugin, RenderDevice **device, DebugManager *debugmgr)
	{
		if(plugin == NULL || device == NULL || debugmgr == NULL)
			return E_FAIL;

		*device = new (std::nothrow) PluginDirect3D9(plugin, debugmgr);
		if(*device == NULL) {
			_DebugPrintS(debugmgr, E_OUT_OF_MEMORY);
			return E_FAIL;
		}
		return S_OK;
	}

	void ReleaseRenderDevice(RenderDevice **device)
	{
		delete *device;
	}

};
