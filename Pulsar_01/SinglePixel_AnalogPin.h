//--------------------------------------------------------------------------------
//	Single Pixel: Analog Pin
//
//	Subclass
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#include "SinglePixel.h"

class SinglePixel_AnalogPin : public SinglePixel {

	public:
		// Constructor
		SinglePixel_AnalogPin(uint8_t pin);
		
		// Destructor
	   ~SinglePixel_AnalogPin(void);
	   
	private:
		uint8_t	pin;

		void setBrightnessValue(uint16_t brightness);
	
};
