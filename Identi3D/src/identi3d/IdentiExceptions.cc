//
// File: IdentiExceptions.cc
// =========================
//

#include <src/identi3d/IdentiExceptions.h>
#include <src/identi3d/Messages.h>

namespace Identi3D
{

	const char *UnknownException::what(void) const throw ()
	{
		return "Unknown exception caught.";
	}

	const char *OutOfMemoryException::what(void) const throw ()
	{
		return "Insufficient memory.";
	}

	const char *InvalidParametersException::what(void) const throw ()
	{
		return "Invalid parameters.";
	}

	const char *FileOperationFailureException::what(void) const throw ()
	{
		return "Failed to read/write the file";
	}

	const char *ObjectUninitializedException::what(void) const throw ()
	{
		return "Object not initialized before using.";
	}

};
