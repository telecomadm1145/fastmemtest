#pragma once
#include <concepts>
#include "defines.h"
namespace nt::utility
{
	template<std::integral Int, typename CharT>
	bool ToString(Int num, CharT* out, size_t bufsz)
	{
		int j = 0;
		if (bufsz == 0 || out == 0)
			return false;
		if (num == 0)
		{
			out[j++] = '0';
			out[j++] = 0;
			return true;
		}
		if (num < 0)
		{
			out[j++] = '-';
			num = -num;
		}
		Int num2 = num;
		while (num2 > 0)
		{
			num2 /= 10;
			j++;
		}
		if (j >= bufsz)
			return false;
		out[j] = 0;
		j--;
		while (num)
		{
			if (j >= bufsz || j < 0)
				return false;
			out[j--] = num % 10 + '0';
			num /= 10;
		}
		return true;
	}
}