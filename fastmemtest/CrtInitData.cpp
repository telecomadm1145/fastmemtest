#ifdef _MSC_VER
#define _CRTALLOC(x) __declspec(allocate(x))
#elif defined(__GNUC__)
#define _CRTALLOC(x) __attribute__ ((section (x) ))
#else
#error Your compiler is not supported.
#endif

#if defined(_MSC_VER)

typedef void(__cdecl* _PVFV)(void);
typedef int(__cdecl* _PIFV)(void);

#if defined(_M_IA64) || defined(_M_AMD64)
#define _ATTRIBUTES read
#else
#define _ATTRIBUTES read
#endif

#pragma section(".CRTMP$XCA",long,_ATTRIBUTES)
#pragma section(".CRTMP$XCZ",long,_ATTRIBUTES)
#pragma section(".CRTMP$XIA",long,_ATTRIBUTES)
#pragma section(".CRTMP$XIZ",long,_ATTRIBUTES)

#pragma section(".CRTMA$XCA",long,_ATTRIBUTES)
#pragma section(".CRTMA$XCZ",long,_ATTRIBUTES)
#pragma section(".CRTMA$XIA",long,_ATTRIBUTES)
#pragma section(".CRTMA$XIZ",long,_ATTRIBUTES)

#pragma section(".CRTVT$XCA",long,_ATTRIBUTES)
#pragma section(".CRTVT$XCZ",long,_ATTRIBUTES)

#pragma section(".CRT$XCA",long,_ATTRIBUTES)
#pragma section(".CRT$XCAA",long,_ATTRIBUTES)
#pragma section(".CRT$XCC",long,_ATTRIBUTES)
#pragma section(".CRT$XCZ",long,_ATTRIBUTES)
#pragma section(".CRT$XDA",long,_ATTRIBUTES)
#pragma section(".CRT$XDC",long,_ATTRIBUTES)
#pragma section(".CRT$XDZ",long,_ATTRIBUTES)
#pragma section(".CRT$XIA",long,_ATTRIBUTES)
#pragma section(".CRT$XIAA",long,_ATTRIBUTES)
#pragma section(".CRT$XIC",long,_ATTRIBUTES)
#pragma section(".CRT$XID",long,_ATTRIBUTES)
#pragma section(".CRT$XIY",long,_ATTRIBUTES)
#pragma section(".CRT$XIZ",long,_ATTRIBUTES)
#pragma section(".CRT$XLA",long,_ATTRIBUTES)
#pragma section(".CRT$XLC",long,_ATTRIBUTES)
#pragma section(".CRT$XLD",long,_ATTRIBUTES)
#pragma section(".CRT$XLZ",long,_ATTRIBUTES)
#pragma section(".CRT$XPA",long,_ATTRIBUTES)
#pragma section(".CRT$XPX",long,_ATTRIBUTES)
#pragma section(".CRT$XPXA",long,_ATTRIBUTES)
#pragma section(".CRT$XPZ",long,_ATTRIBUTES)
#pragma section(".CRT$XTA",long,_ATTRIBUTES)
#pragma section(".CRT$XTB",long,_ATTRIBUTES)
#pragma section(".CRT$XTX",long,_ATTRIBUTES)
#pragma section(".CRT$XTZ",long,_ATTRIBUTES)
#pragma section(".rdata$T",long,read)
#pragma section(".rtc$IAA",long,read)
#pragma section(".rtc$IZZ",long,read)
#pragma section(".rtc$TAA",long,read)
#pragma section(".rtc$TZZ",long,read)
/* for tlssup.c: */
#pragma section(".tls",long,read,write)
#pragma section(".tls$AAA",long,read,write)
#pragma section(".tls$ZZZ",long,read,write)

_CRTALLOC(".CRT$XIA") _PIFV __xi_a[] = { nullptr }; // C initializers (first)
_CRTALLOC(".CRT$XIZ") _PIFV __xi_z[] = { nullptr }; // C initializers (last)
_CRTALLOC(".CRT$XCA") _PVFV __xc_a[] = { nullptr }; // C++ initializers (first)
_CRTALLOC(".CRT$XCZ") _PVFV __xc_z[] = { nullptr }; // C++ initializers (last)

_CRTALLOC(".CRT$XPA") _PVFV __xp_a[] = { nullptr }; // C pre-terminators (first)
_CRTALLOC(".CRT$XPZ") _PVFV __xp_z[] = { nullptr }; // C pre-terminators (last)
_CRTALLOC(".CRT$XTA") _PVFV __xt_a[] = { nullptr }; // C terminators (first)
_CRTALLOC(".CRT$XTZ") _PVFV __xt_z[] = { nullptr }; // C terminators (last)

#pragma comment(linker, "/merge:.CRT=.rdata")

#endif /* _MSC_VER */
