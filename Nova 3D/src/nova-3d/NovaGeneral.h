//
// File: NovaGeneral.h
// ===================
// General include file for Nova3D
//

#ifndef NOVA3D_SRC_NOVA3D_NOVAGENERAL_H
#define NOVA3D_SRC_NOVA3D_NOVAGENERAL_H

// General name string for Nova3D
#if !defined(NOVA3D_NAME)
# if defined(UNICODE)
#  define NOVA3D_NAME	L"Nova3D"
# else
#  define NOVA3D_NAME	"Nova3D"
# endif
#endif

// Nova3D version 0.1
#if !defined(NOVA_VERSION)
# define NOVA_VERSION	0x0001
#else
# error "Versions of Nova3D libraries conflict!"
#endif

#if defined(_WIN32)
# include <Windows.h>
# if !defined(_TARGET_D3D_VERSION)		// Compile for Direct3D 9
#  define _TARGET_D3D9_VERSION 9
# endif
#else
# error "Support Windows platform only."
#endif

#if !defined(_SSE_ALIGN)
# define _DATA_ALIGN __declspec(align(16))	// align specified data structures to 16 bit
#endif

#if !defined(_SSE_ONLY)
# define _SSE_ONLY						// Compile SSE code only.
#endif

// C++ runtime libraries.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <tchar.h>
#include <cstdarg>
#include <cmath>
#include <xmmintrin.h>
#include <intrin.h>
#include <new>
#include <fstream>

// Nova3D Internal Libraries
#include "NovaNotification.h"

// Forward Classes
namespace Nova3D
{
	
	// Basic Classes
	class Color;

	// Math Classes
	class Angle;
	class AxisAlignedBoundingBox;
	class Matrix;
	class OrientedBoundingBox;
	class Plane;
	class Polygon;
	class Quaternion;
	class Ray;
	class Vector3;

	// Renderer Classes
	class RenderDevice;
	class Renderer;
	class RenderTarget;
	class RenderWindow;

	// Utility Classes
	class CpuInfo;
	class DebugManager;
	class JsonReader;
	class JsonWriter;
	class SettingsEnumerator;
	class SettingsManager;

};

#endif