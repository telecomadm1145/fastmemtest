#include "CrtInitData.h"
#include "Array.h"
namespace nt::internal
{
	void InitGlobalVars()
	{
		for (auto func : nt::utility::EnumProxy(__xi_a, __xi_z))
		{
			if (func != 0) func();
		}
		for (auto func : nt::utility::EnumProxy(__xc_a, __xc_z))
		{
			if (func != 0) func();
		}
	}
}