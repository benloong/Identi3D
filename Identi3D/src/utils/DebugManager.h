//
// File: DebugManager.h
// ====================
// Provide a debug system for writing logs.
//

#ifndef IDENTI3D_SRC_UTILS_DEBUGMANAGER_H
#define IDENTI3D_SRC_UTILS_DEBUGMANAGER_H

#include <src/identi3d/General.h>

namespace Identi3D {

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
	static const DWORD DEBUGMANAGER_DEFAULTFLAG =
		(DebugFlag_FileOutput | DebugFlag_AttachSource | DebugFlag_VerboseMode |  DebugFlag_AttachTime);

	/*
	 * Default log file name.
	 */
	static const std::string DEBUGMANAGER_DEFAULTLOGNAME = "identi3d.log";

	/*
	 * Default welcome message of log file.
	 */
	static const std::string DEBUGMANAGER_WELCOMEMESSAGE = "Identi3D: Logging started.\n\n";

	/*
	 * Undefined class signature.
	 */
	static const char *DEBUGFRAME_UNDEFINEDSIGNATURE = "undefined";

	class __declspec(dllexport) DebugManager
	{
	public:

		DebugManager(void) throw () : _prevbuf(NULL), _flag(DEBUGMANAGER_DEFAULTFLAG) {} ;
		~DebugManager(void) throw ();

		/*
		 * Write debugging information to log file.
		 */
		bool print(const char *src_path, int line_number, bool verbose, const char *message, ...) throw ();
		
		/*
		 * Write debugging information to log file.
		 */
		bool print(const char *src_path, int line_number, bool verbose, const char *message, va_list &arg) throw ();

		/*
		 * Write the specified exception to log file.
		 */
		bool print(const char *src_path, int line_number, const std::exception &e) throw ();

		/*
		 * Set log file name.
		 */
		bool setOutputFileName(const std::string &new_filename = DEBUGMANAGER_DEFAULTLOGNAME) throw ();

		/*
		 * Create a new console window for debugging.
		 */
		bool createDebugConsole(void) throw ();
		
		/*
		 * Print current call stack.
		 */
		void dumpCallStack(void) throw ();

		/*
		 * Set output flag.
		 */
		inline void setFlag(DWORD new_flag = DEBUGMANAGER_DEFAULTFLAG) throw () { _flag = new_flag; }

		/*
		 * Get current output flag.
		 */
		inline const DWORD getFlag(void) const throw () { return _flag; }

		/*
		 * On memory allocation.
		 */
		static void onAllocation(size_t size) throw ();

		/*
		 * On memory deallocation.
		 */
		static void onDeallocation(size_t size) throw ();

		/*
		 * Print memory allocation status.
		 */
		void printMemoryStatus(void) throw ();

		/*
		 * Query global debug manager availability.
		 */
		static bool isValid(void) throw ();

		/*
		 * Print raw string without being processed.
		 */
		void printRawString(const char *str, ...) throw ();

	public:
		/*
		 * Overload new operator.
		 */
		static void *operator new(size_t size) throw();

		/*
		 * Overload delete operator.
		 */
		static void operator delete(void *p) throw();

	private:
		/*
		 * Restrict access to create multiple DebugManager object.
		 */
		static void *operator new[](size_t size);
		static void *operator new[](size_t size, void *p);

		/*
		 * and deny inplacement new as well.
		 */
		static void *operator new(size_t size, void *p);

	private:
		std::ofstream _log;
		std::ofstream _console;
		std::streambuf *_prevbuf;

		DWORD _flag;

		static size_t _allocated_memory;
		static bool _is_valid;

	private:
		DebugManager(DebugManager &mgr);
		DebugManager &operator=(DebugManager &rhs);

	private:
		const std::string getTimeStamp(void) const;
		const std::string getFormattedSourcePath(const char *src_path, int line_number) const;
	};

	class __declspec(dllexport) DebugFrame
	{
	protected:
		DebugManager *_debugger;

	public:

		DebugFrame(DebugManager *debugger = NULL);
		virtual ~DebugFrame(void) = 0;

#if defined(_MEMORY_LEAK_DETECTION)
		/*
		 * Override object's new operator.
		 */
		static void *operator new(size_t size);
		static void *operator new[](size_t size);

		/*
		 * Override object's delete operator.
		 */
		static void operator delete(void *p, size_t size);
		static void operator delete[](void *p, size_t size);

#endif // defined(_MEMORY_LEAK_DETECTION)
		
		/*
		 * Set a debug manager.
		 */
		virtual void setDebugManager(DebugManager *debugger = NULL);
		
	protected:

		/*
		 * Print an exception.
		 * Pass __FILE__ and __LINE__ for first two arguments.
		 */
		bool _printException(const char *src_path, int line_number, const std::exception &e) const;

		/*
		 * Print verbose message.
		 * Pass __FILE__ and __LINE__ for first two arguments.
		 */
		bool _printVerboseMessage(const char *src_path, int line_number, const char *message, ...) const;

		/*
		 * Print normal message.
		 * Pass __FILE__ and __LINE__ for first two arguments.
		 */
		bool _printMessage(const char *src_path, int line_number, const char *message, ...) const;
	};

};

#endif // IDENTI3D_SRC_UTILS_DEBUGMANAGER_H
