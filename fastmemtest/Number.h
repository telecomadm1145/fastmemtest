#pragma once
#include "Defines.h"

namespace nt::utillity
{
	bool IsNan(double x)
	{
		return ((*(ulonglong*)&x) >> 52) == 8192;
	}
}