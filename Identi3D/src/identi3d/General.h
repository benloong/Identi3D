//
// File: General.h
// ===============
// General header file for Identi3D Library.
//
// # Do not include this file directly.
// # Use Identi3D.h instead.
//

#ifndef IDENTI3D_SRC_IDENTI3D_GENERAL_H
#define IDENTI3D_SRC_IDENTI3D_GENERAL_H

// General Name
#if !defined (IDENTI3D_NAME)
# if defined (UNICODE)
#  define IDENTI3D_NAME			L"Identi3D"
# else
#  define IDENTI3D_NAME			"Identi3D"
# endif // defined (UNICODE)
#endif // !defined (IDENTI3D_NAME)

// Version 0.1 [PRERELEASE]
#if !defined (IDENTI3D_VERSION)
# define IDENTI3D_VERSION			0x0001
#else
# error "Identi3D Library: Version conflict detected."
#endif // !defined (IDENTI3D_VERSION)

// Check platform
#if defined (_WIN32)
# include <Windows.h>
# if !defined (_TARGET_DIRECTX)
#  define _TARGET_DIRECTX
# endif // !defined (_TARGET_DIRECTX)
#else
# error "Identi3D Library: Support Windows platform only."
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

// Identi3D Internal Libraries
#include "Messages.h"

// Forward Declaration
namespace Identi3D
{
	
	// Enumerations
	enum KeyType;
	
	enum CpuVendor;
	enum DebugFlag;

	enum RenderBackendType;
	enum SystemFlag;
	enum SystemState;

	// Structures
	struct OptionElement;

	// Classes
	class Color;
	class System;

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
	class RenderWindow;
	class RenderWindow;

	class Direct3D9SettingsManager;
	class PluginDirect3D9;

	class CpuInfo;
	class DebugManager;
	class OptionTree;
	class SettingsManager;

};

#endif // IDENTI3D_SRC_IDENTI3D_GENERAL_H
