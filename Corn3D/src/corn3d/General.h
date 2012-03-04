//
// File: General.h
// ===============
// General header file for Corn3D Library.
//
// # Do not include this file directly.
// # Use Corn3D.h instead.
//

#ifndef CORN3D_SRC_CORN3D_GENERAL_H
#define CORN3D_SRC_CORN3D_GENERAL_H

// General Name
#if !defined (CORN3D_NAME)
# if defined (UNICODE)
#  define CORN3D_NAME			L"Corn3D"
# else
#  define CORN3D_NAME			"Corn3D"
# endif // defined (UNICODE)
#endif // !defined (CORN3D_NAME)

// Version 0.1 [PRERELEASE]
#if !defined (CORN3D_VERSION)
# define CORN3D_VERSION			0x0001
#else
# error "Corn3D Library: Version conflict detected."
#endif // !defined (CORN3D_VERSION)

// Check platform
#if defined (_WIN32)
# include <Windows.h>
# if !defined (_TARGET_DIRECTX)
#  define _TARGET_DIRECTX
# endif // !defined (_TARGET_DIRECTX)
#else
# error "Corn3D Library: Support Windows platform only."
#endif // defined (_WIN32)

// 16 bit data align.
#if !defined (_DATA_ALIGN)
# define _DATA_ALIGN __declspec(align(16))
#endif // !defined(_DATA_ALIGN)

// Compile SSE code only.
#if !defined (_SSE_ONLY)
# define _SSE_ONLY
#endif // !defined (_SSE_ONLY)


// C++ runtime libraries.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>
#include <cmath>
#include <new>
#include <fstream>
#include <tchar.h>

#include <xmmintrin.h>
#include <intrin.h>

// Corn3D Internal Libraries
#include "Messages.h"

// Forward Declaration
namespace Corn3D
{
	
	// Enumerations
	enum KeyType;
	
	enum CpuVendor;
	enum DebugFlag;
	enum OptionType;

	enum RenderBackendType;

	// Structures
	struct tagOptionElement;

	// Classes
	class Color;

	class Angle;
	class AxisAlignedBoundingBox;
	class Matrix;
	class OrientedBoundingBox;
	class Plane;
	class Polygon;
	class Quaternion;
	class Ray;
	class Vector3;

	class RenderDevice;
	class Renderer;
	class RenderTarget;
	class RenderWindow;

	class Direct3D9SettingsManager;
	class PluginDirect3D9;

	class CpuInfo;
	class DebugManager;
	class OptionTree;

};

#endif // CORN3D_SRC_CORN3D_GENERAL_H
