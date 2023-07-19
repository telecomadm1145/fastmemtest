// Do not remove this file.

#ifdef _MSC_VER
#pragma comment(linker,"/nodefaultlib /entry:NtSDKStartup")
#pragma comment(linker, "/merge:.CRT=.rdata") // for init
#ifndef _DEBUG
#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/merge:.pdata=.text")
#endif 

#endif

#ifdef __cplusplus
extern "C" {
#endif
	int _fltused = 0; // it should be a single underscore since the double one is the mangled name
#ifdef __cplusplus
}
#endif
