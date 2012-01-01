//
// File: SettingsManager.h
// =======================
// Provides a settings management for Nova 3D.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_SRC_UTILS_SETTINGSMANAGER_H
#define NOVA3D_SRC_UTILS_SETTINGSMANAGER_H

#include "src/nova-3d/NovaGeneral.h"
#include "src/utils/JsonParser.h"

#define SETTINGS_MANAGER_HASH_TABLE_SIZE		37

namespace Nova3D
{
	
	typedef struct tagItemProperties
	{
		TCHAR *name;
		TCHAR *value;
		struct tagItemProperties *next;
	}ItemProperties;

	class SettingsManager : public JsonReaderListener
	{
	private:
		typedef struct tagSettingsGroup
		{
			TCHAR *name;
			ItemProperties *prop;
			struct tagSettingsGroup *next;
		}SettingsGroup;

		enum ListenerState
		{
			ListenerState_Idle	= 0,
			ListenerState_Group = 1,
			ListenerState_Item	= 2,
			ListenerState_AbandonData = 3
		};

		SettingsGroup *global_config[SETTINGS_MANAGER_HASH_TABLE_SIZE];
		SettingsGroup *current;
		ListenerState state;

		void freeGlobal(void);
		void freeGroup(SettingsGroup *group);
		void freeItem(ItemProperties *item);
		
	public:
		SettingsManager(void);
		~SettingsManager(void);

		HRESULT read(const TCHAR *file_name);
		HRESULT write(const TCHAR *file_name);
		HRESULT getGroupByName(const TCHAR *group_name, ItemProperties **group);

		void enterObject(const TCHAR *name);
		void quitObject(const TCHAR *name);
		void newString(const TCHAR *name, const TCHAR *value);
	};

	class SettingsEnumerator
	{
	public:
		SettingsEnumerator(void);
		virtual ~SettingsEnumerator(void);
		void enumerateSettings(const TCHAR *group_name, SettingsManager *manager);

	protected:
		virtual void processEntries(const TCHAR *name, TCHAR *value, UINT buffer_size) = 0;
	};

};

#endif
