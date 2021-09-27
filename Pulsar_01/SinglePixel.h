//--------------------------------------------------------------------------------
//	Single Pixel
//
//	Abstract class for settings the brightness for a single LED or other light source
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#ifndef __SINGLEPIXEL_H__
#define __SINGLEPIXEL_H__

#ifdef ARDUINO
#include "Arduino.h"
#endif

//--------------------------------------------------------------------------------
//	Color Utilities
//--------------------------------------------------------------------------------
uint32_t ScaleColor(uint32_t color, uint8_t numerator, uint8_t denominator);
uint32_t HueToColor(uint16_t hue, uint16_t range = 0xff, uint8_t brightness = 0xff);



//--------------------------------------------------------------------------------
//	Single Pixel
//--------------------------------------------------------------------------------
class SinglePixel {

	public:
		// Constructor
		SinglePixel();
		
		// Destructor
	   ~SinglePixel(void);
	   
		// Set the LED brightness
		void setBrightness(uint16_t brightness);
	   
		// Set the hue
		void setHue(uint8_t hue, uint8_t hueRange = 255);
	   
		// Set the color
		void setColor(uint32_t color);

		
	protected:
		uint16_t	brightness;
		uint8_t		hue;
		uint8_t		hueRange;
		uint32_t	color;
		
		// Set the brightness. Subclasses must override this.
		virtual void setBrightnessValue(uint16_t brightness);
};

#endif // __SINGLEPIXEL_H__