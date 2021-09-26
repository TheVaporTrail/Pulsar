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
#define kAnimationPeriod 5000
#define period  kAnimationPeriod

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
void pixelOff(SinglePixel* single, uint32_t deltaTime);
void triangleFade(SinglePixel* single, uint32_t deltaTime);
void sineFade(SinglePixel* single, uint32_t deltaTime);
void triangleBlinkFade(SinglePixel* single, uint32_t deltaTime);
void rampFade(SinglePixel* single, uint32_t deltaTime);
void pulseFade(SinglePixel* single, uint32_t deltaTime);
void pulseTrainFade(SinglePixel* single, uint32_t deltaTime);
void randomLevelFade(SinglePixel* single, uint32_t deltaTime);
void randomWalkFade(SinglePixel* single, uint32_t deltaTime);
void triangleFadeFade(SinglePixel* single, uint32_t deltaTime);
void randomLevel(SinglePixel* single, uint32_t deltaTime);
void bounce(SinglePixel* single, uint32_t deltaTime);
void candleFlicker(SinglePixel* single, uint32_t deltaTime);
void breath(SinglePixel* single, uint32_t deltaTime);
void heartbeat(SinglePixel* single, uint32_t deltaTime);
void oceanWave(SinglePixel* single, uint32_t time);


