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

	class CpuInfo : public Singleton<CpuInfo>
	{
	private:
		TCHAR cpu_vendor[MAX_CPU_VENDOR_STRING_LENGTH + 1];
		TCHAR brand_string[MAX_CPU_BRAND_STRING_LENGTH + 1];
		int stepping, processor_type;
		DWORD family;
		DWORD model;
		DWORD basic_feature_edx, basic_feature_ecx;
		DWORD ext_feature_edx, ext_feature_ecx;

	public:
		CpuInfo(void);
		~CpuInfo(void);

		inline static void init(void) { getInstance(); }

		inline const TCHAR *getVendorName(void) const { return cpu_vendor; }
		inline const TCHAR *getBrandString(void) const { return brand_string; }
		inline int getStepping(void) const { return stepping; }
		inline int getProcessorType(void) const { return processor_type; }
		inline DWORD getFamily(void) const { return family; }
		inline DWORD getModel(void) const { return model; }

		inline bool isOnboardFPUSupported(void) const { return ((basic_feature_edx & 1) != 0) ? true : false; }
		inline bool isSSESupported(void) const { return ((basic_feature_edx & (1 << 25)) != 0) ? true : false; }
		inline bool isSSE2Supported(void) const { return ((basic_feature_edx & (1 << 26)) != 0) ? true : false; }
		inline bool isSSE3Supported(void) const { return ((basic_feature_ecx & 1) != 0) ? true : false; }
		inline bool is3DNowSupported(void) const { return ((ext_feature_edx & (1 << 31)) != 0) ? true : false; }
		inline bool isMMXSupported(void) const { return ((basic_feature_edx & (1 << 23)) != 0) ? true : false; }
	};

};

#endif