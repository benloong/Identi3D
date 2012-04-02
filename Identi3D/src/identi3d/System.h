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
	private:
		System(System &s);

	private:
		SystemState _state;

		DebugManager *_debugger;
		SettingsManager *_confmgr;
		EventDispatcher	*_dispatcher;

		Renderer *_renderer;

		wchar_t _conf_path[261];

	public:

		System(void);
		~System(void);
		
		/*
		 * Initialize the system and load configuration.
		 * Acceptable Flag:
		 *		SystemFlag_AllowFallbackConfig
		 *		SystemFlag_DisableDebugManager
		 */
		HRESULT init(DWORD flag = NULL, const wchar_t *config_name = NULL);

		/*
		 * Release the system and save configuration.
		 */
		HRESULT release(bool save_config = true);

		/*
		 * Create a renderer.
		 */
		Renderer *createRenderer(void);

		/*
		 * Release the allocated renderer.
		 */
		void releaseRenderer(void);
		
		/*
		 * Start event listening.
		 */
		int	start(void);

		/*
		 * Kill listening process.
		 */
		void kill(void);

		/*
		 * Get Renderer handle.
		 */
		Renderer *getRenderer(void) const { return _renderer; }

		/*
		 * Get DebugManager handle.
		 */
		DebugManager *getDebugManager(void) const { return _debugger; }

		/*
		 * Get SettingsManager handle.
		 */
		SettingsManager *getSettingsManager(void) const { return _confmgr; }

		/*
		 * Get EventDispatcher handle.
		 */
		EventDispatcher *getEventDispatcher(void) const { return _dispatcher; }

		/*
		 * Get system state.
		 */
		SystemState getState(void) const { return _state; }

	};

};

#endif // IDENTI3D_SRC_IDENTI3D_SYSTEM_H
