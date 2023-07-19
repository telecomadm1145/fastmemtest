#pragma once
#include <cstdint>
// Copyed from msdn XD

constexpr auto max_path = 260;
constexpr auto null = 0;

using ulonglong = unsigned long long;
using longlong = long long;
using ulong = unsigned long;
using uint = unsigned int;
using ushort = unsigned short;
using byte = unsigned char;

using CallbackFunction = void*;

#define Uncopyable(x) 		private: \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator=(const x&) = delete; \
x& operator=(x&&) = delete;

#define QUOTE(x) #x
#define WSTR(x) L##x
#define WQUOTE(x) WSTR(#x)

#ifdef _MSC_VER
#define ForceInline constexpr _forceinline
#else
#define ForceInline constexpr inline
#endif

namespace nt::native
{
	struct NtDoubleLinkedList {
		NtDoubleLinkedList* Flink;
		NtDoubleLinkedList* Blink;
	};
	struct NtString {
		ushort Length;
		ushort MaximumLength;
		wchar_t* Buffer;
	};
	struct Win32String {
		ulong Length;
		ulong MaximumLength : 31;
		ulong bAnsi : 1;
		const wchar_t* Buffer;
	};
	struct Guid {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	};
}