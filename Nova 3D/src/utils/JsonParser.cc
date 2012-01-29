//
// File: JsonParser.cc
// ===================
//

#include <src/utils/JsonParser.h>

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
		if(file_name == NULL)
			return E_FAIL;

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

	JsonWriter::JsonWriter(void)
	{
		fp = NULL;
		layer = 0;
	}

	JsonWriter::~JsonWriter(void)
	{
		unlockFile();
	}

	HRESULT JsonWriter::lockFile(const TCHAR *file_name)
	{
		if(file_name == NULL)
			return E_FAIL;

		_tfopen_s(&fp, file_name, __T("w"));
		if(fp == NULL)
			return E_FAIL;

		_fputts(__T("{\n"), fp);
		layer = 1;
		item_count = 0;

		return S_OK;
	}

	HRESULT JsonWriter::unlockFile(void)
	{
		if(fp != NULL) {
			encloseAllObjects();
			fclose(fp);
		}
		return S_OK;
	}

	void JsonWriter::parseString(const TCHAR *str)
	{
		if(str == NULL) return ;
		const TCHAR *p = str;
		_fputtc(__T('\"'), fp);
		while(*p != __T('\0'))
		{
			switch(*p)
			{
			case __T('"'): _fputts(__T("\\\""), fp); break;
			case __T('\\'): _fputts(__T("\\\\"), fp); break;
			case __T('/'): _fputts(__T("\\/"), fp); break;
			case __T('\b'): _fputts(__T("\\b"), fp); break;
			case __T('\f'): _fputts(__T("\\f"), fp); break;
			case __T('\n'): _fputts(__T("\\n"), fp); break;
			case __T('\r'): _fputts(__T("\\r"), fp); break;
			case __T('\t'): _fputts(__T("\\t"), fp); break;
			default: _fputtc(*p, fp);
			}
			p++;
		}
		_fputtc(__T('\"'), fp);
	}
	HRESULT JsonWriter::writeBool(const TCHAR *name, const bool value)
	{
		if(fp == NULL || name == NULL) return E_FAIL;
		doIndent();
		parseString(name);
		_ftprintf_s(fp, __T(" : %s,\n"), (value ? __T("true") : __T("false")));
		item_count++;
		return S_OK;
	}

	HRESULT JsonWriter::writeNumber(const TCHAR *name, const double value, bool forceInt)
	{
		if(fp == NULL || name == NULL) return E_FAIL;
		doIndent();
		parseString(name);
		if(forceInt) {
			_ftprintf_s(fp, __T(" : %ld,\n"), static_cast<long>(value));
		}else{
			_ftprintf_s(fp, __T(" : %E,\n"), value);
		}
		item_count++;
		return S_OK;
	}

	HRESULT JsonWriter::writeObject(const TCHAR *name)
	{
		if(fp == NULL || name == NULL) return E_FAIL;
		doIndent();
		parseString(name);
		_ftprintf_s(fp, __T(" :\n"), name);
		doIndent();
		_ftprintf_s(fp, __T("{\n"));
		item_count++;
		layer++;
		return S_OK;
	}

	HRESULT JsonWriter::encloseObject()
	{
		if(fp == NULL) return E_FAIL;
		if(layer == 0) return E_FAIL;
		layer--;
		doIndent();
		_ftprintf_s(fp, __T("},\n"));
		return S_OK;
	}

	HRESULT JsonWriter::encloseAllObjects()
	{
		if(fp == NULL) return E_FAIL;
		while(layer != 0)
		{
			layer--;
			doIndent();
			_ftprintf_s(fp, __T("}\n"));
		}
		return S_OK;
	}

	HRESULT JsonWriter::writeString(const TCHAR *name, const TCHAR *value)
	{
		if(fp == NULL || name == NULL || value == NULL) return E_FAIL;
		doIndent();
		parseString(name);
		_ftprintf_s(fp, __T(" : "));
		parseString(value);
		_ftprintf_s(fp, __T(",\n"));
		item_count++;
		return S_OK;
	}

	HRESULT JsonWriter::writeComment(const TCHAR *comment)
	{
		if(fp == NULL || comment == NULL) return E_FAIL;
		doIndent();
		parseString(comment);
		_ftprintf_s(fp, __T(" : null,\n"));
		item_count++;
		return S_OK;
	}

	void JsonWriter::doIndent(void)
	{
		for(unsigned int i = 0; i < layer; i++)
			_fputts(__T("\t"), fp);
	}

};
