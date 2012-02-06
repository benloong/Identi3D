//
// File: NovaGeneral.h
// ===================
// General include file for Nova3D
//

#ifndef NOVA3D_SRC_NOVA3D_NOVAGENERAL_H
#define NOVA3D_SRC_NOVA3D_NOVAGENERAL_H

// General name string for Nova3D
#if !defined(NOVA3D_NAME)
 #define NOVA3D_NAME	__T("Nova3D")
#endif

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
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <tchar.h>
#include <cstdarg>
#include <cmath>
#include <xmmintrin.h>
#include <intrin.h>
#include <new>

// Nova3D Internal Libraries
#include "NovaNotification.h"

// Forward Classes
namespace Nova3D
{
	
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