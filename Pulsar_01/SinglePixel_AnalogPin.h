#include "SinglePixel.h"

class SinglePixel_AnalogPin : public SinglePixel {

	public:

		// Constructor
		SinglePixel_AnalogPin(uint8_t pin);
		
		// Destructor
	   ~SinglePixel_AnalogPin(void);
	   
		// 
		void setBrightness(uint16_t brightness);
		
	private:
		uint8_t	pin;

		void setBrightnessValue(uint16_t brightness);
	
};
