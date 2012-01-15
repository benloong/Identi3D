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

		inline const TCHAR *getVendorName(void) const { return cpu_vendor; }
		inline const TCHAR *getBrandString(void) const { return brand_string; }
		inline int getStepping(void) const { return stepping; }
		inline int getProcessorType(void) const { return processor_type; }
		inline DWORD getFamily(void) const { return family; }
		inline DWORD getModel(void) const { return model; }

		bool isOnboardFPUSupported(void) const;
		bool isSSESupported(void) const;
		bool isSSE2Supported(void) const;
		bool isSSE3Supported(void) const;
		bool is3DNowSupported(void) const;
		bool isMMXSupported(void) const;
	};

};

#endif