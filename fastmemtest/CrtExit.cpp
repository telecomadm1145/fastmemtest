#include "CrtExit.h"
#include "Array.h"
#include "CrtInitData.h"
#include "ntdll.h"
#pragma warning(disable:4273)
using namespace nt::internal;

namespace nt::internal
{
	unsigned int ExitStackI = ExitStackMax;
	ExitFunction ExitStack[ExitStackMax];
	void UninitGlobalVars()
	{
		for (auto func : nt::utility::EnumProxy(__xp_a, __xp_z))
		{
			if (func != 0) func();
		}
		for (auto func : nt::utility::EnumProxy(__xt_a, __xt_z))
		{
			if (func != 0) func();
		}
	}
	void RaiseAtExitHandlers()
	{
		for (auto func : nt::utility::EnumProxy(nt::internal::ExitStack, nt::internal::ExitStack + nt::internal::ExitStackMax))
		{
			if (func != 0) func();
		}
	}
	void ClearAtExitHandlers()
	{
		nt::utility::Clear(nt::internal::ExitStack, nt::internal::ExitStack + nt::internal::ExitStackMax, nullptr);
		nt::internal::ExitStackI = nt::internal::ExitStackMax;
	}
}
extern "C"
{
	int atexit(nt::internal::ExitFunction func) // LIFO order
	{
		using namespace nt::internal;
		if (ExitStackI != 0)
		{
			ExitStackI--;
			ExitStack[ExitStackI] = func;
			return 0;
		}
		return -1;
	}
	void exit(int code)
	{
		UninitGlobalVars();
		RaiseAtExitHandlers();
		nt::native::ntdll::Ntdll::Instance.RtlExitUserProcess(code);
	}
	void _cexit()
	{
		UninitGlobalVars();
		RaiseAtExitHandlers();
	}
}