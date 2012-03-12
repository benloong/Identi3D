//
// File: SettingsManager.h
// =======================
// Load and save configuration on I/O level.
//

#ifndef CORN3D_SRC_UTILS_SETTINGSMANAGER_H
#define CORN3D_SRC_UTILS_SETTINGSMANAGER_H

#include <src/corn3d/General.h>

namespace Corn3D
{

	class __declspec(dllexport) SettingsManager
	{
	private:
		OptionTree *_tree;
		DebugManager *_debugger;

	private:
		void saveOptionElement(OptionElement *elem, FILE *fp);

	public:
		SettingsManager(void) : _tree(NULL), _debugger(NULL) {} ;
		~SettingsManager(void);

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
		OptionTree *getOptionTree(void) { return _tree; }

		/*
		 * Set a debug manager.
		 */
		void setDebugManager(DebugManager *new_debugger = NULL);
	};

};

#endif // CORN3D_SRC_UTILS_SETTINGSMANAGER_H