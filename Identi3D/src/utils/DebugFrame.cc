//
// File: DebugFrame.cc
// ===================
//

#include <src/utils/DebugFrame.h>

namespace Identi3D
{

	DebugFrame::~DebugFrame(void)
	{
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

}
