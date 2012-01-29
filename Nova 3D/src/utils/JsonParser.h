//
// File: JsonParser.h
// ==================
// Parse json file and retrive items.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_SRC_UTILS_JSONPARSER_H
#define NOVA3D_SRC_UTILS_JSONPARSER_H

#include <src/nova-3d/NovaGeneral.h>

#define JSON_ROOT_OBJECT_NAME	__T("_ROOT")

namespace Nova3D
{

	//
	// Class: JsonReaderListener
	// ======
	// Classes inherited from this class can receive
	// notifications post from JsonReader.
	//
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
	
	//
	// Class: JsonReader
	// ======
	// Read a json-formatted file.
	//
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

	//
	// Class: JsonWriter
	// ======
	// Write a json-formatted file.
	//
	class JsonWriter
	{
	private:
		FILE *fp;
		UINT layer, item_count;

		void doIndent(void);
		void parseString(const TCHAR *str);
		
	public:
		JsonWriter(void);
		~JsonWriter(void);

		HRESULT lockFile(const TCHAR *file_name);
		HRESULT unlockFile(void);

		HRESULT writeObject(const TCHAR *name);
		HRESULT encloseObject(void);
		HRESULT encloseAllObjects(void);

		HRESULT writeString(const TCHAR *name, const TCHAR *value);
		HRESULT writeNumber(const TCHAR *name, const double value, bool forceInt);
		HRESULT writeBool(const TCHAR *name, const bool value);
		HRESULT writeComment(const TCHAR *comment);

		inline UINT getItemCount(void) { return item_count; }
	};

};

#endif
