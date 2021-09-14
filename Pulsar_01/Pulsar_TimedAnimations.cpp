//--------------------------------------------------------------------------------
//	Pulsar: Timed Animations
//
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#include "Pulsar_TimedAnimations.h"


//--------------------------------------------------------------------------------
//	Animation Settings
//--------------------------------------------------------------------------------
#define kMaxBrightness 255
#define kMinBrightness 0

/// ?? Parameters or constants?
#define period  5000
#define deltaBrightness  (kMaxBrightness - kMinBrightness)
#define period2  500

//--------------------------------------------------------------------------------
//	Static globals
//
//	These variables are here instead of having static variables in each
//	animation routine
//--------------------------------------------------------------------------------
static uint32_t gStartTime = 0;
static uint32_t gEndTime = 0;

//--------------------------------------------------------------------------------
//	Animation Prototypes
//--------------------------------------------------------------------------------
void triangleFade(SinglePixel* single, uint32_t deltaTime);
void sineFade(SinglePixel* single, uint32_t deltaTime);
void triangleBlinkFade(SinglePixel* single, uint32_t deltaTime);
void rampFade(SinglePixel* single, uint32_t deltaTime);
void pulseFade(SinglePixel* single, uint32_t deltaTime);
void pulseTrainFade(SinglePixel* single, uint32_t deltaTime);
void randomWalkFade(SinglePixel* single, uint32_t deltaTime);
void triangleFadeFade(SinglePixel* single, uint32_t deltaTime);
void randomLevel(SinglePixel* single, uint32_t deltaTime);
void bounce(SinglePixel* single, uint32_t deltaTime);


//--------------------------------------------------------------------------------
//	Animation Function List
//--------------------------------------------------------------------------------
static animationFunc_t sAnimationFunctions[] = {
	bounce,
	pulseTrainFade,
	randomLevel,
	pulseFade,
	triangleFadeFade,
	randomWalkFade,
	rampFade,
	triangleFade,
	sineFade,
	triangleBlinkFade,
	NULL
};

//--------------------------------------------------------------------------------
//	Get Timed Animation List
//--------------------------------------------------------------------------------
animationFunc_t* getTimedAnimationList(void)
{
	return sAnimationFunctions;
}

