#pragma once
#include "Defines.h"
namespace nt::native
{
	struct CurrentDirectory
	{
		NtString DosPath;
		void* Handle;
	};
	struct RtlUserProcessParamerters {
		ulong MaximumLength;
		ulong Length;
		ulong Flags;
		ulong DebugFlags;
		void* ConsoleHandle;
		ulong ConsoleFlags;
		void* StdIn;
		void* StdOut;
		void* StdErr;
		CurrentDirectory CurrentDir;
		NtString DllPath;
		NtString ImagePathName;
		NtString CommandLine;
		void* Environment;
		ulong StartingX;
		ulong StartingY;
		ulong CountX;
		ulong CountY;
		ulong CountCharsX;
		ulong CountCharsY;
		ulong FIllAttribute;
		ulong WindowFlags;
		ulong ShowWindowFlags;
		NtString WindowTitle;
		NtString DesktopInfo;
		NtString ShellInfo;
		NtString RuntimeData;
		// see https://www.geoffchappell.com/studies/windows/km/ntoskrnl/inc/api/pebteb/rtl_user_process_parameters.htm
	};
	struct PebLoaderList {
		void* Reserved1[2];
		NtDoubleLinkedList InMemoryOrderLinks;
		void* Reserved2[2];
		void* DllBase;
		void* Reserved3[2];
		NtString FullDllName;
		byte Reserved4[8];
		void* Reserved5[3];
		union {
			ulong CheckSum;
			void* Reserved6;
		};
		ulong TimeDateStamp;
	};
	struct PebLoaderData {
		byte Reserved1[8];
		void* Reserved2[1];
		PebLoaderList InMemoryOrderModuleList;
	};
	struct Peb {
		byte Reserved1[2];
		byte BeingDebugged;
		byte Reserved2[1];
		void* Reserved3[2];
		PebLoaderData* Ldr;
		RtlUserProcessParamerters* ProcessParameters;
		void* Reserved4[3];
		void* AtlThunkSListPtr;
		void* Reserved5;
		ulong Reserved6;
		void* Reserved7;
		ulong Reserved8;
		ulong AtlThunkSListPtr32;
		void* Reserved9[45];
		byte Reserved10[96];
		CallbackFunction PostProcessInitRoutine;
		byte Reserved11[128];
		void* Reserved12[1];
		ulong SessionId;
		void* GetProcessHeap()
		{
#ifdef _WIN64
			return *(void**)((char*)this + 0x30);
#else
			return *(void**)((char*)this + 0x18);
#endif
		}
	};
}