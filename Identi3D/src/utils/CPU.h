//
// File: CPU.h
// ===========
// Retrive and store CPU information.
//

#ifndef IDENTI3D_SRC_UTILS_CPU_H
#define IDENTI3D_SRC_UTILS_CPU_H

#include <src/identi3d/General.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{
	
	/*
	 * Vendors of CPU.
	 * Currently only support judgement for AMD or Intel.
	 */
	enum CpuVendor
	{
		CpuVendor_Unknown,
		CpuVendor_AMD,
		CpuVendor_Intel,
		
		CpuVendor_ForceInt = 65535
	};

	class __declspec(dllexport) CpuInfo : public DebugFrame
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
		CpuInfo(DebugManager *debugger = NULL);
		~CpuInfo(void);

		/*
		 * Get the vendor of CPU
		 */
		inline CpuVendor getVendor(void) const { return vendor; }

		/*
		 * Get SSE support state.
		 */
		inline bool isSSESupported(void) const { return _sse_supported; }

		/*
		 * Get SSE2 support state.
		 */
		inline bool isSSE2Supported(void) const { return _sse2_supported; }
			
		/*
		 * Get SSE3 support state.
		 */
		inline bool isSSE3Supported(void) const { return _sse3_supported; }

		/*
		 * Get 3DNow! support state.
		 */
		inline bool is3DNowSupported(void) const { return _3dnow_supported; }
			
		/*
		 * Get MMX support state.
		 */
		inline bool isMMXSupported(void) const { return _mmx_supported; }
	};

};

#endif // IDENTI3D_SRC_UTILS_CPU_H