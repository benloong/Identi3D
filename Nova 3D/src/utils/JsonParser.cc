//
// File: JsonParser.cc
// ===================
//

#include "src/utils/JsonParser.h"

#include <stack>
#include <string>

#define MAX_STRING_BUFFER_LENGTH	256

typedef std::basic_string <TCHAR> tstring;

namespace Nova3D
{

	JsonReader::JsonReader(void)
	{
		fp = NULL;
		listener = NULL;
	}

	JsonReader::JsonReader(JsonReaderListener *event_listener)
	{
		fp = NULL;
		listener = event_listener;
	}

	JsonReader::~JsonReader(void)
	{
		unlockFile();
	}

	HRESULT JsonReader::setEventListener(JsonReaderListener *event_listener)
	{
		listener = event_listener;
		return S_OK;
	}

	HRESULT JsonReader::lockFile(const TCHAR *file_name)
	{
		_tfopen_s(&fp, file_name, __T("r"));
		if(fp == NULL)
			return E_FAIL;

		return S_OK;
	}

	HRESULT JsonReader::unlockFile(void)
	{
		if(fp != NULL)
			fclose(fp);

		return S_OK;
	}

	TCHAR JsonReader::transformCharacter(TCHAR ch)
	{
		switch(ch)
		{
		case __T('\"'):
		case __T('\\'):
		case __T('/'): 
			break;
		case __T('b'):
			ch = __T('\b');
			break;
		case __T('f'):
			ch = __T('\f');
			break;
		case __T('n'):
			ch = __T('\n');
			break;
		case __T('r'):
			ch = __T('\r');
			break;
		case __T('t'):
			ch = __T('\t');
			break;
		case __T('u'):
			//Unsupported flag.
			break;
		}
		return ch;
	}

	HRESULT JsonReader::getString(TCHAR *buffer, UINT limit)
	{
		UINT count;
		TCHAR ch;
		
		count = 0;
		while(!feof(fp))
		{
			ch = _fgettc(fp);
			if(ch == __T('\"')) {
				*(buffer + count) = '\0';
				break;
			}
			if(count < limit - 1) {
				if(ch == __T('\\')) {
					if(!feof(fp)) {
						ch = _fgettc(fp);
						ch = transformCharacter(ch);
					} else {
						return E_FAIL;
						break;
					}
				}
				*(buffer + count++) = ch;
			}
		}
		return S_OK;
	}

	HRESULT JsonReader::parse(void)
	{
		std::stack<tstring> object_stack;
		TCHAR buffer[MAX_STRING_BUFFER_LENGTH], value_buffer[MAX_STRING_BUFFER_LENGTH], ch;
		UINT count;
		bool error = false;
		double number;

		if(listener == NULL)
			return E_FAIL;

		fseek(fp, 0, SEEK_SET);
		while(!feof(fp) && ((ch = _fgettc(fp)) != __T('{'))) ;
		object_stack.push(JSON_ROOT_OBJECT_NAME);
		listener->enterObject(JSON_ROOT_OBJECT_NAME);

		while(!feof(fp))
		{
			if(object_stack.empty()) break;
			ch = _fgettc(fp);
			switch(ch)
			{
			case __T('}'):
				listener->quitObject(object_stack.top().c_str());
				object_stack.pop();
				break;
			case __T('\"'):
				if(FAILED(getString(buffer, MAX_STRING_BUFFER_LENGTH))) {
					error = true;
					break;
				}
				while(!feof(fp) && ((ch = _fgettc(fp)) != __T(':'))) ;
				while(!feof(fp)){
					ch = _fgettc(fp);
					if(ch == __T(' ') || ch == __T('\r') || ch == __T('\n') || ch == __T('\t')) continue;
					else break;
				}
				switch(ch)
				{
				case __T('{'):
					object_stack.push(buffer);
					listener->newObject(buffer);
					listener->enterObject(buffer);
					break;
				case __T('t'):	// true
					listener->newBool(buffer, true);
					break;
				case __T('f'):	// false
					listener->newBool(buffer, false);
					break;
				case __T('n'):	// null
					// regarded as comments
					break;
				case __T('\"'):	// string
					if(FAILED(getString(value_buffer, MAX_STRING_BUFFER_LENGTH))){
						error = true;
						break;
					}
					listener->newString(buffer, value_buffer);
					break;
				default:	// numbers
					count = 0;
					number = 0.0f;
					value_buffer[count++] = ch;
					while(!feof(fp)) {
						value_buffer[count] = fgetc(fp);
						if(value_buffer[count] == __T('\n')) {
							fseek(fp, -1, SEEK_CUR);
							value_buffer[count++] = '\0';
							break;
						}
						count++;
					}
					number = _tstof(value_buffer);
					listener->newNumber(buffer, number);
				}
				break;
			}
			if(error) break;
		}

		return (object_stack.empty() ? S_OK : E_FAIL);
	}

};
