#pragma once

#if defined(_MSC_VER)

typedef void(__cdecl* _PVFV)(void);
typedef int(__cdecl* _PIFV)(void);

extern _PIFV __xi_a[]; // C initializers (first)
extern _PIFV __xi_z[]; // C initializers (last)
extern _PVFV __xc_a[]; // C++ initializers (first)
extern _PVFV __xc_z[]; // C++ initializers (last)

extern _PVFV __xp_a[]; // C pre-terminators (first)
extern _PVFV __xp_z[]; // C pre-terminators (last)
extern _PVFV __xt_a[]; // C terminators (first)
extern _PVFV __xt_z[]; // C terminators (last)

#endif /* _MSC_VER */
