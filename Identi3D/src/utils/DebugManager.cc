//
// File: DebugManager.cc
// =====================
// 

#include <src/utils/DebugManager.h>
#include <DbgHelp.h>
#include <iostream>

namespace Identi3D {
	
	const char	*month_name[12]	= { "Jan", "Feb", "Mar", 
									"Apr", "May", "Jun", 
									"Jul", "Aug", "Sept", 
									"Oct", "Nov", "Dec" };

	DebugManager::~DebugManager(void)
	{
		try
		{
			if(!_prevbuf) {
				std::clog.rdbuf(_prevbuf);
				_prevbuf = NULL;
			}
			_log.close();
			_console.close();
		} catch(...) {
			// Abandon all data trying avoid crashes.
		}

		return ;
	}

	const std::string DebugManager::getTimeStamp(void) const
	{
		const int max_buffer_length = 64;

		time_t rawtime;
		struct tm tmetrics;

		time(&rawtime);
		localtime_s(&tmetrics, &rawtime);						// Get local time

		char temp[max_buffer_length];

		_snprintf_s(temp, max_buffer_length, "%s. %d %02d:%02d ", 
			month_name[tmetrics.tm_mon], tmetrics.tm_mday, tmetrics.tm_hour, tmetrics.tm_min);

		return std::string(temp);
	}

	const std::string DebugManager::getFormattedSourcePath(const char *src_path, int line_number) const
	{
		const int max_buffer_length = 256;
		
		std::string path(src_path);
		char temp[max_buffer_length];

		_snprintf_s(temp, max_buffer_length, "[%s:%d] ", 
			path.substr(path.find_last_of('\\')).c_str(), line_number);

		return std::string(temp);
	}

	void DebugManager::printRawString(const char *str, ...)
	{
		const int max_buffer_length = 512;

		char msg[max_buffer_length];
		va_list arg;

		va_start(arg, str);
		_vsnprintf_s(msg, max_buffer_length, str, arg);
		va_end(arg);

		if(_flag & DebugFlag_ConsoleOutput)
			std::clog << msg;

		if(_flag & DebugFlag_FileOutput) {
			if(!_log.is_open())
				if(!setOutputFileName()) return ;

			_log << msg;
		}
	}

	bool DebugManager::print(const char *src_path, 
							 int line_number, 
							 const std::exception &e)
	{
		if(src_path == NULL) return false;

		try 
		{
			std::string msg;
			
			if(_flag & DebugFlag_AttachTime) 
				msg += getTimeStamp();
			if(_flag & DebugFlag_AttachSource) 
				msg += getFormattedSourcePath(src_path, line_number);

			msg += std::string(e.what());

			if(_flag & DebugFlag_ConsoleOutput)
				std::clog << msg << std::endl;

			if(_flag & DebugFlag_FileOutput) {
				if(!_log.is_open())
					if(!setOutputFileName()) return false;

				_log << msg << std::endl;
			}
		} catch(...) {
			return false;
		}

		return true;
	}

	bool DebugManager::print(const char *src_path, 
							 int line_number, 
							 bool verbose, 
							 const char *message, ...)
	{
		if(src_path == NULL) return false;

		const int max_buffer_length = 512;

		char format[max_buffer_length];
		va_list arg;

		try
		{
			va_start(arg, message);
			_vsnprintf_s(format, max_buffer_length, message, arg);
			va_end(arg);

			std::string msg;

			if(_flag & DebugFlag_AttachTime) 
				msg += getTimeStamp();
			if(_flag & DebugFlag_AttachSource) 
				msg += getFormattedSourcePath(src_path, line_number);

			msg += std::string(format);
			
			if(_flag & DebugFlag_ConsoleOutput)
				std::clog << msg << std::endl;

			if(_flag & DebugFlag_FileOutput) {
				if(!_log.is_open())
					if(!setOutputFileName()) return false;

				_log << msg << std::endl;
			}
		} catch(...) {
			return false;
		}

		return true;
	}



//	HRESULT DebugManager::print(const char *src_filename, 
//								int line_number, 
//								bool verbose,
//								const TCHAR *message,
//								...)
//	{
//		TCHAR	buffer[max_buffer_length], 
//				tmp_buffer[max_buffer_length],
//				msg_buffer[max_buffer_length],
//				source_fname[max_buffer_length];
//		time_t	rawtime;
//		struct	tm tmetrics;
//		va_list arg;
//		size_t	s;
//		
//		if(src_filename == NULL || message == NULL) 
//			return E_FAIL;
//		if((output_flag & DebugFlag_VerboseMode) == 0 && verbose) 
//			return S_FALSE;
//
//		va_start(arg, message);
//		_vsntprintf_s(msg_buffer, max_buffer_length, message, arg);
//		va_end(arg);
//
//		memset(buffer, 0, sizeof(buffer));
//		_tcsncpy_s(buffer, perfix_mark, max_buffer_length - 1);
//
//		if(output_flag & DebugFlag_AttachTime) {					// Attach a timestamp.
//			time(&rawtime);
//			localtime_s(&tmetrics, &rawtime);						// Get local time
//
//			_sntprintf_s(tmp_buffer, max_buffer_length, time_format, month_name[tmetrics.tm_mon], 
//				tmetrics.tm_mday, tmetrics.tm_hour, tmetrics.tm_min);
//			connectString(buffer, tmp_buffer);
//
//			if(output_flag & DebugFlag_AttachSource) {
//				connectString(buffer, info_seperator);
//			}
//		}
//
//		if(output_flag & DebugFlag_AttachSource){					// Attach source path & line.
//
//#if defined(_UNICODE)
//			mbstowcs_s(&s, source_fname, src_filename, max_buffer_length);
//			if(s == 0) return E_FAIL;
//#else
//			strncpy(source_fname, src_filename, max_buffer_length);
//#endif
//			truncSourceFileName(source_fname);
//			_sntprintf_s(tmp_buffer, max_buffer_length, source_format, source_fname, line_number);
//			connectString(buffer, tmp_buffer);
//		}
//
//		connectString(buffer, suffix_mark);
//		connectString(buffer, msg_buffer);
//		connectString(buffer, __T("\n"));
//
//		if(output_flag & DebugFlag_ConsoleOutput && console != NULL)	// Write to console window (if any).
//			_fputts(buffer, console);
//		
//		if(output_flag & DebugFlag_FileOutput){						// Write to log file.
//			if(log_file == NULL){
//				_tfopen_s(&log_file, DEBUGMANAGER_DEFAULT_FILENAME, __T("w"));
//				if(log_file == NULL){
//					_ftprintf(stderr, E_FILE_OPEN_FAILURE, DEBUGMANAGER_DEFAULT_FILENAME);
//					output_flag &= ~DebugFlag_FileOutput;
//				}
//				_fputts(I_LOG_STARTED, log_file);
//			}
//
//			_fputts(buffer, log_file);
//		}
//
//		return S_OK;
//	}

