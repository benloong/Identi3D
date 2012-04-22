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

	struct SystemStartupProperties
	{
		bool disable_debug_manager;
		bool disallow_fallback_config;

		SystemStartupProperties(bool disableDebugManager, bool disallowFallbackConfig) :
			disable_debug_manager(disableDebugManager),
			disallow_fallback_config(disallowFallbackConfig) {};
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
		System &operator=(System &rhs);

	private:
		SystemState _state;

		DebugManager *_debugger;
		SettingsManager *_confmgr;
		EventDispatcher	*_dispatcher;

		Renderer *_renderer;

		std::wstring _conf_path;

	public:

		System(void);
		~System(void);
		
		/*
		 * Initialize the system and load configuration.
		 * Acceptable Flag:
		 *		SystemFlag_AllowFallbackConfig
		 *		SystemFlag_DisableDebugManager
		 */
		bool init(const std::wstring &config_name, const SystemStartupProperties &prop);

		/*
		 * Release the system and save configuration.
		 */
		bool release(bool save_config = true);

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
		inline Renderer *getRenderer(void) const
		{
			return _renderer;
		}

		/*
		 * Get DebugManager handle.
		 */
		inline DebugManager *getDebugManager(void) const 
		{
			return _debugger;
		}

		/*
		 * Get SettingsManager handle.
		 */
		inline SettingsManager *getSettingsManager(void) const
		{
			return _confmgr;
		}

		/*
		 * Get EventDispatcher handle.
		 */
		inline EventDispatcher getEventDispatcher(void) const
		{
			return _dispatcher;
		}

		/*
		 * Get system state.
		 */
		inline SystemState getState(void) const
		{
			return _state;
		}

	};

};

#endif // IDENTI3D_SRC_IDENTI3D_SYSTEM_H