//--------------------------------------------------------------------------------
//	Animation: Triangle Fade
//--------------------------------------------------------------------------------
void triangleFade(SinglePixel* single, uint32_t deltaTime)
{
	uint32_t ms = (deltaTime % period);
	
	uint32_t brightness; 
	
	if (ms < period/2)
		brightness= (ms * deltaBrightness)/(period/2 - 1) + kMinBrightness;
	else
		brightness= ((period-1-ms) * deltaBrightness)/(period/2 - 1) + kMinBrightness;
	
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Ramp Fade
//--------------------------------------------------------------------------------
void rampFade(SinglePixel* single, uint32_t deltaTime)
{
	uint32_t ms = (deltaTime % period);
	
	uint32_t brightness; 
	
	brightness= (ms * deltaBrightness)/(period - 1) + kMinBrightness;
	
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Sine Fade
//--------------------------------------------------------------------------------
void sineFade(SinglePixel* single, uint32_t deltaTime)
{
	uint32_t ms = (deltaTime % period);
	
	uint32_t brightness = (ms * deltaBrightness)/(period - 1) + kMinBrightness;
	
	single->setBrightness(sine8t(brightness));
}

//--------------------------------------------------------------------------------
//	Animation: Triangle Blink Fade
//--------------------------------------------------------------------------------
void triangleBlinkFade(SinglePixel* single, uint32_t deltaTime)
{
	uint32_t ms = (deltaTime % period);
	uint32_t m2 = (ms % period2);
	
	uint32_t brightness; 
	
	if (ms < period/2)
		brightness= (ms * deltaBrightness)/(period/2 - 1) + kMinBrightness;
	else
		brightness= ((period-1-ms) * deltaBrightness)/(period/2 - 1) + kMinBrightness;
		
	if (m2 < period2/2)
		brightness = 0;
	
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Triangle Fade Fade
//--------------------------------------------------------------------------------
void triangleFadeFade(SinglePixel* single, uint32_t deltaTime)
{
	uint32_t ms = (deltaTime % period);
	uint32_t m2 = (ms % period2);
	
	#define BrightAdjust 50
	
	#define minBr (kMinBrightness)
	#define maxBr (kMaxBrightness - BrightAdjust)
	#define delBr (maxBr - minBr)
	
	uint32_t brightness; 
	
	uint32_t adj;
	if (m2 < period2/2)
		adj= (m2 * BrightAdjust)/(period2/2 - 1);
	else
		adj= ((period2-1-m2) * BrightAdjust)/(period2/2 - 1);
	
	
	
	if (ms < period/2)
		brightness= (ms * delBr)/(period/2 - 1) + minBr;
	else
		brightness= ((period-1-ms) * delBr)/(period/2 - 1) + minBr;
	
 	brightness += adj;
	
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Pulse Fade
//--------------------------------------------------------------------------------
void pulseFade(SinglePixel* single, uint32_t deltaTime)
{
	uint32_t ms = (deltaTime % period);
	uint32_t m2 = (ms % period2);
	
	uint32_t brightness; 
	
	if (ms < period2/2)
		brightness= (m2 * deltaBrightness)/(period2/2 - 1) + kMinBrightness;
	else if (ms < period2)
		brightness= ((period2-1-m2) * deltaBrightness)/(period2/2 - 1) + kMinBrightness;
	else
		brightness = 0;
		
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Pulse Train Fade
//--------------------------------------------------------------------------------
void pulseTrainFade(SinglePixel* single, uint32_t deltaTime)
{
	uint32_t ms = (deltaTime % period);
	uint32_t m2 = (ms % period2);
	uint32_t i  = ms/period2; // Subperiod number
	
	uint32_t brightness;
	
	static uint16_t sPulseTrain = 0;
	
	// Init: Random number to represent eight random pulses
	if (deltaTime == 0)
		sPulseTrain = random(255);
	
	// If the bit is not set, then no pulse
	if ( (sPulseTrain & (1 << i)) )
		brightness = 0;
	// Otherwise, ramp up for the first half of the period
	else if (m2 < period2/2)
		brightness= (m2 * deltaBrightness)/(period2/2 - 1) + kMinBrightness;
	// And ramp down for the second half.
	else
		brightness= ((period2-1-m2) * deltaBrightness)/(period2/2 - 1) + kMinBrightness;
		
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Random Walk Fade
//--------------------------------------------------------------------------------
void randomWalkFade(SinglePixel* single, uint32_t deltaTime)
{
	static uint8_t startLevel = 0;
	static uint8_t endLevel = 0;
	
	if (deltaTime == 0 || gEndTime == 0 || deltaTime > gEndTime)
	{
		gEndTime = deltaTime + random(750, 2000);
		gStartTime = deltaTime;
		startLevel = endLevel;
		endLevel = random(8, 255);
	}
	
	uint32_t ms = (deltaTime - gStartTime);
	
	uint32_t brightness; 
	uint32_t deltaT = gEndTime - gStartTime;
	
	if (startLevel < endLevel)
	{
		uint32_t deltaL = endLevel - startLevel;
		brightness = (ms * deltaL)/(deltaT - 1) + startLevel;
	}
	else
	{
		uint32_t deltaL = startLevel - endLevel;
		ms = deltaT - ms;
		brightness = (ms * deltaL)/(deltaT - 1) + endLevel;
	}
	
	single->setBrightness(brightness);
}


//--------------------------------------------------------------------------------
//	Animation: Random Level
//--------------------------------------------------------------------------------
void randomLevel(SinglePixel* single, uint32_t deltaTime)
{
	static uint32_t brightness = 0;
	#define kRandomLevelPeriod 1000
	
	uint32_t b; 

	if (deltaTime == 0 || gEndTime == 0 || deltaTime > gEndTime)
	{
		gEndTime = deltaTime + kRandomLevelPeriod;

		do {
			b = 0x0f * random(0, 0x11);
		} while (b == brightness);
		
		brightness = b;
		single->setBrightness(brightness);
	}
}


//--------------------------------------------------------------------------------
//	Animation: Bounce
//--------------------------------------------------------------------------------
void bounce(SinglePixel* single, uint32_t deltaTime)
{
	#define kBounceInitialMaxBrightness kMaxBrightness
	#define kBounceInitialWidth (period/4)
	#define kBounceDecayNum (3)
	#define kBounceDecayDen (4)
	
	
	static uint16_t sMaxBrightness = kBounceInitialMaxBrightness;
	static uint16_t sBounceWidth = kBounceInitialWidth;
	
	uint32_t brightness;
	
	// Init: Reset max brightness and bounce width
	if (deltaTime == 0 || sBounceWidth == 0)
	{
		gStartTime = deltaTime;
		gEndTime = gStartTime + sBounceWidth;
		sMaxBrightness = kBounceInitialMaxBrightness;
		sBounceWidth = kBounceInitialWidth;
	}
	else if (deltaTime > gEndTime)
	{
		gStartTime = deltaTime;
		gEndTime = gStartTime + sBounceWidth;
		sBounceWidth =  (sBounceWidth * kBounceDecayNum) / kBounceDecayDen;
		sMaxBrightness = (sMaxBrightness * kBounceDecayNum) / kBounceDecayDen;
	}
	
	if (sBounceWidth < 100)
	{
		brightness = 0;
	}
	else
	{
		deltaTime -= gStartTime;

		brightness = (deltaTime * sMaxBrightness)/(sBounceWidth - 1);
	}
	single->setBrightness(sine8t(brightness));
}


