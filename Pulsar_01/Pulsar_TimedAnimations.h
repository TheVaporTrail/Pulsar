//--------------------------------------------------------------------------------
//	Pulsar: Timed Animations
//
//
//	2021.08.31: Created
//--------------------------------------------------------------------------------
#include "SinglePixel.h"

typedef void (*animationFunc_t)(SinglePixel* single, uint32_t deltaTime);

animationFunc_t* getTimedAnimationList(void);

