// 现代nt6的控制台api

// 参考: https://www.unknowncheats.me/forum/c-and-c-/467327-cconsolecallservergeneric.html
#include "NtPeb.h"
#include "peb.h"
#include "ntdll.h"
#include "ConApi.h"
using namespace nt::native::ntdll;

struct CCSG_Message
{
	ulong Length;
	void* Buffer;
};
struct CCSG_ParamsBase
{
	ulong ControlCode;
	ulong Length;
};


inline void* GetConsoleHandle()
{
	auto peb = (nt::native::Peb*)nt::native::NtCurrentPeb();
	return peb->ProcessParameters->ConsoleHandle;
}
inline NtStatus ConsoleCallServerGeneric(void* FileHandle, void* Handle, CCSG_ParamsBase* Buffer, ulong ControlCode, ulong Length, CCSG_Message* message, ulong nMessage, CCSG_Message* a8, ulong a9)
{
	if (nMessage + a9 > 8) return (NtStatus)0xC00000BB;
	if ((uintptr_t)FileHandle <= 0) return (NtStatus)0xC0000008;

	struct
	{
		void* Handle;
		ulong Count;
		ulong Unk;
		struct
		{
			ulong Length, Unk;
			void* Buffer;
		} msg[3];
	}input;

	Buffer->ControlCode = ControlCode;
	Buffer->Length = Length;

	input.Handle = Handle;
	input.Count = nMessage + 1;
	input.Unk = a9 + 1;

	input.msg[0].Length = Length + sizeof(CCSG_Message);
	input.msg[0].Buffer = Buffer;

	input.msg[nMessage + 1].Length = Length;
	input.msg[nMessage + 1].Buffer = Buffer + 1;

	for (ulong i = 0; i < nMessage; ++i)
	{
		input.msg[i + 1].Buffer = message[i].Buffer;
		input.msg[i + 1].Length = message[i].Length;
	}
	for (ulong i = 0; i < a9; ++i)
	{
		input.msg[nMessage + i + 2].Length = a8[i].Length;
		input.msg[nMessage + i + 2].Buffer = a8[i].Buffer;
	}

	IoStatusBlock IoStatusBlock{};
	return nt::native::Ntdll.NtDeviceIoControlFile(FileHandle, 0, 0, 0, &IoStatusBlock, 0x500016, &input, 16 * (nMessage + a9 + 3), 0, 0);
}
NtStatus ConsoleCallServerWithBuffers(void* Handle, CCSG_ParamsBase* Buffer, ulong ControlCode, ulong Length, CCSG_Message* message, bool nMessage, CCSG_Message* a7, ulong a8)
{
	return ConsoleCallServerGeneric(GetConsoleHandle(), Handle, Buffer, ControlCode, Length, message, nMessage, a7, a8);
}
NtStatus ConsoleCallServer(void* FileHandle, CCSG_ParamsBase* Buffer, ulong ControlCode, ulong Length)
{
	return ConsoleCallServerGeneric(GetConsoleHandle(), FileHandle, Buffer, ControlCode, Length, 0, 0, 0, 0);
}
NtStatus WriteConsoleInternal(void* hConsoleOutput, const void* lpBuffer, ulong nNumberOfCharsToWrite, ulong* lpNumberOfCharsWritten, bool bUnicode)
{
	struct Params : CCSG_ParamsBase
	{
		ulong Chars;
		bool bUnicode;
	};

	CCSG_Message message;
	message.Length = bUnicode ? nNumberOfCharsToWrite * 2 : nNumberOfCharsToWrite;
	message.Buffer = (void*)lpBuffer;

	Params params;
	params.bUnicode = bUnicode;

	NtStatus status = ConsoleCallServerGeneric(hConsoleOutput, NULL, &params, 16777222, 8, &message, 1, 0, 0);
	if ((status == NtStatus::Success) && lpNumberOfCharsWritten)
	{
		*lpNumberOfCharsWritten = params.Chars;
		if (bUnicode) *lpNumberOfCharsWritten = params.Chars >> 1;
	}
	return ((int)status == 0xC000000D) ? (NtStatus)0xC0000008 : status;
}
bool WriteConsoleA(void* hConsoleOutput, const char* lpBuffer, ulong nNumberOfCharsToWrite, ulong* lpNumberOfCharsWritten)
{
	return WriteConsoleInternal(hConsoleOutput, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, 0) == NtStatus::Success;
}
bool WriteConsoleW(void* hConsoleOutput, const wchar_t* lpBuffer, ulong nNumberOfCharsToWrite, ulong* lpNumberOfCharsWritten)
{
	return WriteConsoleInternal(hConsoleOutput, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, 1) == NtStatus::Success;
}

