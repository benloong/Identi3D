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

/*
 * Detect unicode support.
 */
#if !defined (UNICODE)
# error "Identi3D Library: Requires unicode environment."
#endif // !define (UNICODE)

/*
 * Identi3D general name string.
 */
#if !defined (IDENTI3D_NAME)
# define IDENTI3D_NAME			L"Identi3D"
#endif // !defined (IDENTI3D_NAME)

/*
 * Current library version:
 *    0.1 [prerelease]
 */
#if !defined (IDENTI3D_VERSION)
# define IDENTI3D_VERSION			0x0001
#else
# error "Identi3D Library: Version conflict detected."
#endif // !defined (IDENTI3D_VERSION)

/*
 * Check target platform.
 */
#if defined (_WIN32)
# include <Windows.h>

/*
 * Compile DirectX support module.
 */
# if !defined (_TARGET_DIRECTX)
#  define _TARGET_DIRECTX
# endif // !defined (_TARGET_DIRECTX)

#else
# error "Identi3D Library: Support Windows platform only."
#endif // defined (_WIN32)

/*
 * Enable 16 bit data align.
 * Essential for SSE computation.
 */
#if !defined (_DATA_ALIGN)
# define _DATA_ALIGN __declspec(align(16))
#endif // !defined(_DATA_ALIGN)

/*
 * Compile SSE code only [Improve Performance].
 * Many CPUs are shipped with SSE support,
 *   cutting away normal calculations seems to be fine.
 */
#if !defined (_SSE_ONLY)
//# define _SSE_ONLY
#endif // !defined (_SSE_ONLY)

/*
 * Enable internal memory leak detection.
 * Object inherited from DebugFrame will be recorded.
 */
#if !defined (_MEMORY_LEAK_DETECTION)
# define _MEMORY_LEAK_DETECTION
#endif // !defined (_MEMORY_LEAK_DETECTION)

/*
 * C++ Runtime and STL libraries.
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>
#include <cmath>
#include <new>
#include <fstream>
#include <string>
#include <vector>
#include <tchar.h>

/*
 * SSE & SSE2 essentials.
 */
#include <xmmintrin.h>
#include <intrin.h>

/*
 * Something went wrong between STL and DLL classes.
 * No affect though, just disable it.
 */
#pragma warning (disable : 4251)

/*
 * Debug messages header file.
 */
#include "Messages.h"

/*
 * Forward declaration.
 */
namespace Identi3D
{
	
	// Enumerations
	enum KeyType;
	
	enum CpuVendor;
	enum DebugFlag;

	enum RenderBackendType;
	enum SystemFlag;
	enum SystemState;
	enum Event;

	// Structures
	struct OptionElement;
	struct EventPacket;

	// Classes
	class Color;
	class System;
	class EventDispatcher;
	class EventListener;

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
	class DebugFrame;
	class OptionTree;
	class SettingsManager;

};

#endif // IDENTI3D_SRC_IDENTI3D_GENERAL_H
