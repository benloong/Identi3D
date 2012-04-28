//
// File: SettingsManager.cc
// ========================
//

#include <src/utils/SettingsManager.h>
#include <src/utils/DebugManager.h>
#include <src/identi3d/IdentiExceptions.h>

namespace Identi3D
{

	bool SettingsManager::load(const std::wstring &path)
	{
		std::wifstream fin;
		int correct, total;
		std::wstring name, value;

		try{
			_tree.clean();
			correct = 0, total = 0;

			fin.open(path);
			if(!fin.is_open()) throw FileOperationFailureException();

			while(!fin) {
				fin >> name >> value;
				if(name.length() == 0 || value.length() == 0) continue;
				if(_tree.addElement(name, value)) correct++;
				total++;
			}

			fin.close();
			_printVerboseMessage(__FILE__, __LINE__, I_SETTINGS_LOAD_COMPLETED, path, correct, total);
		} catch(std::exception &e) {
			_printException(__FILE__, __LINE__, e);
			_tree.clean();
			fin.close();
			return false;
		}
		return true;
	}

	bool SettingsManager::save(const std::wstring &path)
	{
		std::wofstream fout;

		try
		{
			fout.open(path);
			if(!fout.is_open()) throw FileOperationFailureException();

			saveElementRecursively(NULL, fout);
			fout.close();
		} catch(std::exception &e) {
			_printException(__FILE__, __LINE__, e);
			fout.close();
			return false;
		}
		return true;
	}

	void SettingsManager::saveElementRecursively(const OptionElement *elem, std::wofstream &fout)
	{
		OptionIterator iter((elem == NULL) ? _tree.getRootIterator() : elem->child);
		
		while(!iter.end()) {
			if(iter->child) {
				saveElementRecursively(iter.get(), fout);
			} else {
				fout << iter->name << TEXT(" ") << iter->value << std::endl;
			}
			++iter;
		}
	}

};
