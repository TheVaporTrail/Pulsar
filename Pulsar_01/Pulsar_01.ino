//--------------------------------------------------------------------------------
//	Pulsar
//
//	Animations for a single LED
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#include "SinglePixel_AnalogPin.h"
#include "Pulsar_TimedAnimations.h"

//--------------------------------------------------------------------------------
//	Hardware Configuration
//--------------------------------------------------------------------------------
#define HW_USE_ANALOG_OUTPUT 1
#define HW_ANALOG_LED_PIN 11

//#define HW_USE_NEOPIXEL 0
//#define HW_NEOPIXEL_PIN 9

//--------------------------------------------------------------------------------
//	System Globals
//--------------------------------------------------------------------------------
SinglePixel* single = NULL;

//--------------------------------------------------------------------------------
//	Animation Settings
//--------------------------------------------------------------------------------
// Millisecond delay between calls to animations
#define kDelayInterval 30

// Time, in seconds, to run each animation
#define kAnimationTimeSeconds 25

//--------------------------------------------------------------------------------
//	Animation Globals
//--------------------------------------------------------------------------------
// Time, in ms, that the current animation started
uint32_t gStartTime;

// Animation function list
animationFunc_t* gAnimations = NULL;
// Current animation index
uint16_t gAnimationIdx = 0;
// Current animation function
animationFunc_t gCurrentAnimation = NULL;

//--------------------------------------------------------------------------------
//	Prototypes
//--------------------------------------------------------------------------------
void test(void);
void run(void);

//--------------------------------------------------------------------------------
//	Setup
//--------------------------------------------------------------------------------
void setup()
{
	gStartTime = millis();
	
	gAnimations = getTimedAnimationList();
	gAnimationIdx = 0;
	gCurrentAnimation = gAnimations[gAnimationIdx];
	
	#if HW_USE_ANALOG_OUTPUT
	single = new SinglePixel_AnalogPin(HW_ANALOG_LED_PIN);
	#endif
	
	//Serial.begin(115200);
}

//--------------------------------------------------------------------------------
//	Loop
//--------------------------------------------------------------------------------
void loop()
{
	run();
	//test();
}

//--------------------------------------------------------------------------------
//	Test
//--------------------------------------------------------------------------------
void test(void)
{
	#if HW_USE_ANALOG_OUTPUT
	pinMode(HW_ANALOG_LED_PIN, OUTPUT);
	digitalWrite(HW_ANALOG_LED_PIN, 1);
	delay(500);
	digitalWrite(HW_ANALOG_LED_PIN, 0);
	delay(500);
	#endif
}

//--------------------------------------------------------------------------------
//	next Animation
//--------------------------------------------------------------------------------
void nextAnimation(void)
{
	gAnimationIdx++;
	if (gAnimations[gAnimationIdx] == NULL)
		gAnimationIdx = 0;
		
	gCurrentAnimation = gAnimations[gAnimationIdx];
}

//--------------------------------------------------------------------------------
//	Run
//--------------------------------------------------------------------------------
void run(void)
{
	uint32_t now = millis();
	uint32_t deltaTime = now - gStartTime;

	if (deltaTime > kAnimationTimeSeconds*1000)
	{
		gStartTime = now;
		deltaTime = 0;
		nextAnimation();
	}

	gCurrentAnimation(single, deltaTime);
	
	delay(kDelayInterval);
}


