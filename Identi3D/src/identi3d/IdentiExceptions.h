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
		const char *what(void) const throw ();
	};

	class OutOfMemoryException : public std::exception
	{
		const char *what(void) const throw ();
	};

	class InvalidParametersException : public std::exception
	{
		const char *what(void) const throw ();
	};

	class FileOperationFailureException : public std::exception
	{
		const char *what(void) const throw ();
	};

	class ObjectUninitializedException : public std::exception
	{
		const char *what(void) const throw ();
	};

};

#endif // IDENTI3D_SRC_IDENTI3D_IDENTIEXCEPTION_H