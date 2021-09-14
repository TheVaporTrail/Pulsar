#include "SinglePixel_AnalogPin.h"

SinglePixel_AnalogPin::SinglePixel_AnalogPin(uint8_t pin) : SinglePixel()
{
	this->pin = pin;
	pinMode(this->pin, OUTPUT);
}

SinglePixel_AnalogPin::~SinglePixel_AnalogPin(void)
{
}

void SinglePixel_AnalogPin::setBrightnessValue(uint16_t brightness)
{
	analogWrite(this->pin, brightness);
}

