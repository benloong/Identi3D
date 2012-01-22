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
		DWORD edx_value, ecx_value, ext_edx, ext_ecx;

		memset(tmp_vendor, 0, sizeof(char) * MAX_CPU_VENDOR_STRING_LENGTH + 1);
		__asm {
			mov eax, 0h;			// Get CPU vendor
			cpuid;
			mov esi, tmp_vendor;
			mov [esi], ebx;
			mov [esi + 4], edx;
			mov [esi + 8], ecx;

			mov eax, 1h;			// Get CPU basic feature
			cpuid;
			mov [edx_value], edx;
			mov [ecx_value], ecx;

			mov eax, 80000001h;		// Get CPU extended feature
			cpuid;
			mov [ext_edx], edx;
			mov [ext_ecx], ecx;
		}

		if(*tmp_vendor == 'G') {
			if(*(tmp_vendor + 7) == 'I') vendor = CpuVendor_Intel;
			else vendor = CpuVendor_Unknown;
		} else if(*tmp_vendor == 'A') {
			if(*(tmp_vendor + 9) == 'A') vendor = CpuVendor_AMD;
			else if(*(tmp_vendor + 11) == '!') vendor = CpuVendor_AMD;
			else vendor = CpuVendor_Unknown;
		} else {
			vendor = CpuVendor_Unknown;
		}
		delete[] tmp_vendor;

		basic_feature_edx = edx_value;
		basic_feature_ecx = ecx_value;
		ext_feature_edx = ext_edx;
		ext_feature_ecx = ext_ecx;

		_sse_supported = ((basic_feature_edx & (1 << 25)) != 0) ? true : false;
		_sse2_supported = ((basic_feature_edx & (1 << 26)) != 0) ? true : false;
		_sse3_supported = ((basic_feature_ecx & 1) != 0) ? true : false;
		_3dnow_supported = ((ext_feature_edx & (1 << 31)) != 0) ? true : false;
		_mmx_supported = ((basic_feature_edx & (1 << 23)) != 0) ? true : false;
	}

	CpuInfo::~CpuInfo(void)
	{
	}

}