//
// File: CPU.h
// ===========
// Retrive and store CPU information.
//
// Copyright (c) Sparkfire, 2011 - 2012.
//

#ifndef NOVA3D_SRC_UTILS_CPU_H
#define NOVA3D_SRC_UTILS_CPU_H

#include "src/nova-3d/NovaGeneral.h"
#include "src/utils/Singleton.h"

#define MAX_CPU_VENDOR_STRING_LENGTH	12
#define MAX_CPU_BRAND_STRING_LENGTH		48

namespace Nova3D
{
	
	enum CpuVendor
	{
		CpuVendor_Unknown,
		CpuVendor_AMD,
		CpuVendor_Intel,
		
		CpuVendor_ForceInt = 65535
	};

	class CpuInfo : public Singleton<CpuInfo>
	{
	private:
		CpuVendor vendor;
		DWORD basic_feature_edx, basic_feature_ecx;
		DWORD ext_feature_edx, ext_feature_ecx;

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

		inline bool isOnboardFPUSupported(void) const { return ((basic_feature_edx & 1) != 0) ? true : false; }
		inline bool isSSESupported(void) const { return _sse_supported; }
		inline bool isSSE2Supported(void) const { return _sse2_supported; }
		inline bool isSSE3Supported(void) const { return _sse3_supported; }
		inline bool is3DNowSupported(void) const { return _3dnow_supported; }
		inline bool isMMXSupported(void) const { return _mmx_supported; }
	};

};

#endif