//--------------------------------------------------------------------------------
//	Animation Function List
//--------------------------------------------------------------------------------
static animationFunc_t sAnimationFunctions[] = {
	oceanWave,
// 	pixelOff,
	candleFlicker,
	breath,
	heartbeat,
	randomLevelFade,
	bounce,
	pulseTrainFade,
	randomLevel,
	pulseFade,
	triangleFadeFade,
	rampFade,
	randomWalkFade,
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

//--------------------------------------------------------------------------------
//	Animation: Triangle Fade
//--------------------------------------------------------------------------------
void pixelOff(SinglePixel* single, uint32_t deltaTime)
{
	single->setBrightness(0);
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
//	Animation: Random Level Fade
//--------------------------------------------------------------------------------
void randomLevelFade(SinglePixel* single, uint32_t deltaTime)
{
	static uint8_t startLevel = 0;
	static uint8_t endLevel = 0;

	if (deltaTime == 0 || gEndTime == 0 || deltaTime > gEndTime)
	{
		gEndTime = deltaTime + random(750, 2000);
		gStartTime = deltaTime;
		startLevel = endLevel;
		endLevel = random(kMinBrightness, kMaxBrightness);
	}

	uint32_t ms = (deltaTime - gStartTime);
	uint32_t deltaT = gEndTime - gStartTime;
	uint32_t brightness = CalcRamp(ms, deltaT, startLevel, endLevel);

	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Random Walk Fade
//--------------------------------------------------------------------------------
void randomWalkFade(SinglePixel* single, uint32_t deltaTime)
{
	#define kRandomWalkPeriod 250
	#define kRandomWalkRandomness 31 /* -4 to +4 */
	#define kRandomWalkStep 12

	static uint8_t startLevel = 0;
	static uint8_t endLevel = 0;

	if (deltaTime == 0 || gEndTime == 0 || deltaTime > gEndTime)
	{
		gEndTime = deltaTime + kRandomWalkPeriod;
		gStartTime = deltaTime;
		startLevel = endLevel;

// 		int8_t d = random(kRandomWalkRandomness);
//
// 		if (endLevel < kMinBrightness + kRandomWalkRandomness)
// 			endLevel += d;
// 		else if (endLevel > kMaxBrightness - kRandomWalkRandomness)
// 			endLevel -= d;
// 		else
// 			endLevel += d - kRandomWalkRandomness/2;

// 		int8_t d = random(3) - 1;
//
// 		if (d < 0 && endLevel >= kMinBrightness + kRandomWalkStep)
// 			endLevel -= kRandomWalkStep;
// 		else if (d > 0 &&endLevel > kMaxBrightness - kRandomWalkStep )
// 			endLevel += kRandomWalkStep;

		int8_t d = random(2);

		// Decrement 
		if (endLevel >= kMaxBrightness - kRandomWalkStep)
			endLevel -= kRandomWalkStep;
		else if (endLevel <= kMinBrightness + kRandomWalkStep)
			endLevel += kRandomWalkStep;
		else if (d == 0)
			endLevel -= kRandomWalkStep;
		else
			endLevel += kRandomWalkStep;
	}

	uint32_t ms = (deltaTime - gStartTime);
	uint32_t brightness = CalcRamp(ms, kRandomWalkPeriod, startLevel, endLevel);

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
	#define kBounceBrightnessDecrement (16)
	#define kBounceMinBrightness (96)
	#define kBounceMinWidth 20
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
		sBounceWidth =  (sBounceWidth * kBounceDecayNum) / kBounceDecayDen;
		//sMaxBrightness = (sMaxBrightness * kBounceDecayNum) / kBounceDecayDen;
		if (sMaxBrightness > kBounceMinBrightness + kBounceBrightnessDecrement)
			sMaxBrightness -= kBounceBrightnessDecrement;


		gStartTime = deltaTime;
		gEndTime = gStartTime + sBounceWidth;
	}
	
	if (sBounceWidth < kBounceMinWidth)
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

//--------------------------------------------------------------------------------
//	Animation: Candle Flicker
//--------------------------------------------------------------------------------
void candleFlicker(SinglePixel* single, uint32_t deltaTime)
{
	#define kFlickerMinPeriod 50
	#define kFlickerMaxPeriod 150
	#define kFlickerPeriods   5
	#define kFlickerLowBrightnessMin		(kMinBrightness + 50)
	#define kFlickerLowBrightnessMax		(kMinBrightness + 80)
	#define kFlickerMidBrightnessMin 		(kMinBrightness/2 + kMaxBrightness/2 - 20)
	#define kFlickerMidBrightnessMax 		(kMinBrightness/2 + kMaxBrightness/2 + 20)
	#define kFlickerHighBrightnessMin		(kMaxBrightness - 50)
	#define kFlickerHighBrightnessMax		(kMaxBrightness)

	static uint8_t startLevel = 0;
	static uint8_t endLevel = 0;
	static uint8_t levelCount = kFlickerPeriods;
	static uint8_t candleBrightRange = 1; // mid

	if (deltaTime == 0 || gEndTime == 0 || deltaTime > gEndTime)
	{
		if (levelCount == 0)
		{
			uint8_t r = random(20);
			if (r < 1)
				candleBrightRange = 0; // low
			else if (r < 2)
				candleBrightRange = 2; // high
			else
				candleBrightRange = 1; // high

			levelCount = kFlickerPeriods;
		}

		gEndTime = deltaTime + random(kFlickerMinPeriod, kFlickerMaxPeriod);
		gStartTime = deltaTime;
		startLevel = endLevel;
		levelCount -= 1;

		// Select a new level based on the "brightness range"
		if (candleBrightRange == 0)
			endLevel = random(kFlickerLowBrightnessMin, kFlickerLowBrightnessMax);
		else if (candleBrightRange == 1)
			endLevel = random(kFlickerMidBrightnessMin, kFlickerMidBrightnessMax);
		else
			endLevel = random(kFlickerHighBrightnessMin, kFlickerHighBrightnessMax);
	}

	uint32_t ms = deltaTime - gStartTime;
	uint32_t deltaT = gEndTime - gStartTime;
	uint32_t brightness = CalcRamp(ms, deltaT, startLevel, endLevel);

	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Breath
//
//	https://www.e-breathing.com/normal-respiratory-rate-volume-chart/
//--------------------------------------------------------------------------------
void breath(SinglePixel* single, uint32_t deltaTime)
{
	#define kBreathPeriod (kAnimationPeriod/2)
	#define kBreathOffset (kAnimationPeriod/4)

	uint32_t ms = (deltaTime % kAnimationPeriod);
	uint32_t brightness; 

	if (ms < kBreathOffset)
	{
		brightness = 0;
	}
	else
	{
		ms -= kBreathOffset;

		if (ms > kBreathPeriod)
			brightness = 0;
		else if (ms < kBreathPeriod/2)
			brightness= (ms * deltaBrightness)/(kBreathPeriod/2 - 1) + kMinBrightness;
		else
			brightness= ((kBreathPeriod-1-ms) * deltaBrightness)/(kBreathPeriod/2 - 1) + kMinBrightness;
	}

	single->setBrightness(brightness);
}


//--------------------------------------------------------------------------------
//	Animation: Heartbeat
//--------------------------------------------------------------------------------
void heartbeat(SinglePixel* single, uint32_t deltaTime)
{
	#define kHeartbeatPeriod 1000
	#define kHeartbeatOffset 250
	#define kHeartbeatWidth 200
	#define kHeartbeatBrightnessA (kMaxBrightness - 50)
	#define kHeartbeatBrightnessB (kMaxBrightness - 150)

	uint16_t ms = (deltaTime % kHeartbeatPeriod);
	uint16_t brightness = 0;

	if (ms < kHeartbeatOffset)
	{
		brightness = 0;
	}
	else
	{
		ms -= kHeartbeatOffset;

		if (ms < kHeartbeatWidth)
			brightness = CalcTriangleWave(ms, kHeartbeatWidth, kMinBrightness, kHeartbeatBrightnessA);
		else if (ms < 2*kHeartbeatWidth)
			brightness = CalcTriangleWave(ms, kHeartbeatWidth, kMinBrightness, kHeartbeatBrightnessB);
		else
			brightness = 0;
	}

	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Ocean Wave
//--------------------------------------------------------------------------------
void oceanWave(SinglePixel* single, uint32_t time)
{
	#define kOceanWaveMinPeriod		1500
	#define kOceanWaveMaxPeriod		3500
	#define kOceanWaveMinPeriodBrightness	125
	#define kOceanWaveMaxPeriodBrightness	(kMaxBrightness - 50)
	#define kOceanWaveMinBrightness	20

	static uint16_t sWavePeriod = kOceanWaveMinPeriod;
	static uint8_t sMaxBrightness = kMaxBrightness;
	uint32_t brightness;

	// Init: Reset max brightness and bounce width
	if (time == 0 || time > gEndTime)
	{
		sWavePeriod = random(kOceanWaveMinPeriod, kOceanWaveMaxPeriod);

		uint32_t a = sWavePeriod - kOceanWaveMinPeriod;
		sMaxBrightness = kOceanWaveMinPeriodBrightness +
					((kOceanWaveMaxPeriodBrightness - kOceanWaveMinPeriodBrightness) * a) /
					(kOceanWaveMaxPeriod - kOceanWaveMinPeriod);

		gStartTime = time;
		gEndTime = gStartTime + sWavePeriod;
	}

	time -= gStartTime;

	brightness = CalcTriangleWave(time, sWavePeriod, kOceanWaveMinBrightness, sMaxBrightness);
	single->setBrightness((brightness));
}
