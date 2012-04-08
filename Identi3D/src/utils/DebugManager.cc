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

	size_t DebugManager::_allocated_memory = 0;
	bool DebugManager::_is_valid = false;

	DebugManager::~DebugManager(void)
	{
		try
		{
#if defined(_MEMORY_LEAK_DETECTION)
			if(_allocated_memory) {
				printRawString(" [WARNING] Memory leaks detected before DebugManager freed. Orphan size: %d\n",
					_allocated_memory);
			} else {
				printRawString(" No memory leaks detected.\n");
			}
#endif // defined(_MEMORY_LEAK_DETECTION)

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
		
		try
		{
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
		} catch(...) {} ;
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
		bool retval;
		va_list arg;

		try
		{
			va_start(arg, message);
			retval = print(src_path, line_number, verbose, message, arg);
			va_end(arg);
		} catch(...) {
			return false;
		}

		return true;
	}

	bool DebugManager::print(const char *src_path, int line_number, bool verbose, const char *message, va_list &arg)
	{
		const int max_buffer_length = 512;
		char format[max_buffer_length];
		
		if(src_path == NULL) return false;

		try
		{
			_vsnprintf_s(format, max_buffer_length, message, arg);

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
		HANDLE process = 0, thread = 0;
		PSYMBOL_INFO symbol = NULL;
		IMAGEHLP_LINE64 source_info;
		DWORD displacement;
		size_t bufsize;
		bool lib_init = false;

		try
		{
			// Initialize symbol info structure.
			bufsize = sizeof(SYMBOL_INFO) + (max_name_length - 1) * sizeof(char);
			symbol = static_cast<PSYMBOL_INFO>(::operator new(bufsize));
			if(symbol == NULL) return ;
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
						printRawString(": No debug symbol loaded.\n");
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
		::operator delete(symbol);
		return ;
	}

	void DebugManager::onAllocation(size_t size)
	{
		_allocated_memory += size;
	}

	void DebugManager::onDeallocation(size_t size)
	{
		_allocated_memory -= size;
	}

	void DebugManager::printMemoryStatus(void)
	{
		printRawString("\tMEMORY STATUS: %d byte(s) allocated.\n", _allocated_memory);
	}

	bool DebugManager::isValid(void)
	{
		return _is_valid;
	}

	void *DebugManager::operator new(size_t size)
	{
		// Only one instance allowed.
		if(_is_valid) return NULL;

		void *p = ::operator new(size, std::nothrow);
		if(p != NULL) {
			_is_valid = true;
		}
		return p;
	}

	void DebugManager::operator delete(void *p)
	{
		_is_valid = false;
		::operator delete(p, std::nothrow);
	}

	DebugFrame::DebugFrame(DebugManager *debugger, const char *class_sign)
	{
		_debugger = debugger;
		if(debugger) {
			debugger->printRawString("=== object [%s] initialized ===\n", class_sign);
		}
		_sign = class_sign;
	}

	DebugFrame::~DebugFrame(void)
	{
		if(_debugger != NULL && DebugManager::isValid()) {
			_debugger->printRawString("=== object [%s] freed ===\n", _sign.c_str());
		}
	}

	void *DebugFrame::operator new(size_t size)
	{
		void *p = ::operator new(size, std::nothrow);
		if(p != NULL) {
			DebugManager::onAllocation(size);
		}
		return p;
	}

	void *DebugFrame::operator new[](size_t size)
	{
		void *p = ::operator new[](size, std::nothrow);
		if(p != NULL) {
			DebugManager::onAllocation(size);
		}
		return p;
	}

	void DebugFrame::operator delete(void *p, size_t size)
	{
		DebugManager::onDeallocation(size);
		::operator delete(p, std::nothrow);
	}

	void DebugFrame::operator delete[](void *p, size_t size)
	{
		DebugManager::onDeallocation(size);
		::operator delete[](p, std::nothrow);
	}

	void DebugFrame::setDebugManager(DebugManager *debugger)
	{
		_debugger = debugger;
	}

	bool DebugFrame::_printException(const char *src_path, int line_number, const std::exception &e) const
	{
		if(_debugger != NULL && DebugManager::isValid()) {
			return _debugger->print(src_path, line_number, e);
		}
		return false;
	}

	bool DebugFrame::_printVerboseMessage(const char *src_path, int line_number, const char *message, ...) const
	{
		if(_debugger != NULL && DebugManager::isValid()) {
			va_list arg;
			bool retval;

			va_start(arg, message);
			retval = _debugger->print(src_path, line_number, true, message, arg);
			va_end(arg);
			return retval;
		}
		return false;
	}

	bool DebugFrame::_printMessage(const char *src_path, int line_number, const char *message, ...) const
	{
		if(_debugger != NULL && DebugManager::isValid()) {
			va_list arg;
			bool retval;

			va_start(arg, message);
			retval = _debugger->print(src_path, line_number, false, message, arg);
			va_end(arg);
			return retval;
		}
		return false;
	}

};