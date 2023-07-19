#pragma once
#ifdef _M_X64
extern "C"
unsigned __int64 __readgsqword(
	unsigned long Offset
);
#pragma intrinsic(__readgsqword)
#elif _M_IX86
extern "C"
unsigned long __readfsdword(
	unsigned long Offset
);
#pragma intrinsic(__readfsdword)
#endif
namespace nt::native
{
	void* NtCurrentPeb()
	{
#ifdef _M_X64
		return (void*)__readgsqword(0x60);
#elif _M_IX86
		return (void*)__readfsdword(0x30);
#endif
	}
	void* NtCurrentTeb()
	{
#ifdef _M_X64
		return (void*)__readgsqword(0x30);
#elif _M_IX86
		return (void*)__readfsdword(0x18);
#endif
	}
}