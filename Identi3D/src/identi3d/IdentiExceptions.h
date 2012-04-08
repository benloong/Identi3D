//
// File: IdentiExceptions.h
// ========================
// Exceptions for Identi3D.
//

#ifndef IDENTI3D_SRC_IDENTI3D_IDENTIEXCEPTION_H
#define IDENTI3D_SRC_IDENTI3D_IDENTIEXCEPTION_H

#include <exception>

namespace Identi3D
{

	class UnknownException : public std::exception
	{
		const char *what(void) const throw ()
		{
			return "Unknown exception caught.";
		}
	};

	class OutOfMemoryException : public std::exception
	{
		const char *what(void) const throw ()
		{
			return "Insufficient memory.";
		}
	};

	class InvalidParametersException : public std::exception
	{
		const char *what(void) const throw ()
		{
			return "Invalid parameters.";
		}
	};

	class FileOperationFailureException : public std::exception
	{
		const char *what(void) const throw ()
		{
			return "Failed to read/write the file";
		}
	};

	class ObjectUninitializedException : public std::exception
	{
		const char *what(void) const throw ()
		{
			return "Object not initialized before using.";
		}
	};

	class InitializationFailedException : public std::exception
	{
		const char *what(void) const throw ()
		{
			return "Failed to initialize object(s).";
		}
	};

	class RenderingProcedureFailedException : public std::exception
	{
		const char *what(void) const throw()
		{
			return "System failed to render.";
		}
	};

	class PrerequisiteNotSatisfiedException : public std::exception
	{
		const char *what(void) const throw()
		{
			return "Prerequisites are not satisfied.";
		}
	};

};

#endif // IDENTI3D_SRC_IDENTI3D_IDENTIEXCEPTION_H