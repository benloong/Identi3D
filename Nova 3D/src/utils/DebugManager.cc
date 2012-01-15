//
// File: DebugManager.cc
// =====================
// 

#include "src/utils/DebugManager.h"

#include <DbgHelp.h>

namespace Nova3D {
	
	const TCHAR	*month_name[12]	= { __T("Jan."), __T("Feb."), __T("Mar."), 
									__T("Apr."), __T("May."), __T("Jun."), 
									__T("Jul."), __T("Aug."), __T("Sept."), 
									__T("Oct."), __T("Nov."), __T("Dec.") };
	const TCHAR	*perfix_mark	= __T("[");
	const TCHAR	*suffix_mark	= __T("] ");
	const TCHAR	*time_format	= __T("%s %d %d:%d");
	const TCHAR	*info_seperator	= __T(" @ ");
	const TCHAR	*source_format	= __T("%s:%d");

	const UINT max_buffer_length = 256;
	
	DebugManager::DebugManager(void)
	{
		log_file = NULL;
		console = NULL;
		output_flag = DEBUGFLAG_DEFAULT;
	}

	DebugManager::~DebugManager(void)
	{
		if(log_file != NULL) fclose(log_file);
		if(console != NULL) fclose(console);
	}

	inline void connectString(TCHAR *dest, const TCHAR *src)
	{
		if(dest == NULL && src == NULL) return ;
		TCHAR *p = dest;
		const TCHAR *s = src;
		while(*p != __T('\0')) p++;
		while(*s != __T('\0') && p - dest + 1 < max_buffer_length) *(p++) = *(s++);
		*p = '\0';
	}

	inline void truncSourceFileName(TCHAR *src_filename)
	{
		if(src_filename == NULL) return ;
		TCHAR *p = src_filename + _tcslen(src_filename) - 1, *s = src_filename;
		while(p != src_filename && *p != __T('/') && *p != __T('\\')) p--;
		if(p == src_filename) return ;
		while(p - src_filename < static_cast<int>(_tcslen(src_filename))) *(s++) = *(p++);
		*s = '\0';
		return ;
	}

	HRESULT DebugManager::print(const char *src_filename, 
								int line_number, 
								bool verbose,
								const TCHAR *message,
								...)
	{
		TCHAR	buffer[max_buffer_length], 
				tmp_buffer[max_buffer_length],
				msg_buffer[max_buffer_length],
				source_fname[max_buffer_length];
		time_t	rawtime;
		struct	tm tmetrics;
		va_list arg;
		size_t	s;
		
		if(src_filename == NULL || message == NULL) 
			return E_FAIL;
		if((output_flag & DebugFlag_VerboseMode) == 0 && verbose) 
			return S_FALSE;

		va_start(arg, message);
		_vsntprintf_s(msg_buffer, max_buffer_length, message, arg);
		va_end(arg);

		memset(buffer, 0, sizeof(buffer));
		_tcsncpy_s(buffer, perfix_mark, max_buffer_length - 1);

		if(output_flag & DebugFlag_AttachTime) {					// Attach a timestamp.
			time(&rawtime);
			localtime_s(&tmetrics, &rawtime);						// Get local time

			_sntprintf_s(tmp_buffer, max_buffer_length, time_format, month_name[tmetrics.tm_mon], 
				tmetrics.tm_mday, tmetrics.tm_hour, tmetrics.tm_min);
			connectString(buffer, tmp_buffer);

			if(output_flag & DebugFlag_AttachSource) {
				connectString(buffer, info_seperator);
			}
		}

		if(output_flag & DebugFlag_AttachSource){					// Attach source path & line.

#if defined(_UNICODE)
			mbstowcs_s(&s, source_fname, src_filename, max_buffer_length);
			if(s == 0) return E_FAIL;
#else
			strncpy(source_fname, src_filename, max_buffer_length);
#endif
			truncSourceFileName(source_fname);
			_sntprintf_s(tmp_buffer, max_buffer_length, source_format, source_fname, line_number);
			connectString(buffer, tmp_buffer);
		}

		connectString(buffer, suffix_mark);
		connectString(buffer, msg_buffer);
		connectString(buffer, __T("\n"));

		if(output_flag & DebugFlag_ConsoleOutput && console != NULL)	// Write to console window (if any).
			_fputts(buffer, console);
		
		if(output_flag & DebugFlag_FileOutput){						// Write to log file.
			if(log_file == NULL){
				_tfopen_s(&log_file, DEBUG_DEFAULT_FILENAME, __T("w"));
				if(log_file == NULL){
					_ftprintf(stderr, E_FILE_OPEN_FAILURE, DEBUG_DEFAULT_FILENAME);
					output_flag &= ~DebugFlag_FileOutput;
				}
				_fputts(I_LOG_STARTED, log_file);
			}

			_fputts(buffer, log_file);
		}

		return S_OK;
	}

