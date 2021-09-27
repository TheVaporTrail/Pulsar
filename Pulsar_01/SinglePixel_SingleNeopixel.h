//--------------------------------------------------------------------------------
//	Single Pixel: Single Neopixel
//
//	Subclass
//
//	2021.09.27: Created
//--------------------------------------------------------------------------------
#include "SinglePixel.h"
#include <Adafruit_NeoPixel.h>

class SinglePixel_SingleNeopixel : public SinglePixel {

	public:
		// Constructor
		SinglePixel_SingleNeopixel(uint8_t pin);
		
		// Destructor
	   ~SinglePixel_SingleNeopixel(void);
	   
		// 
		void setBrightness(uint16_t brightness);
		
	private:
		Adafruit_NeoPixel* neopixel;

		void setBrightnessValue(uint16_t brightness);
		
		
	
};