NtStatus SetConsoleTitleInternal(const void* lpConsoleTitle, ulong len, bool bUnicode)
{
	struct Params : CCSG_ParamsBase
	{
		bool bUnicode;
	};

	CCSG_Message message;
	message.Length = bUnicode ? len * 2 : len;
	message.Buffer = (void*)lpConsoleTitle;

	Params params;
	params.bUnicode = bUnicode;

	return ConsoleCallServerWithBuffers(NULL, &params, 33554453, 1, &message, 1, 0, 0);
}
bool SetConsoleTitleA(const char* lpConsoleTitle)
{
	return SetConsoleTitleInternal((void*)lpConsoleTitle, nt::utility::GetStringLength(lpConsoleTitle), 0) == NtStatus::Success;
}
bool SetConsoleTitleW(const wchar_t* lpConsoleTitle)
{
	return SetConsoleTitleInternal((void*)lpConsoleTitle, nt::utility::GetStringLength(lpConsoleTitle) * 2, 1) == NtStatus::Success;
}

bool SetConsoleTextAttribute(void* hConsoleOutput, ushort wAttributes)
{
	struct Params : CCSG_ParamsBase
	{
		ushort Attributes;
	};
	Params params;
	params.Attributes = wAttributes;
	return ConsoleCallServer(hConsoleOutput, &params, 33554445, 2) == NtStatus::Success;
}
bool SetConsoleMode(void* hConsoleHandle, ulong dwMode)
{
	struct Params : CCSG_ParamsBase
	{
		ulong mode;
	};
	Params params;
	params.mode = dwMode;
	return ConsoleCallServer(hConsoleHandle, &params, 16777218, 4) == NtStatus::Success;
}

bool SetConsoleOutputCP(uint wCodePageID)
{
	struct Params : CCSG_ParamsBase
	{
		uint CodePage;
		uint Flags;
	};
	Params params;
	params.CodePage = wCodePageID;
	params.Flags = 513;
	return ConsoleCallServer(NULL, &params, 33554436, 8) == NtStatus::Success;
}
bool SetConsoleCP(uint wCodePageID)
{
	struct Params : CCSG_ParamsBase
	{
		uint CodePage;
		uint Flags;
	};
	Params params;
	params.CodePage = wCodePageID;
	params.Flags = 0;
	return ConsoleCallServer(NULL, &params, 33554436, 8) == NtStatus::Success;
}

bool SetConsoleFont(void* hConsoleOutput, bool bMaximumWindow, uint nFont, Coord dwFontSize, uint FontFamily, uint FontWeight, const wchar_t facename[38])
{
	struct Params : CCSG_ParamsBase
	{
		int bMaximumWindow;
		uint nFont;
		Coord dwFontSize;
		uint FontFamily;
		uint FontWeight;
		wchar_t FaceName[38];
	};
	Params params;
	params.dwFontSize = dwFontSize;
	params.FontFamily = FontFamily;
	params.FontWeight = FontWeight;
	params.nFont = nFont;
	params.bMaximumWindow = bMaximumWindow;
	nt::utility::Copy(facename, facename + 38, params.FaceName);
	return ConsoleCallServer(hConsoleOutput, &params, 50331692, 84) == NtStatus::Success;
}
bool SetConsoleScreenBufferSize(void* hConsoleOutput, Coord dwSize)
{
	struct Params : CCSG_ParamsBase
	{
		Coord Size;
	};
	Params params;
	params.Size = dwSize;
	return ConsoleCallServer(hConsoleOutput, &params, 33554441, 4) == NtStatus::Success;
}
//bool SetConsoleWindowInfo(HANDLE hConsoleOutput, BOOL bAbsolute, SMALL_RECT lpConsoleWindow)
//{
//	struct Params : CCSG_ParamsBase
//	{
//		short bAbsolute;
//		SMALL_RECT rect;
//	};
//	Params params;
//	params.bAbsolute = bAbsolute;
//	params.rect = lpConsoleWindow;
//	return ConsoleCallServer(hConsoleOutput, &params, 33554446, 10);
//}
bool SetConsoleCursorPosition(void* hConsoleOutput, Coord dwCursorPosition)
{
	struct Params : CCSG_ParamsBase {
		Coord pos;
	}params;
	params.pos = dwCursorPosition;
	return ConsoleCallServer(hConsoleOutput, &params, 33554442, sizeof(Params) - sizeof(CCSG_ParamsBase)) == NtStatus::Success;
}
// 对win32的逆向 详细请见kernelbase
bool ReadConsoleInput(void* hConsoleInput, InputRecord* lpBuffer, int nLength, uint* lpNumberOfEventsRead)
{
	struct Params : CCSG_ParamsBase {
	} params;
	CCSG_Message message;
	message.Length = 20 * nLength;
	message.Buffer = lpBuffer;
	return ConsoleCallServerGeneric(hConsoleInput, 0, &params, 0x1000004u, 8, 0, 0, &message, 1) == NtStatus::Success;
}