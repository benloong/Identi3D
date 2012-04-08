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
		SettingsManager(SettingsManager &mgr);
		SettingsManager &operator=(SettingsManager &rhs);

	private:
		void saveElementRecursively(OptionElement *elem, std::wofstream &fout);

	public:
		SettingsManager(DebugManager *debugger);
		~SettingsManager(void);

		/*
		 * Load configuration from file.
		 */
		bool load(const std::wstring &path);

		/*
		 * Write configuration to file.
		 */
		bool save(const std::wstring &path);

		/*
		 * Get the handle of global OptionTree.
		 */
		inline OptionTree *getOptionTree(void)
		{
			return &_tree;
		}

		/*
		 * Set a debug manager.
		 */
		inline void setDebugManager(DebugManager *new_debugger = NULL)
		{
			_debugger = new_debugger;
			_tree.setDebugManager(new_debugger);
		}
	};

};

#endif // IDENTI3D_SRC_UTILS_SETTINGSMANAGER_H