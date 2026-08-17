//--------------------------------------------------------------------------------
//	Pulsar
//
//	Animations for a single LED
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#include "Pulsar_Animations.h"

//--------------------------------------------------------------------------------
//	Hardware Selection
//--------------------------------------------------------------------------------
// Uncomment if using an LED on a PWM pin
#define HW_USE_ANALOG_OUTPUT 1

// Uncomment if using a Neopixel
//#define HW_USE_NEOPIXEL 1

//--------------------------------------------------------------------------------
//	Hardware Header file and configure
//--------------------------------------------------------------------------------
#ifdef HW_USE_ANALOG_OUTPUT
	#include "SinglePixel_AnalogPin.h"
	#define HW_ANALOG_LED_PIN 3
#endif

#ifdef HW_USE_NEOPIXEL
	#include "SinglePixel_SingleNeopixel.h"
	#define HW_NEOPIXEL_PIN 6
#endif

//--------------------------------------------------------------------------------
//	System Globals
//--------------------------------------------------------------------------------
SinglePixel* single = NULL;

//--------------------------------------------------------------------------------
//	Animation Settings
//--------------------------------------------------------------------------------
// Millisecond delay between calls to animations. Changing this value does not
// change the animation speed.
#define kDelayInterval 30

// Time, in seconds, to run each animation
#define kAnimationTimeSeconds 25

//--------------------------------------------------------------------------------
//	Animation Globals
//--------------------------------------------------------------------------------
// Time, in ms.
// In this app we are resetting this at the beginning of each animation, but 
// that is not required.
uint32_t gStartTime;

// Animation function list
animationFunc_t* gAnimations = NULL;
// Current animation index
uint16_t gAnimationIdx = 0;
// Current animation function
animationFunc_t gCurrentAnimation = NULL;

// Context (ie, data) for the current animation.
// We need a context for each animation if running multiple animations simultaneously
animationContext_t gAnimationContext;

//--------------------------------------------------------------------------------
//	Prototypes
//--------------------------------------------------------------------------------
void initAnimationList(void);
void nextAnimation(void);
void runAnimation(void);

//--------------------------------------------------------------------------------
//	Setup
//--------------------------------------------------------------------------------
void setup()
{
	gStartTime = millis();
	
	// Load the list of animations, point to the first one, and reset the context
	initAnimationList();
	
	// Create the SinglePixel object that will handle updating the LED
	#ifdef HW_USE_ANALOG_OUTPUT
	single = new SinglePixel_AnalogPin(HW_ANALOG_LED_PIN);
	#endif

	#ifdef HW_USE_NEOPIXEL
	single = new SinglePixel_SingleNeopixel(HW_NEOPIXEL_PIN);
	// Set the color that will be used for all the animations
	single->setColor(0x00ffff);
	#endif
}

//--------------------------------------------------------------------------------
//	Loop
//--------------------------------------------------------------------------------
void loop()
{
	runAnimation();
	delay(kDelayInterval);
}

//--------------------------------------------------------------------------------
//	Init Animation List
//--------------------------------------------------------------------------------
void initAnimationList(void)
{
	gAnimations = getAnimationList();
	gAnimationIdx = 0;
	gCurrentAnimation = gAnimations[gAnimationIdx];
	
	memset(&gAnimationContext, 0, sizeof(gAnimationContext));
}

//--------------------------------------------------------------------------------
//	Next Animation
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
void runAnimation(void)
{
	uint32_t now = millis();
	uint32_t deltaTime = now - gStartTime;

	if (deltaTime > kAnimationTimeSeconds*1000)
	{
		gStartTime = now;
		deltaTime = 0;
		nextAnimation();
	}

	gCurrentAnimation(single, deltaTime, &gAnimationContext);
}

