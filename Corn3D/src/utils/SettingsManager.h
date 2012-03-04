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
		OptionTree *tree;

	public:

		SettingsManager(void);
		~SettingsManager(void);

		HRESULT load(const TCHAR *filename);
		HRESULT save(const TCHAR *filename);

		OptionTree *getOptionTree(void) const { return tree; }
	};

};

#endif // CORN3D_SRC_UTILS_SETTINGSMANAGER_H