	bool DebugManager::setOutputFileName(const std::string &new_filename)
	{
		try
		{
			if(_log.is_open()) _log.close();

			_log.open(new_filename);
			if(!_log.is_open()) {
				std::clog << "Warning: Failed to open log file \"" 
					<< new_filename << "\"!" << std::endl;
				_flag &= ~DebugFlag_FileOutput;
				return false;
			}
			_log << DEBUGMANAGER_WELCOMEMESSAGE;
		} catch(...) {
			return false;
		}

		return true;
	}

	bool DebugManager::createDebugConsole(void)
	{
		try
		{
			if(_console.is_open()) return false;

			if(AllocConsole()) {
				// Set console window style
				SetConsoleTitle(IDENTI3D_NAME);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);

				// Redirect file stream.
				_console.open("CONOUT$");
				if(!_console.is_open()) return false;
				_prevbuf = std::clog.rdbuf(_console.rdbuf());
			}
		} catch(...) {
			return false;
		}

		return true;
	}

	void DebugManager::dumpCallStack(void)
	{
		const UINT max_name_length = 256;

		CONTEXT context;
		STACKFRAME64 stackframe;
		HANDLE process, thread;
		PSYMBOL_INFO symbol = NULL;
		IMAGEHLP_LINE64 source_info;
		DWORD displacement;
		size_t bufsize;
		bool lib_init = false;

		try
		{
			// Initialize symbol info structure.
			bufsize = sizeof(SYMBOL_INFO) + (max_name_length - 1) * sizeof(char);
			symbol = static_cast<PSYMBOL_INFO>(operator new(bufsize));
			memset(symbol, 0, bufsize);
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = max_name_length;

			// Initialize source info structure.
			memset(&source_info, 0, sizeof(IMAGEHLP_LINE64));
			source_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
	
			// Setup capture registers.
			RtlCaptureContext(&context);
			memset(&stackframe, 0, sizeof(STACKFRAME64));
			stackframe.AddrPC.Offset = context.Eip;
			stackframe.AddrPC.Mode = AddrModeFlat;
			stackframe.AddrStack.Offset = context.Esp;
			stackframe.AddrStack.Mode = AddrModeFlat;
			stackframe.AddrFrame.Offset = context.Ebp;
			stackframe.AddrFrame.Mode = AddrModeFlat;

			// Get handles.
			process = GetCurrentProcess();
			thread = GetCurrentThread();

			// Initialize dbghelper library.
			if(!SymInitialize(process, NULL, TRUE))
				return ;
			lib_init = true;

			printRawString("\nDumping call stack...\n");

			// Enumerate each frame inside the stack.
			while(StackWalk64(IMAGE_FILE_MACHINE_I386, process, thread, &stackframe, 
				&context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
			{
				// Bottom reached.
				if(stackframe.AddrFrame.Offset == 0)
					break;
		
				// Get symbol name.
				if(SymFromAddr(process, stackframe.AddrPC.Offset, NULL, symbol))
					printRawString(" \"%s\" ", symbol->Name);

				// Get source code info & function address.
				if(SymGetLineFromAddr64(process, stackframe.AddrPC.Offset, 
					&displacement, &source_info)) {
						printRawString(" [%s:%d] @ 0x%08LX\n", 
							source_info.FileName, 
							source_info.LineNumber,
							stackframe.AddrPC.Offset);
				} else {
					if(GetLastError() == 0x1E7) {
						printRawString(": No debug symbol loaded.");
					}
				}
			}
			
			lib_init = false;
			SymCleanup(process);
		} catch(std::exception &e) {
			print(__FILE__, __LINE__, e);
			if(lib_init) SymCleanup(process);
		} catch(...) {
			if(lib_init) SymCleanup(process);
		}
		operator delete(symbol);
		return ;
	}


};