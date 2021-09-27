//--------------------------------------------------------------------------------
//	Animation Utilities
//
//	2021.09.27: Created
//--------------------------------------------------------------------------------
#include "AnimationUtils.h"

//--------------------------------------------------------------------------------
//	Calc Triangle Wave
//--------------------------------------------------------------------------------
uint32_t CalcTriangleWave(uint32_t ms, uint32_t width, uint32_t lo, uint32_t hi)
{
	uint16_t tri;

	ms = (ms % width);
	width = (width >> 1);
	hi = hi - lo;

	if (ms < width)
		tri = (ms * hi)/(width - 1);
	else
		tri = (((width<<1)-1-ms) * hi)/(width - 1);

	tri += lo;

	return tri;
}

//--------------------------------------------------------------------------------
//	Calc Ramp
//--------------------------------------------------------------------------------
uint32_t CalcRamp(uint32_t ms, uint32_t width, uint32_t startLevel, uint32_t endLevel)
{
	uint16_t r;

	ms = (ms % width);

	if (startLevel < endLevel)
	{
		endLevel -= startLevel;
		r = (ms * endLevel)/(width - 1) + startLevel;
	}
	else
	{
		startLevel -= endLevel;
		ms = width - ms;
		r = (ms * startLevel)/(width - 1) + endLevel;
	}

	return r;
}

