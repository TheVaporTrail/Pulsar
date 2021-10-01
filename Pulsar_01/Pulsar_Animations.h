//--------------------------------------------------------------------------------
//	Pulsar: Animations
//
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#include "SinglePixel.h"

typedef struct tag_animationContext {
	uint32_t startTime;
	uint32_t endTime;
	uint32_t startLevel;
	uint32_t endLevel;
	uint32_t brightness;
	uint32_t a;
	uint32_t b;
} animationContext_t;

typedef void (*animationFunc_t)(SinglePixel* single, uint32_t time, animationContext_t* context);

animationFunc_t* getAnimationList(void);

