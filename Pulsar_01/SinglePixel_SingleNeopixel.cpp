//--------------------------------------------------------------------------------
//	Single Pixel: Single Neopixel
//
//	Subclass
//
//	2021.09.27: Created
//--------------------------------------------------------------------------------
#include "SinglePixel_SingleNeopixel.h"

#define NEOPIXEL_COUNT 1

SinglePixel_SingleNeopixel::SinglePixel_SingleNeopixel(uint8_t neopixelPin) : SinglePixel()
{
	neopixel = new Adafruit_NeoPixel(NEOPIXEL_COUNT, neopixelPin, NEO_GRB + NEO_KHZ800);

	neopixel->begin();
	neopixel->clear();
	neopixel->show();
}

SinglePixel_SingleNeopixel::~SinglePixel_SingleNeopixel(void)
{
}

void SinglePixel_SingleNeopixel::setBrightnessValue(uint16_t brightness)
{
	uint32_t color;
	
	color = ScaleColor(this->color, brightness, 255);
	
	neopixel->setPixelColor(0, color);
	neopixel->show();
}

