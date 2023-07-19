#pragma once
#include <stack>
namespace nt::internal
{
	constexpr size_t ExitStackMax = 32;
	extern unsigned int ExitStackI;
	using ExitFunction = void(__cdecl*)(void);
	extern ExitFunction ExitStack[ExitStackMax];
	void UninitGlobalVar();
	void RaiseAtExitHandler();
}