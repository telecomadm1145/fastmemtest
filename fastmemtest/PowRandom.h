#pragma once
#include "Defines.h"
class PowRandom
{
private:
	uint state;
public:
	uint Next()
	{
		state = (state >> 10) + state + (state << 10);
		return state;
	}
};