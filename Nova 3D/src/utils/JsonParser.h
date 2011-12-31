//
// File: JsonParser.h
// ==================
// Parse json file and retrive items.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_SRC_UTILS_JSONPARSER_H
#define NOVA3D_SRC_UTILS_JSONPARSER_H

#include "src/nova-3d/NovaGeneral.h"

#define JSON_ROOT_OBJECT_NAME	__T("_ROOT")

namespace Nova3D
{

	class JsonReaderListener
	{
	public:
		JsonReaderListener(void) {};
		virtual ~JsonReaderListener(void) {};

		virtual void newBool(const TCHAR *name, bool value) {};
		virtual void newNumber(const TCHAR *name, double value) {};
		virtual void newObject(const TCHAR *name) {};
		virtual void enterObject(const TCHAR *name) {};
		virtual void quitObject(const TCHAR *name) {};
		virtual void newString(const TCHAR *name, const TCHAR *value) {};
	};
	
	class JsonReader
	{
	private:
		FILE *fp;
		JsonReaderListener *listener;

		TCHAR	transformCharacter(TCHAR ch);
		HRESULT getString(TCHAR *buffer, UINT limit);

	public:
		JsonReader(void);
		JsonReader(JsonReaderListener *event_listener);
		~JsonReader(void);

		HRESULT setEventListener(JsonReaderListener *event_listener);
		HRESULT lockFile(const TCHAR *file_name);
		HRESULT unlockFile(void);
		HRESULT parse(void);
	};

};

#endif
