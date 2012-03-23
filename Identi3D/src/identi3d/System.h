//
// File: System.h
// ==============
// An interface to Identi3D.
//

#ifndef IDENTI3D_SRC_IDENTI3D_SYSTEM_H
#define IDENTI3D_SRC_IDENTI3D_SYSTEM_H

#include <src/identi3d/Identi3D.h>
#include <src/utils/Singleton.h>

namespace Identi3D
{

	enum SystemFlag
	{
		SystemFlag_AllowFallbackConfig	= 0x00000001,
		SystemFlag_DisableDebugManager	= 0x00000002,

		SystemFlag_CreateRenderer		= 0x00000010,
		SystemFlag_CreateDefaultDevice	= 0x00000020,

		SystemFlag_ForceDword			= 0xFFFFFFFF
	};

	enum SystemState
	{
		SystemState_NotInitialized		= 0,
		SystemState_Idle				= 1,
		SystemState_Listening			= 2,

		SystemState_ForceInt			= 0xFFFF
	};

	class System : public Singleton<System>
	{
	protected:
		bool _isload;
		SystemState _state;

		DebugManager *_debugger;
		SettingsManager *_confmgr;

		Renderer *_renderer;

		TCHAR _conf_path[261];

	public:

		System(void);
		System(System &s);
		~System(void);
		
		/*
		 * Initialize the system and load configuration.
		 * Acceptable Flag:
		 *		SystemFlag_AllowFallbackConfig
		 *		SystemFlag_DisableDebugManager
		 *		SystemFlag_CreateRenderer
		 *		SystemFlag_CreateDefaultDevice
		 */
		HRESULT init(DWORD flag = NULL, const TCHAR *config_name = NULL);

		/*
		 * Release the system and save configuration.
		 */
		HRESULT release(bool save_config = true);

		/*
		 * Create a renderer.
		 * Acceptable Flag:
		 *		SystemFlag_DisableDebugManager
		 *		SystemFlag_CreateDefaultDevice	(will fail SILENTLY)
		 */
		Renderer *createRenderer(DWORD flag = NULL);

		/*
		 * Release the allocated renderer.
		 */
		void releaseRenderer(void);
		
		/*
		 * Start event listening.
		 */
		int		start(void);

		/*
		 * Get Renderer handle.
		 */
		Renderer *getRenderer(void) const { return _renderer; }

		/*
		 * Get DebugManager handle.
		 */
		DebugManager *getDebugManager(void) const { return _debugger; }

		/*
		 * Get system state.
		 */
		SystemState getState(void);

	};

};

#endif // IDENTI3D_SRC_IDENTI3D_SYSTEM_H
