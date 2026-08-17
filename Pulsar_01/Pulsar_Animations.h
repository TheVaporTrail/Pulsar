//--------------------------------------------------------------------------------
//	Pulsar: Animations
//
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#include "SinglePixel.h"

// Animation Context: temporary variables for an animation to use if it needs to
typedef struct tag_animationContext {
	uint32_t startTime;
	uint32_t endTime;
	uint32_t startLevel;
	uint32_t endLevel;
	uint32_t brightness;
	uint32_t a;
	uint32_t b;
} animationContext_t;

// Animation Function: typedef so we can create a list of function pointers
typedef void (*animationFunc_t)(SinglePixel* single, uint32_t time, animationContext_t* context);

// Get Animation List: returns an array of animation functions
animationFunc_t* getAnimationList(void);