	HRESULT DebugManager::setOutputFileName(const TCHAR *new_filename)
	{
		if(new_filename == NULL) 
			return E_FAIL;

		if(log_file) fclose(log_file);
		_tfopen_s(&log_file, new_filename, __T("w"));
		if(log_file == NULL) {
			_fputts(__T("warning: failed to open log file for writing!"), stderr);
			output_flag &= ~DebugFlag_FileOutput;
			return E_FAIL;
		}
		_fputts(__T("Nova3D debug system started.\n\n"), log_file);

		return S_OK;
	}

	HRESULT DebugManager::createDebugConsole(void)
	{
		if(console) 
			return S_FALSE;
		if(AllocConsole()){												// Allocate a console window under Windows.
			_tfreopen_s(&console, __T("CONOUT$"), __T("wt"), stderr);	// Redirect standard output to console window.
			SetConsoleTitle(NOVA3D_NAME);								// Set title.
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
				FOREGROUND_INTENSITY);									// Set text color to gray.
		}
		return S_OK;
	}

	HRESULT DebugManager::dumpCallStack(void)
	{
		const UINT max_name_length = 256;

		CONTEXT context;
		STACKFRAME64 stackframe;
		HANDLE process, thread;
		PSYMBOL_INFO symbol;
		IMAGEHLP_LINE64 source_info;
		DWORD displacement;

		symbol = (PSYMBOL_INFO)malloc(sizeof(SYMBOL_INFO) + (max_name_length - 1) * sizeof(TCHAR));
		memset(symbol, 0, sizeof(SYMBOL_INFO) + (max_name_length - 1) * sizeof(TCHAR));
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = max_name_length;

		memset(&source_info, 0, sizeof(IMAGEHLP_LINE64));
		source_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
	
		RtlCaptureContext(&context);
		memset(&stackframe, 0, sizeof(STACKFRAME64));
		stackframe.AddrPC.Offset = context.Eip;
		stackframe.AddrPC.Mode = AddrModeFlat;
		stackframe.AddrStack.Offset = context.Esp;
		stackframe.AddrStack.Mode = AddrModeFlat;
		stackframe.AddrFrame.Offset = context.Ebp;
		stackframe.AddrFrame.Mode = AddrModeFlat;

		process = GetCurrentProcess();
		thread = GetCurrentThread();

		if(!SymInitialize(process, NULL, TRUE))
			return E_FAIL;

		print(__FILE__, __LINE__, false, __T("Dumping call stack..."));

		while(StackWalk64(IMAGE_FILE_MACHINE_I386, process, thread, &stackframe, 
			&context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
		{
			if(stackframe.AddrFrame.Offset == 0)
				break;
		
			if(SymFromAddr(process, stackframe.AddrPC.Offset, NULL, symbol))
				print(__FILE__, __LINE__, false, __T("  Func %s"), symbol->Name);

			if(SymGetLineFromAddr64(process, stackframe.AddrPC.Offset, 
				&displacement, &source_info)) {
					print(__FILE__, __LINE__, false, __T("    [%s:%d] at addr 0x%08LX"), 
						source_info.FileName, 
						source_info.LineNumber,
						stackframe.AddrPC.Offset);
			} else {
				if(GetLastError() == 0x1E7) {
					print(__FILE__, __LINE__, false, __T("    No debug symbol loaded for this function."));
				}
			}
		}

		SymCleanup(process);
		free(symbol);
		return S_OK;
	}


};