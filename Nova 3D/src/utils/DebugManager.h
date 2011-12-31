//
// File: DebugManager.h
// ====================
//

#ifndef NOVA3D_SRC_UTILS_DEBUGMANAGER_H
#define NOVA3D_SRC_UTILS_DEBUGMANAGER_H

#include "src/nova-3d/NovaGeneral.h"

namespace Nova3D {

	// Assistant Macro Function
	#if !defined(_DEBUGPRINT)
	 #define _DEBUGPRINT(DBGMGR, FMT, ...) \
		(DBGMGR != NULL && (static_cast<DebugManager *>(DBGMGR)->print(__FILE__, __LINE__, FMT, __VA_ARGS__)))
	#endif

	enum DebugFlag
	{
		DebugFlag_ConsoleOutput	= 0x0001,	// Write to console.
		DebugFlag_FileOutput	= 0x0002,	// Write to log file.

		DebugFlag_AttachSource	= 0x0010,	// Attach source line to outputs.
		DebugFlag_AttachTime	= 0x0020,	// Attach timestamp to outputs.

		DebugFlag_ForceInt		= 65535
	};

	#define DEBUGFLAG_DEFAULT		(DebugFlag_FileOutput	| \
									 DebugFlag_AttachSource | \
									 DebugFlag_AttachTime)					// Default flags.

	#define DEBUG_DEFAULT_FILENAME	__T("nova.log")

	class DebugManager
	{
	private:
		FILE	*log_file, *console;
		DWORD	output_flag;

	public:
		DebugManager(void);
		~DebugManager(void);

		HRESULT print(const char *src_filename, int line_number, const TCHAR *message, ...);

		HRESULT setOutputFileName(const TCHAR *new_filename = DEBUG_DEFAULT_FILENAME);
		HRESULT createDebugConsole(void);
			
		inline void	setOutputFlag(DWORD new_flag = DEBUGFLAG_DEFAULT) 
			{ output_flag = new_flag; }
		inline const DWORD	getOutputFlag(void) const 
			{ return output_flag; }
	};

};

#endif
