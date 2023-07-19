#pragma once
#include "Defines.h"
namespace nt::native
{
	void ExitProcess(uint uExitCode);
	using ExitProcessT = decltype(ExitProcess)*;
}