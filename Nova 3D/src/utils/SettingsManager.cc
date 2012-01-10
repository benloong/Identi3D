//
// File: SettingsManager.cc
// ========================
//

#include "src/utils/SettingsManager.h"

#define DEFAULT_STRING_LENGTH	256

namespace Nova3D
{

	inline ULONG hashByString(const TCHAR *str)
	{
		const ULONG seed = 131;
		ULONG hash = 0;

		if(str == NULL) return 0;
		while(*str) hash = hash * seed + *str++;
		return hash % SETTINGS_MANAGER_HASH_TABLE_SIZE;
	}

	SettingsManager::SettingsManager(void)
	{
		memset(global_config, 0, sizeof(global_config));
		state = ListenerState_Idle;
		current = NULL;
	}

	SettingsManager::~SettingsManager(void)
	{
		freeGlobal();
	}

	void SettingsManager::freeGlobal(void)
	{
		for(unsigned int i = 0; i < SETTINGS_MANAGER_HASH_TABLE_SIZE; i++)
		{
			if(global_config[i] != NULL) {
				freeGroup(global_config[i]);
				global_config[i] = NULL;
			}
		}
	}

	void SettingsManager::freeGroup(SettingsGroup *group)
	{
		if(group == NULL) return ;
		
		SettingsGroup *p, *s;
		
		p = group;
		s = group->next;

		while(s != NULL){
			if(p->prop != NULL) freeItem(p->prop);
			delete p;
			p = s;
			s = p->next;
		}
		
		if(p->prop != NULL) freeItem(p->prop);
		delete p;
		return ;
	}

	void SettingsManager::freeItem(ItemProperties *item)
	{
		if(item == NULL) return ;

		ItemProperties *p, *s;

		p = item;
		s = item->next;

		while(s != NULL)
		{
			if(p->name != NULL) delete[] p->name;
			if(p->value != NULL) delete[] p->value;
			delete p;
			p = s;
			s = p->next;
		}
		
		if(p->name != NULL) delete[] p->name;
		if(p->value != NULL) delete[] p->value;
		delete p;
		return ;
	}

	HRESULT SettingsManager::read(const TCHAR *file_name)
	{
		JsonReader *reader;
		HRESULT hr;

		reader = new (std::nothrow) JsonReader(this);
		if(reader == NULL)
			return E_FAIL;
		hr = reader->lockFile(file_name);
		if(FAILED(hr)) {
			delete reader;
			return E_FAIL;
		}

		hr = reader->parse();
		if(FAILED(hr)) {
			delete reader;
			return E_FAIL;
		}

		delete reader;
		return S_OK;
	}

	HRESULT SettingsManager::write(const TCHAR *file_name)
	{
		JsonWriter *writer;
		HRESULT hr;
		unsigned int i;
		ItemProperties *p;
		SettingsGroup *s;

		writer = new (std::nothrow) JsonWriter();
		if(writer == NULL)
			return E_FAIL;
		hr = writer->lockFile(file_name);
		if(FAILED(hr)) {
			delete writer;
			return E_FAIL;
		}

		for(i = 0; i < SETTINGS_MANAGER_HASH_TABLE_SIZE; i++)
		{
			if(global_config[i] != NULL) {
				s = global_config[i];
				while(s)
				{
					writer->writeObject(s->name);
					p = s->prop;
					while(p)
					{
						writer->writeString(p->name, p->value);
						p = p->next;
					}
					s = s->next;
					writer->encloseObject();
				}
			}
		}

		writer->encloseAllObjects();
		delete writer;
		return S_OK;
	}

	HRESULT SettingsManager::getGroupByName(const TCHAR *group_name, ItemProperties **group)
	{
		if(group_name == NULL) return E_FAIL;
		ULONG hashval = hashByString(group_name);

		SettingsGroup *s;
		s = global_config[hashval];
		while(s != NULL)
		{
			if(_tcscmp(group_name, s->name) == 0) {
				*group = s->prop;
				return S_OK;
			}
		}
		*group = NULL;
		return E_FAIL;
	}

	void SettingsManager::enterObject(const TCHAR *name)
	{
		SettingsGroup *p, *s;
		ULONG hashval;

		if(state == ListenerState_Idle && _tcscmp(name, JSON_ROOT_OBJECT_NAME) == 0) {
			state = ListenerState_Group;
		} else if(state == ListenerState_Group) {
			hashval = hashByString(name);
			if(global_config[hashval] == NULL) {
				global_config[hashval] = new (std::nothrow) SettingsGroup;
				s = global_config[hashval];
			} else {
				p = global_config[hashval];
				s = p->next;
				while(s != NULL)
				{
					p = s;
					s = s->next;
				}
				p->next = new (std::nothrow) SettingsGroup;
				s = p->next;
			}
			if(s == NULL) {
				state = ListenerState_Idle;
				return ;
			}
			s->name = new (std::nothrow) TCHAR[DEFAULT_STRING_LENGTH];
			if(s->name == NULL) {
				state = ListenerState_Idle;
				return ;
			}
			_tcsncpy_s(s->name, DEFAULT_STRING_LENGTH, name,  DEFAULT_STRING_LENGTH - 1);
			s->prop = NULL;
			s->next = NULL;
			current = s;
			state = ListenerState_Item;
		}
	}

	void SettingsManager::quitObject(const TCHAR *name)
	{
		if(state == ListenerState_Group && _tcscmp(name, JSON_ROOT_OBJECT_NAME) == 0) {
			state = ListenerState_Idle;
		} else if(state == ListenerState_Item) {
			state = ListenerState_Group;
			current = NULL;
		}
	}

	void SettingsManager::newString(const TCHAR *name, const TCHAR *value)
	{
		ItemProperties *p, *s;
		if(state == ListenerState_Item) {
			if(current == NULL) return ;
			if(current->prop == NULL) {
				current->prop = new (std::nothrow) ItemProperties;
				s = current->prop;
			} else {
				p = current->prop;
				s = p->next;
				while(s != NULL)
				{
					p = s;
					s = s->next;
				}
				p->next = new (std::nothrow) ItemProperties;
				s = p->next;
			}
			if(s == NULL) {
				state = ListenerState_Idle;
				return ;
			}
			s->name = new (std::nothrow) TCHAR[DEFAULT_STRING_LENGTH];
			if(s->name == NULL) {
				state = ListenerState_Idle;
				return ;
			}
			s->value = new (std::nothrow) TCHAR[DEFAULT_STRING_LENGTH];
			if(s->value == NULL) {
				state = ListenerState_Idle;
				return ;
			}
			_tcsncpy_s(s->name, DEFAULT_STRING_LENGTH, name, DEFAULT_STRING_LENGTH - 1);
			_tcsncpy_s(s->value, DEFAULT_STRING_LENGTH, value, DEFAULT_STRING_LENGTH - 1);
			s->next = NULL;
		}
	}

	SettingsEnumerator::SettingsEnumerator(void)
	{
	}

	SettingsEnumerator::~SettingsEnumerator(void)
	{
	}

	void SettingsEnumerator::enumerateSettings(const TCHAR *group_name, SettingsManager &manager)
	{
		if(group_name == NULL) return ;
		ItemProperties *p;
		manager.getGroupByName(group_name, &p);
		if(p == NULL) return ;
		while(p != NULL)
		{
			processEntries(p->name, p->value, DEFAULT_STRING_LENGTH);
			p = p->next;
		}
		return ;
	}

};