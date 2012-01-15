//
// File: CPU.cc
// ============
//

#include "src/utils/CPU.h"

namespace Nova3D
{

	CpuInfo::CpuInfo(void)
	{
		char *tmp_vendor = new char[MAX_CPU_VENDOR_STRING_LENGTH + 1];
		char *tmp_brand = new char[MAX_CPU_BRAND_STRING_LENGTH + 1];
		DWORD cpuinfo, edx_value, ecx_value, ext_edx, ext_ecx;

		memset(cpu_vendor, 0, sizeof(cpu_vendor));
		memset(brand_string, 0, sizeof(brand_string));
		memset(tmp_vendor, 0, sizeof(char) * MAX_CPU_VENDOR_STRING_LENGTH + 1);
		memset(tmp_brand, 0, sizeof(char) * MAX_CPU_BRAND_STRING_LENGTH + 1);
		__asm {
			mov eax, 0h;			// Get CPU vendor
			cpuid;

			mov esi, tmp_vendor;
			mov [esi], ebx;
			mov [esi + 4], edx;
			mov [esi + 8], ecx;

			mov eax, 1h;			// Get CPU basic feature
			cpuid;
			
			mov [cpuinfo], eax;
			mov [edx_value], edx;
			mov [ecx_value], ecx;

			mov eax, 80000001h;		// Get CPU extended feature
			cpuid;

			mov [ext_edx], edx;
			mov [ext_ecx], ecx;

			mov eax, 80000000h;
			cpuid;

			mov esi, 80000004h;
			cmp esi, eax;
			jg no_brand_string;
			
			mov esi, tmp_brand;
			mov eax, 80000002h;
			cpuid;

			mov [esi], eax;
			mov [esi + 4], ebx;
			mov [esi + 8], ecx;
			mov [esi + 12], edx;

			mov eax, 80000003h;
			cpuid;

			mov [esi + 16], eax;
			mov [esi + 20], ebx;
			mov [esi + 24], ecx;
			mov [esi + 28], edx;

			mov eax, 80000004h;
			cpuid;

			mov [esi + 32], eax;
			mov [esi + 36], ebx;
			mov [esi + 40], ecx;
			mov [esi + 44], edx;

		no_brand_string:
		}

#ifndef UNICODE
		strncpy_s(cpu_vendor, tmp_vendor, MAX_CPU_VENDOR_STRING_LENGTH);
		strncpy_s(brand_string, tmp_brand, MAX_CPU_BRAND_STRING_LENGTH);
#else
		mbstowcs_s(NULL, cpu_vendor, tmp_vendor, MAX_CPU_VENDOR_STRING_LENGTH);
		mbstowcs_s(NULL, brand_string, tmp_brand, MAX_CPU_BRAND_STRING_LENGTH);
#endif
		delete[] tmp_vendor, tmp_brand;

		stepping = cpuinfo & 0x0000000F;
		if(_tcscmp(cpu_vendor, __T("AuthenticAMD")) == 0) {
			family = (cpuinfo & 0x00000F00) >> 8;
			model = (cpuinfo & 0x000000F0) >> 4;
			if(family >= 0x0F) {
				family += (cpuinfo & 0x0FF00000) >> 20;
				model += (cpuinfo & 0x000F0000) >> 12;
			}
		} else {
			family = ((cpuinfo & 0x00000F00) >> 8) + ((cpuinfo & 0x0FF00000) >> 20);
			model = ((cpuinfo & 0x000000F0) >> 4) + ((cpuinfo & 0x000F0000) >> 12);
		}
		basic_feature_edx = edx_value;
		basic_feature_ecx = ecx_value;
		ext_feature_edx = ext_edx;
		ext_feature_ecx = ext_ecx;
	}

	bool CpuInfo::isOnboardFPUSupported(void) const
	{
		return ((basic_feature_edx & 1) != 0) ? true : false;
	}

	bool CpuInfo::isSSESupported(void) const
	{
		return ((basic_feature_edx & (1 << 25)) != 0) ? true : false;
	}

	bool CpuInfo::isSSE2Supported(void) const
	{
		return ((basic_feature_edx & (1 << 26)) != 0) ? true : false;
	}

	bool CpuInfo::isSSE3Supported(void) const
	{
		return ((basic_feature_ecx & 1) != 0) ? true : false;
	}

	bool CpuInfo::is3DNowSupported(void) const
	{
		return ((ext_feature_edx & (1 << 31)) != 0) ? true : false;
	}

	bool CpuInfo::isMMXSupported(void) const
	{
		return ((basic_feature_edx & (1 << 23)) != 0) ? true : false;
	}

	CpuInfo::~CpuInfo(void)
	{
	}

}