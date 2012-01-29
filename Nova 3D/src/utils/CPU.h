//
// File: CPU.h
// ===========
// Retrive and store CPU information.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_SRC_UTILS_CPU_H
#define NOVA3D_SRC_UTILS_CPU_H

#include <src/nova-3d/NovaGeneral.h>
#include <src/utils/Singleton.h>

namespace Nova3D
{
	
	//
	// Enum: CpuVendor
	// =====
	// Defines the vendor of CPU.
	//
	// Values:
	//	CpuVendor_Unknown: Unknown vendor.
	//	CpuVendor_AMD: Advanced Micro Devices(R).
	//	CpuVendor_Intel: Intel(R).
	//
	enum CpuVendor
	{
		CpuVendor_Unknown,
		CpuVendor_AMD,
		CpuVendor_Intel,
		
		CpuVendor_ForceInt = 65535
	};

	//
	// Class: CpuInfo
	// ======
	// Get and store CPU information.
	//
	class CpuInfo : public Singleton<CpuInfo>
	{
	private:
		CpuVendor vendor;

		struct {
			bool _sse_supported;
			bool _sse2_supported;
			bool _sse3_supported;
			bool _3dnow_supported;
			bool _mmx_supported;
		};

	public:
		CpuInfo(void);
		~CpuInfo(void);

		inline CpuVendor getVendor(void) const { return vendor; }

		inline bool isSSESupported(void) const { return _sse_supported; }
		inline bool isSSE2Supported(void) const { return _sse2_supported; }
		inline bool isSSE3Supported(void) const { return _sse3_supported; }
		inline bool is3DNowSupported(void) const { return _3dnow_supported; }
		inline bool isMMXSupported(void) const { return _mmx_supported; }
	};

};

#endif