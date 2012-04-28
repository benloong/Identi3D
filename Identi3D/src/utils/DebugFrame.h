//
// File: DebugFrame.h
// ==================
// Provide a debug framework for classes.
//

#ifndef IDENTI3D_SRC_UTILS_DEBUGFRAME_H
#define IDENTI3D_SRC_UTILS_DEBUGFRAME_H

#include <src/identi3d/General.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{
	
	class __declspec(dllexport) DebugFrame
	{
	protected:
		DebugManager *_debugger;

	public:

		DebugFrame(DebugManager *debugger = NULL) : _debugger(debugger) {} ;
		virtual ~DebugFrame(void) = 0;

#if defined(_MEMORY_LEAK_DETECTION)
		/*
		 * Override object's new operator.
		 */
		static void *operator new(size_t size, const std::nothrow_t &);
		static void *operator new[](size_t size, const std::nothrow_t &);

		/*
		 * Override object's delete operator.
		 */
		static void operator delete(void *p, size_t size);
		static void operator delete[](void *p, size_t size);

		/*
		 * Prepare for exceptions thrown during initialization process.
		 */
		static void operator delete(void *p, const std::nothrow_t &);
		static void operator delete[](void *p, const std::nothrow_t &);

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

	private:

		/*
		 * Forbid throw-able new operations.
		 */
		static void *operator new(size_t size);
		static void *operator new[](size_t size);
	};

};

#endif // IDENTI3D_SRC_UTILS_DEBUGFRAME_H
