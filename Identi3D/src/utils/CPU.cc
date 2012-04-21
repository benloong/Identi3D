//
// File: CPU.cc
// ============
//

#include <src/utils/CPU.h>

#define _TestBit(n, b) ((((n) & (1 << (b))) != 0) ? true : false)

namespace Identi3D
{

	CpuInfo::CpuInfo(void)
#if defined(_MEMORY_LEAK_DETECTION)
		: DebugFrame(NULL)
#endif // _MEMORY_LEAK_DETECTION
	{
		int data[4];
		
		__cpuid(data, 0);
		data[2] ^= data[3];
		data[3] ^= data[2];
		data[2] ^= data[3];
		char *tmp_vendor = reinterpret_cast<char *>(&data[1]);
		if(*tmp_vendor == 'G' && *(tmp_vendor + 7) == 'I') {
			vendor = CpuVendor_Intel;
		} else if(*tmp_vendor == 'A' && *(tmp_vendor + 9) == 'A') {
			vendor = CpuVendor_AMD;
		} else {
			vendor = CpuVendor_Unknown;
		}

		__cpuid(data, 1);
		_sse_supported = _TestBit(data[3], 25);
		_sse2_supported = _TestBit(data[3], 26);
		_sse3_supported = _TestBit(data[2], 0);
		_mmx_supported = _TestBit(data[3], 23);

		__cpuid(data, 0x80000001);
		_3dnow_supported = _TestBit(data[3], 31);
	}

	CpuInfo::~CpuInfo(void)
	{
	}

}