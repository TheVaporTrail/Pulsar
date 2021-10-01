//--------------------------------------------------------------------------------
//	Pulsar: Animations
//
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#include "Pulsar_Animations.h"
#include "AnimationUtils.h"


//--------------------------------------------------------------------------------
//	Animation Settings
//--------------------------------------------------------------------------------
#define kMaxBrightness 255
#define kMinBrightness 0
#define kDeltaBrightness  (kMaxBrightness - kMinBrightness)

#define kAnimationPeriod	5000
#define kAnimationPeriod2 	500


//--------------------------------------------------------------------------------
//	Animation Prototypes
//--------------------------------------------------------------------------------
void anim_PixelOff(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_SawtoothWave(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_TriangleWave(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_SineWave(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_TrianglePulse(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_TriangleWaveBlink(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_PulseTrain(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_RandomLevel(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_RandomLevelFade(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_RandomWalkFade(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_SummedTriangleWaves(SinglePixel* single, uint32_t time, animationContext_t* context);

void anim_Breath(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_Heartbeat(SinglePixel* single, uint32_t time, animationContext_t* context);

void anim_Bounce(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_CandleFlicker(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_OceanWave(SinglePixel* single, uint32_t time, animationContext_t* context);
void anim_ModulatedSineWave(SinglePixel* single, uint32_t time, animationContext_t* context);


//--------------------------------------------------------------------------------
//	Animation Function List
//--------------------------------------------------------------------------------
static animationFunc_t sAnimationFunctions[] = {
	anim_ModulatedSineWave,
	anim_OceanWave,
	anim_CandleFlicker,
	anim_Breath,
	anim_Heartbeat,
	anim_RandomLevelFade,
	anim_Bounce,
	anim_PulseTrain,
	anim_RandomLevel,
	anim_TrianglePulse,
	anim_SummedTriangleWaves,
	anim_SawtoothWave,
	anim_RandomWalkFade,
	anim_TriangleWave,
	anim_SineWave,
	anim_TriangleWaveBlink,
	NULL
};

//--------------------------------------------------------------------------------
//	Get Animation List
//--------------------------------------------------------------------------------
animationFunc_t* getAnimationList(void)
{
	return sAnimationFunctions;
}

//--------------------------------------------------------------------------------
//	Animation: Pixel Off
//--------------------------------------------------------------------------------
void anim_PixelOff(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	single->setBrightness(0);
}

//--------------------------------------------------------------------------------
//	Animation: Triangle Wave
//--------------------------------------------------------------------------------
void anim_TriangleWave(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	uint32_t brightness; 
	
	brightness = CalcTriangleWave(time, kAnimationPeriod, kMinBrightness, kMaxBrightness);
	
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Sawtooth Wave
//--------------------------------------------------------------------------------
void anim_SawtoothWave(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	uint32_t brightness; 
	
	brightness = CalcRamp(time, kAnimationPeriod, kMinBrightness, kMaxBrightness);
	
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Sine Wave
//--------------------------------------------------------------------------------
void anim_SineWave(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	// First, calculate the brightness as a ramp
	uint32_t brightness = CalcRamp(time, kAnimationPeriod, kMinBrightness, kMaxBrightness);
	
	// Then convert the ramp value to a sine value.
	brightness = sine8t(brightness);
	
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Triangle Pulse
//	Short triangle pulse once during the period
//--------------------------------------------------------------------------------
void anim_TrianglePulse(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	uint32_t brightness; 
	time = (time % kAnimationPeriod);
	
	if (time <= kAnimationPeriod2)
		brightness = CalcTriangleWave(time, kAnimationPeriod2, kMinBrightness, kMaxBrightness);
	else
		brightness = 0;
		
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Triangle Wave Blink
//	Blinking, where the brightness following a triangle wave 
//--------------------------------------------------------------------------------
void anim_TriangleWaveBlink(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	uint32_t ms = (time % kAnimationPeriod);
	uint32_t m2 = (ms % kAnimationPeriod2);
	
	uint32_t brightness; 
	
	brightness = CalcTriangleWave(ms, kAnimationPeriod, kMinBrightness, kMaxBrightness);
		
	if (m2 < kAnimationPeriod2/2)
		brightness = 0;
	
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Pulse Train
//--------------------------------------------------------------------------------
void anim_PulseTrain(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	uint32_t ms = (time % kAnimationPeriod);
	uint32_t m2 = (ms % kAnimationPeriod2);
	uint32_t i  = ms/kAnimationPeriod2; // Subperiod number
	
	uint32_t brightness;

	// Init: Random number to represent eight random pulses
	if (time == 0)
		context->a = random(255);

	// If the bit is set, then triangle wave
	if ( (context->a & (1 << i)) )
		brightness = CalcTriangleWave(m2, kAnimationPeriod2, kMinBrightness, kMaxBrightness);
	// Otherwise, no output
	else
		brightness = 0;

	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Random Level
//	Change to a random level on a fixed period
//--------------------------------------------------------------------------------
void anim_RandomLevel(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	// Constant settings
	#define kRandomLevelPeriod 1000
	#define kRandomLevelCount  0x11
	

	// The first time we are called and at the end of the period
	// find a new random level
	if (time == 0 || time > context->endTime)
	{
		uint32_t b; 

		context->endTime = time + kRandomLevelPeriod;

		// Find a new level that is different from the current level.
		// To insure that there is a noticeable difference in brightness we use
		// levels or steps. Each level is (0xff/kRandomLevelCount) away from
		// the next level 
		do {
			b = (0xff/kRandomLevelCount) * random(0, kRandomLevelCount);
		} while (b == context->brightness);
		
		context->brightness = b;
		single->setBrightness(b);
	}
}

//--------------------------------------------------------------------------------
//	Animation: Random Fade
//	Fade from one random level to another over a random period
//--------------------------------------------------------------------------------
void anim_RandomLevelFade(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	// Constant settings
	#define kRandomLevelMinPeriod	750
	#define kRandomLevelMaxPeriod	2000

	// The first time we are called and at the end of the period,
	// randomly pick a new period duration and end level
	if (time == 0 || time > context->endTime)
	{
		context->endTime = time + random(kRandomLevelMinPeriod, kRandomLevelMaxPeriod);
		context->startTime = time;
		context->startLevel = context->endLevel;
		context->endLevel = random(kMinBrightness, kMaxBrightness);
	}

	// Calculate the brightness as a value between the start and end levels
	// based on how far along we are in the period
	uint32_t ms = (time - context->startTime);
	uint32_t deltaT = context->endTime - context->startTime;
	uint32_t brightness = CalcRamp(ms, deltaT, context->startLevel, context->endLevel);

	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Random Walk Fade
//	At a fixed interval, randomly increase or decrease the brightness by a fixed 
//	amount. The idea is to give the impression of a meandering walk.
//--------------------------------------------------------------------------------
void anim_RandomWalkFade(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	// Constant settings
	#define kRandomWalkPeriod 250
	#define kRandomWalkStep 12

	// At the first call or the end of the period, update the end level
	if (time == 0 || time > context->endTime)
	{
		context->endTime = time + kRandomWalkPeriod;
		context->startTime = time;
		context->startLevel = context->endLevel;

		int8_t d = random(2);

		// If we are close to the max brightness, then decrease the brightness 
		if (context->endLevel >= kMaxBrightness - kRandomWalkStep)
			context->endLevel -= kRandomWalkStep;
		// Otherwise, if we are close to the min brightness, then increase the brightness
		else if (context->endLevel <= kMinBrightness + kRandomWalkStep)
			context->endLevel += kRandomWalkStep;
		// Otherwise, randomly increase or decrease the brightness
		else if (d == 0)
			context->endLevel -= kRandomWalkStep;
		else
			context->endLevel += kRandomWalkStep;
	}

	// Calculate the brightness as a value between the start and end levels
	// based on how far along we are in the period
	uint32_t ms = (time - context->startTime);
	uint32_t brightness = CalcRamp(ms, kRandomWalkPeriod, context->startLevel, context->endLevel);

	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Triangle Fade Fade
//	Two triangle waves added together. The second wave period is much short than
//	the first.
//--------------------------------------------------------------------------------
void anim_SummedTriangleWaves(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	#define kBrightAdjust 50

	uint32_t ms = (time % kAnimationPeriod);
	uint32_t m2 = (ms % kAnimationPeriod2);
	uint32_t brightness; 

	brightness = CalcTriangleWave(ms, kAnimationPeriod, kMinBrightness, (kMaxBrightness - kBrightAdjust));
	brightness += = CalcTriangleWave(m2, kAnimationPeriod2, 0, kBrightAdjust);
	
 	brightness += adj;
	
	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Sine Fade
//--------------------------------------------------------------------------------
// NEEDS WORK
void anim_ModulatedSineWave(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	#define kFadeModulatedBaseline	128
	#define kFadeModulatedRangeA	 120
	//#define kFadeModulatedRangeB	 20

	uint32_t ms = (time % kAnimationPeriod);

	// Calc modulation. Range is 0 to 255
	#define kFadeModulationMin 0
	uint32_t modulation = (ms * (255-kFadeModulationMin))/(kAnimationPeriod - 1) + kFadeModulationMin;
	//modulation = sine8t(modulation);

	uint32_t m2 = (ms % kAnimationPeriod2);
	uint32_t b = (m2 * 255)/(kAnimationPeriod2 - 1);
	b = (sine8t(b)*kFadeModulatedRangeA)/(255);
	//b = (b * modulation)/255;

	uint32_t brightness = kFadeModulatedBaseline /*- kFadeModulatedRangeA/2*/ + b;

	single->setBrightness(sine8t(brightness));
}

//--------------------------------------------------------------------------------
//	Animation: Bounce
//--------------------------------------------------------------------------------
// NEEDS WORK
void anim_Bounce(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	#define kBounceInitialMaxBrightness kMaxBrightness
	#define kBounceBrightnessDecrement (16)
	#define kBounceMinBrightness (96)
	#define kBounceMinWidth 20
	#define kBounceInitialWidth (kAnimationPeriod/4)
	#define kBounceDecayNum (3)
	#define kBounceDecayDen (4)
	
	// sMaxBrightness: context->a
	// sBounceWidth: context->b
	
	uint32_t brightness;
	
	// Init: Reset max brightness and bounce width
	if (time == 0 || context->b == 0)
	{
		context->startTime = time;
		context->endTime = context->startTime + context->b;
		context->a = kBounceInitialMaxBrightness;
		context->b = kBounceInitialWidth;
	}
	else if (time > context->endTime)
	{
		context->b =  (context->b * kBounceDecayNum) / kBounceDecayDen;
		//context->a = (context->a * kBounceDecayNum) / kBounceDecayDen;
		if (context->a > kBounceMinBrightness + kBounceBrightnessDecrement)
			context->a -= kBounceBrightnessDecrement;

		context->startTime = time;
		context->endTime = context->startTime + context->b;
	}
	
	if (context->b < kBounceMinWidth)
	{
		brightness = 0;
	}
	else
	{
		time -= context->startTime;

		brightness = (time * context->a)/(context->b - 1);
	}
	
	single->setBrightness(sine8t(brightness));
}

//--------------------------------------------------------------------------------
//	Animation: Candle Flicker
//--------------------------------------------------------------------------------
// NEEDS WORK
void anim_CandleFlicker(SinglePixel* single, uint32_t time, animationContext_t* context)
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

	// levelCount: context->a = kFlickerPeriods;
	// candleBrightRange: context->b = 1; // mid

	if (time == 0 || time > context->endTime)
	{
		if (context->a == 0)
		{
			uint8_t r = random(20);
			if (r < 1)
				context->b = 0; // low
			else if (r < 2)
				context->b = 2; // high
			else
				context->b = 1; // high

			context->a = kFlickerPeriods;
		}

		context->endTime = time + random(kFlickerMinPeriod, kFlickerMaxPeriod);
		context->startTime = time;
		context->startLevel = context->endLevel;
		context->a -= 1;

		// Select a new level based on the "brightness range"
		if (context->b == 0)
			context->endLevel = random(kFlickerLowBrightnessMin, kFlickerLowBrightnessMax);
		else if (context->b == 1)
			context->endLevel = random(kFlickerMidBrightnessMin, kFlickerMidBrightnessMax);
		else
			context->endLevel = random(kFlickerHighBrightnessMin, kFlickerHighBrightnessMax);
	}

	uint32_t ms = time - context->startTime;
	uint32_t deltaT = context->endTime - context->startTime;
	uint32_t brightness = CalcRamp(ms, deltaT, context->startLevel, context->endLevel);

	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Breath
//
//	https://www.e-breathing.com/normal-respiratory-rate-volume-chart/
//--------------------------------------------------------------------------------
void anim_Breath(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	#define kBreathPeriod (kAnimationPeriod/2)
	#define kBreathOffset (kAnimationPeriod/4)

	uint32_t brightness = 0; 
	
	time = (time % kAnimationPeriod);

	if (time >= kBreathOffset && time <= kBreathOffset + kBreathPeriod)
	{
		time -= kBreathOffset;
		brightness = CalcTriangleWave(time, kBreathPeriod, kMinBrightness, kMaxBrightness);
	}

	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Heartbeat
//--------------------------------------------------------------------------------
void anim_Heartbeat(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	#define kHeartbeatPeriod 1000
	#define kHeartbeatOffset 250
	#define kHeartbeatWidth 200
	#define kHeartbeatBrightnessA (kMaxBrightness - 50)
	#define kHeartbeatBrightnessB (kMaxBrightness - 150)

	uint16_t brightness = 0;
	time = (time % kHeartbeatPeriod);

	if (time >= kHeartbeatOffset)
	{
		time -= kHeartbeatOffset;

		if (time < kHeartbeatWidth)
			brightness = CalcTriangleWave(time, kHeartbeatWidth, kMinBrightness, kHeartbeatBrightnessA);
		else if (time < 2*kHeartbeatWidth)
			brightness = CalcTriangleWave(time, kHeartbeatWidth, kMinBrightness, kHeartbeatBrightnessB);
	}

	single->setBrightness(brightness);
}

//--------------------------------------------------------------------------------
//	Animation: Ocean Wave
//--------------------------------------------------------------------------------
// NEEDS WORK
void anim_OceanWave(SinglePixel* single, uint32_t time, animationContext_t* context)
{
	#define kOceanWaveMinPeriod		1500
	#define kOceanWaveMaxPeriod		3500
	#define kOceanWaveMinPeriodBrightness	125
	#define kOceanWaveMaxPeriodBrightness	(kMaxBrightness - 50)
	#define kOceanWaveMinBrightness	20

	// sWavePeriod: context->a
	// sMaxBrightness: context->b
	uint32_t brightness;

	// Init: Reset max brightness and bounce width
	if (time == 0 || time > context->endTime)
	{
		context->a = random(kOceanWaveMinPeriod, kOceanWaveMaxPeriod);

		uint32_t a = context->a - kOceanWaveMinPeriod;
		context->b = kOceanWaveMinPeriodBrightness +
					((kOceanWaveMaxPeriodBrightness - kOceanWaveMinPeriodBrightness) * a) /
					(kOceanWaveMaxPeriod - kOceanWaveMinPeriod);

		context->startTime = time;
		context->endTime = context->startTime + context->a;
	}

	time -= context->startTime;

	brightness = CalcTriangleWave(time, context->a, kOceanWaveMinBrightness, context->b);
	single->setBrightness((brightness));
}
