//
// File: NovaGeneral.h
// ===================
//

#ifndef NOVA3D_SRC_NOVA3D_NOVAGENERAL_H
#define NOVA3D_SRC_NOVA3D_NOVAGENERAL_H

// Nova3D version 0.1
#if !defined(NOVA_VERSION)
 #define NOVA_VERSION	0x0001
#else
 #error "Versions of Nova3D libraries conflict!"
#endif

#if defined(_WIN32)
 #include <Windows.h>
#else
 #error "Support Windows platform only."
#endif

// C++ runtime libraries.
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <tchar.h>
#include <cstdarg>
#include <sstream>

#include "NovaException.h"

// General name string for Nova3D
#if !defined(NOVA3D_NAME)
 #define NOVA3D_NAME	__T("Nova3D")
#endif

#endif