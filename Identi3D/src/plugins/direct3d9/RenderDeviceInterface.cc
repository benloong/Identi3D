//
// File: RenderDeviceInterface.cc
// ==============================
//

#include <src/plugins/direct3d9/PluginDirect3D9.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{

	bool CreateRenderDevice(HMODULE plugin, RenderDevice **device, DebugManager *debugger)
	{
		if(plugin == NULL || device == NULL)
			return false;

		*device = ntnew PluginDirect3D9(plugin, debugger);
		if(*device == NULL) {
			if(debugger)
				debugger->print(__FILE__, __LINE__, false, E_OUT_OF_MEMORY);
			return false;
		}
		return true;
	}

	void ReleaseRenderDevice(RenderDevice **device)
	{
		delete *device;
	}

};
