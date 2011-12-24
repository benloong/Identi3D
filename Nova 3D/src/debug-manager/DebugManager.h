//
// File: DebugManager.h
// ====================
//

#ifndef NOVA3D_SRC_DEBUGMANAGER_DEBUGMANAGER_H
#define NOVA3D_SRC_DEBUGMANAGER_DEBUGMANAGER_H

#include "src/nova-3d/NovaGeneral.h"

namespace Nova3D {

	// Assistant Macro Function
	#if !defined(_ERROR)
	 #define _ERROR(PDBGMGR, FMT, ...) \
		(PDBGMGR && (((DebugManager *)PDBGMGR)->write(__FILE__, __LINE__, AlertLevel_Error, FMT, __VA_ARGS__)))
	#endif

	#if !defined(_WARNING)
	 #define _WARNING(PDBGMGR, FMT, ...) \
		(PDBGMGR && (((DebugManager *)PDBGMGR)->write(__FILE__, __LINE__, AlertLevel_Warning, FMT, __VA_ARGS__)))
	#endif

	#if !defined(_INFO)
	 #define _INFO(PDBGMGR, FMT, ...) \
		(PDBGMGR && (((DebugManager *)PDBGMGR)->write(__FILE__, __LINE__, AlertLevel_Info, FMT, __VA_ARGS__)))
	#endif

	enum DebugFlag
	{
		DebugFlag_ConsoleOutput	= 0x0001,	// Write to console.
		DebugFlag_FileOutput	= 0x0002,	// Write to log file.
		DebugFlag_PromptAtError	= 0x0004,	// Show an message box on errors.

		DebugFlag_OutputInfo	= 0x0008,	// Includes info.
		DebugFlag_OutputWarning	= 0x0010,	// Includes warnings.
		DebugFlag_OutputError	= 0x0020,	// Includes errors.
		DebugFlag_AttachSource	= 0x0040,	// Attach source line to outputs.
		DebugFlag_AttachTime	= 0x0080,	// Attach timestamp to outputs.

		DebugFlag_ForceInt		= 65535
	};

	#define DebugFlag_VerboseMode	(DebugFlag_OutputInfo		| \
									 DebugFlag_OutputWarning	| \
									 DebugFlag_OutputError)				// All types included.

	#define DEBUGFLAG_DEFAULT		(DebugFlag_FileOutput	| \
									 DebugFlag_VerboseMode	| \
									 DebugFlag_AttachSource | \
									 DebugFlag_AttachTime)					// Default flags.

	#define DEBUG_DEFAULT_FILENAME	TEXT("nova.log")

	class DebugManager
	{
	private:
		FILE	*log_file;
		DWORD	output_flag;
		TCHAR	filename[260];
		bool	alert_on_failure;

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
