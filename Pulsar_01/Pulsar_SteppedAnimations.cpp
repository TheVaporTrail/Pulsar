//--------------------------------------------------------------------------------
//	Pulsar: Stepped Animations
//
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#include "SinglePixel.h"

#define kMaxBrightness 255
#define kMinBrightness 0


//--------------------------------------------------------------------------------
//	Animation: Triangle Fade, Original
//--------------------------------------------------------------------------------
int8_t dir = 1;
uint8_t brightness = kMinBrightness;

void triangleFadeOriginal(SinglePixel* single)
{
	single->setBrightness(brightness);

	if (dir == 1)
	{
		brightness += 1;
		if (brightness >= kMaxBrightness)
			dir = -1;
	}
	else
	{
		brightness -= 1;
		if (brightness <= kMinBrightness)
			dir = 1;
	}	
}

//--------------------------------------------------------------------------------
//	Animation: Ramp Fade
//--------------------------------------------------------------------------------
void rampFade(SinglePixel* single)
{
	single->setBrightness(brightness);

	brightness += 1;
	if (brightness >= kMaxBrightness)
		brightness = kMinBrightness;
}

