//
// File: DebugManager.h
// ====================
// Provide a debug system for writing logs.
//

#ifndef IDENTI3D_SRC_UTILS_DEBUGMANAGER_H
#define IDENTI3D_SRC_UTILS_DEBUGMANAGER_H

#include <src/identi3d/General.h>

#if defined (UNICODE) && !defined (DBGHELP_TRANSLATE_TCHAR)
# define DBGHELP_TRANSLATE_TCHAR				// Gain wide char support.
#endif // !defined (DBGHELP_TRANSLATE_TCHAR)

namespace Identi3D {

	/*
	 * Prints non-verbose debug messages.
	 */
	#if !defined (_DebugPrint)
	# define _DebugPrint(DebugMgr, _Format, ...) \
		((DebugMgr) && (reinterpret_cast<DebugManager *>(DebugMgr))->print(__FILE__, __LINE__, false, _Format, __VA_ARGS__))
	#endif // !defined (_DebugPrint)
	
	/*
	 * Prints verbose debug messages.
	 */
	#if !defined (_DebugPrintV)
	# define _DebugPrintV(DebugMgr, _Format, ...) \
		((DebugMgr) && (reinterpret_cast<DebugManager *>(DebugMgr))->print(__FILE__, __LINE__, true, _Format, __VA_ARGS__))
	#endif // !defined (_DebugPrintV)

	/*
	 * Several flags used in formatting the output.
	 * 
	 *	DebugFlag_ConsoleOutput:	Writes messages to console window.
	 *	DebugFlag_FileOutput:		Writes messages to log file.
	 *	DebugFlag_AttachSource:		Adds source line info to messages.
	 *	DebugFlag_AttachTime:		Adds time stamp to messages.
	 *	DebugFlag_VerboseMode:		Prints verbose message.
	 */
	enum DebugFlag
	{
		DebugFlag_ConsoleOutput	= 0x0001,
		DebugFlag_FileOutput	= 0x0002,

		DebugFlag_AttachSource	= 0x0010,
		DebugFlag_AttachTime	= 0x0020,

		DebugFlag_VerboseMode	= 0x0100,

		DebugFlag_ForceDword	= 0xFFFFFFFF
	};

	/*
	 * Default debug flags.
	 */
	#if !defined (DEBUGMANAGER_DEFAULT_FLAG)
	# define DEBUGMANAGER_DEFAULT_FLAG	(DebugFlag_FileOutput	| \
										 DebugFlag_AttachSource | \
										 DebugFlag_VerboseMode	| \
										 DebugFlag_AttachTime)
	#endif // !defined (DEBUGMANAGER_DEFAULT_FLAG)

	/*
	 * Default log file name.
	 */
	#if !defined (DEBUGMANAGER_DEFAULT_FILENAME)
	# define DEBUGMANAGER_DEFAULT_FILENAME	__T("identi3d.log")
	#endif // !defined (DEBUGMANAGER_DEFAULT_FILENAME)

	class __declspec(dllexport) DebugManager
	{
	public:

		DebugManager(void);
		~DebugManager(void);

		/*
		 * Write debugging information to log file.
		 */
		HRESULT print(const char *src_filename, int line_number, bool verbose, const TCHAR *message, ...);

		/*
		 * Set log file name.
		 */
		HRESULT setOutputFileName(const TCHAR *new_filename = DEBUGMANAGER_DEFAULT_FILENAME);

		/*
		 * Create a new console window for debugging.
		 */
		HRESULT createDebugConsole(void);
		
		/*
		 * Print current call stack.
		 */
		HRESULT dumpCallStack(void);

		/*
		 * Print Option Tree.
		 */
		HRESULT printOptionTree(const OptionTree *tree);

		/*
		 * Set output flag.
		 */
		void setOutputFlag(DWORD new_flag = DEBUGMANAGER_DEFAULT_FLAG) { output_flag = new_flag; }

		/*
		 * Get current output flag.
		 */
		const DWORD	getOutputFlag(void) const { return output_flag; }

	private:
		FILE	*log_file, *console;
		DWORD	output_flag;
	};

};

#endif // IDENTI3D_SRC_UTILS_DEBUGMANAGER_H
