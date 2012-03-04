//
// File: SettingsManager.cc
// ========================
//

#include <src/utils/SettingsManager.h>
#include <src/utils/OptionTree.h>

namespace Corn3D
{

	SettingsManager::SettingsManager(void)
	{
		tree = new OptionTree;
	}

	SettingsManager::~SettingsManager(void)
	{
		delete tree;
	}

	HRESULT SettingsManager::load(const TCHAR *filename)
	{
		return S_OK;
	}

	HRESULT SettingsManager::save(const TCHAR *filename)
	{
		return S_OK;
	}

};
