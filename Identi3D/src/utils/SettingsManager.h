//
// File: SettingsManager.h
// =======================
// Load and save configuration on I/O level.
//

#ifndef IDENTI3D_SRC_UTILS_SETTINGSMANAGER_H
#define IDENTI3D_SRC_UTILS_SETTINGSMANAGER_H

#include <src/identi3d/General.h>
#include <src/utils/OptionTree.h>

namespace Identi3D
{

	class __declspec(dllexport) SettingsManager
	{
		friend class System;

	private:
		OptionTree _tree;
		DebugManager *_debugger;

	private:
		SettingsManager(DebugManager *debugger);
		SettingsManager(SettingsManager &mgr);
		~SettingsManager(void);

		void saveOptionElement(OptionElement *elem, FILE *fp);

	public:

		/*
		 * Load configuration from file.
		 */
		HRESULT load(const TCHAR *filename);

		/*
		 * Write configuration to file.
		 */
		HRESULT save(const TCHAR *filename);

		/*
		 * Get the handle of global OptionTree.
		 */
		OptionTree *getOptionTree(void) { return &_tree; }

		/*
		 * Set a debug manager.
		 */
		void setDebugManager(DebugManager *new_debugger = NULL);
	};

};

#endif // IDENTI3D_SRC_UTILS_SETTINGSMANAGER_H