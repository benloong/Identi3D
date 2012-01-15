//
// File: DebugManager.h
// ====================
// Provides a debug system for Nova 3D.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_SRC_UTILS_DEBUGMANAGER_H
#define NOVA3D_SRC_UTILS_DEBUGMANAGER_H

#include "src/nova-3d/NovaGeneral.h"
#include "src/utils/Singleton.h"

#ifndef DBGHELP_TRANSLATE_TCHAR
 #define DBGHELP_TRANSLATE_TCHAR
#endif

namespace Nova3D {

	// 
	// Macro Function: _DebugPrint(Format, ...)
	// ========================================
	// Prints non-verbose debug messages according to the Format via global DebugManager.
	// Do not call this function in DLL.
	//
	#if !defined(_DebugPrint)
	 #define _DebugPrint(_Format, ...) \
		(DebugManager::getInstance().print(__FILE__, __LINE__, false, _Format, __VA_ARGS__))
	#endif
	
	// 
	// Macro Function: _DebugPrintV(Format, ...)
	// =========================================
	// Prints verbose debug messages according to the Format via global DebugManager.
	// Do not call this function in DLL.
	//
	#if !defined(_DebugPrintV)
	 #define _DebugPrintV(_Format, ...) \
		(DebugManager::getInstance().print(__FILE__, __LINE__, true, _Format, __VA_ARGS__))
	#endif
	
	// 
	// Macro Function: _DebugPrintS(DebugManager, Format, ...)
	// =======================================================
	// Prints non-verbose debug messages according to the Format via DebugManager specified.
	//
	#if !defined(_DebugPrintS)
	 #define _DebugPrintS(DebugMgr, _Format, ...) \
		((reinterpret_cast<DebugManager *>(DebugMgr))->print(__FILE__, __LINE__, false, _Format, __VA_ARGS__))
	#endif
	
	// 
	// Macro Function: _DebugPrintSV(DebugManager, Format, ...)
	// ========================================================
	// Prints verbose debug messages according to the Format via DebugManager specified.
	//
	#if !defined(_DebugPrintSV)
	 #define _DebugPrintSV(DebugMgr, _Format, ...) \
		((reinterpret_cast<DebugManager *>(DebugMgr))->print(__FILE__, __LINE__, true, _Format, __VA_ARGS__))
	#endif

	//
	// Enum: DebugFlag
	// ===============
	// Defines several flags used in debugging output.
	// 
	// Values:
	//	DebugFlag_ConsoleOutput: Writes messages to console window.
	//	DebugFlag_FileOutput: Writes messages to log file.
	//	DebugFlag_AttachSource: Adds source line info to messages.
	//	DebugFlag_AttachTime: Adds time stamp to messages.
	//	DebugFlag_VerboseMode: Prints verbose message.
	//	DebugFlag_ForceDword: Not used. Force the type DebugFlag to be compiled in size of DWORD.
	//
	enum DebugFlag
	{
		DebugFlag_ConsoleOutput	= 0x0001,
		DebugFlag_FileOutput	= 0x0002,

		DebugFlag_AttachSource	= 0x0010,
		DebugFlag_AttachTime	= 0x0020,

		DebugFlag_VerboseMode	= 0x0100,

		DebugFlag_ForceDword	= 0xFFFFFFFF
	};

	//
	// Macro: DEBUGFLAG_DEFAULT
	// ========================
	// A set of default debug flags.
	//
	#define DEBUGFLAG_DEFAULT		(DebugFlag_FileOutput	| \
									 DebugFlag_AttachSource | \
									 DebugFlag_VerboseMode	| \
									 DebugFlag_AttachTime)					// Default flags.

	//
	// Macro: DEBUG_DEFAULT_FILENAME
	// =============================
	// The default log file global DebugManager uses.
	//
	#define DEBUG_DEFAULT_FILENAME	__T("nova.log")

	//
	// Class: DebugManager <Inherits from Singleton>
	// =============================================
	// Declares a global DebugManager used in Nova 3D.
	//
	class DebugManager : public Singleton<DebugManager>
	{
	public:

		//
		// Function: DebugManager(void)
		// ============================
		// Constructor of the class, initializes variables.
		//
		DebugManager(void);

		//
		// Function: ~DebugManager(void)
		// =============================
		// Destructor of the class, closes log file and do some cleaning.
		//
		~DebugManager(void);

		//
		// Function: print(SourceFileName, LineNumber, isVerboseMessage, Message, ...)
		// ===========================================================================
		// Prints debugging messages according to parameters.
		//
		HRESULT print(const char *src_filename, int line_number, bool verbose, const TCHAR *message, ...);

		//
		// Function: setOutputFileName(NewFileName)
		// ========================================
		// Sets log file to NewFileName
		//
		HRESULT setOutputFileName(const TCHAR *new_filename = DEBUG_DEFAULT_FILENAME);

		//
		// Function: createDebugConsole(void)
		// ==================================
		// Creates a new console window.
		//
		HRESULT createDebugConsole(void);
		
		//
		// Function: dumpCallStack(void)
		// =============================
		// Prints call stack.
		//
		HRESULT dumpCallStack(void);

		//
		// Inline Function: setOutputFlag(NewFlag)
		// =======================================
		// Sets output flag to NewFlag.
		//
		inline void	setOutputFlag(DWORD new_flag = DEBUGFLAG_DEFAULT) 
			{ output_flag = new_flag; }

		//
		// Inline Function: getOutputFlag(void)
		// ====================================
		// Get output flag.
		//
		inline const DWORD	getOutputFlag(void) const 
			{ return output_flag; }

	private:
		FILE	*log_file, *console;
		DWORD	output_flag;
	};

};

#endif
