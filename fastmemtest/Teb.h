#pragma once
#include "NtPeb.h"
namespace nt::native
{
	struct _TEB {
		void* Reserved1[12];
		Peb* ProcessEnvironmentBlock;
		void* Reserved2[399];
		byte Reserved3[1952];
		void* TlsSlots[64];
		byte Reserved4[8];
		void* Reserved5[26];
		void* ReservedForOle;  // Windows 2000 only
		void* Reserved6[4];
		void* TlsExpansionSlots;
		uint* LastError()
		{
#ifdef _WIN64
			return (uint*)((byte*)this + 0x68);
#else
			return 0;
#endif
		}
	};
}