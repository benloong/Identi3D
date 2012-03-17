//
// File: SettingsManager.cc
// ========================
//

#include <src/utils/SettingsManager.h>
#include <src/utils/OptionTree.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{
	SettingsManager::~SettingsManager(void)
	{
		delete _tree;
	}

	HRESULT SettingsManager::load(const TCHAR *filename)
	{
		delete _tree;
		_tree = new OptionTree();
		_tree->setDebugManager(_debugger);

		FILE *fp;
		int correct, total;
		TCHAR name[OPTIONTREE_NAME_MAXLEN + 1], value[OPTIONTREE_VALUE_MAXLEN + 1];

		_tfopen_s(&fp, filename, __T("r"));
		if(fp == NULL) {
			_DebugPrint(_debugger, E_FILE_OPEN_FAILURE, filename);
			return E_FAIL;
		}
		correct = 0, total = 0;
		memset(name, 0, sizeof(name));
		memset(value, 0, sizeof(value));
		while(!feof(fp)) {
			_ftscanf_s(fp, __T("%s%s\r\n"), name, OPTIONTREE_NAME_MAXLEN + 1, value, OPTIONTREE_VALUE_MAXLEN + 1);
			if(_tree->addElement(name, value) != NULL) {
				correct++;
			}
			total++;
		}
		fclose(fp);
		_DebugPrint(_debugger, I_SETTINGS_LOAD_COMPLETED, filename, correct, total);
		return S_OK;
	}

	HRESULT SettingsManager::save(const TCHAR *filename)
	{
		FILE *fp;

		_tfopen_s(&fp, filename, __T("w"));
		if(fp == NULL) {
			_DebugPrint(_debugger, E_FILE_OPEN_FAILURE, filename);
			return E_FAIL;
		}
		saveOptionElement(NULL, fp);
		fclose(fp);
		return S_OK;
	}

	void SettingsManager::saveOptionElement(OptionElement *elem, FILE *fp)
	{
		OptionIterator iter((elem == NULL) ? _tree->getRootIterator() : elem->child);
		TCHAR loc[OPTIONTREE_LOCATION_MAXLEN];
		
		while(!iter.end()) {
			if((*iter).child) {
				saveOptionElement(iter.get(), fp);
			} else {
				if(SUCCEEDED(_tree->getLocation(iter.get(), loc, OPTIONTREE_LOCATION_MAXLEN))) {
					_ftprintf_s(fp, __T("%s %s\n"), loc, (*iter).value);
				}
			}
			++iter;
		}
	}

	void SettingsManager::setDebugManager(DebugManager *new_debugger)
	{ 
		_debugger = new_debugger;
		if(_tree) _tree->setDebugManager(new_debugger);
	}

};
