#include "utils.h"

void clamp(float* valueToClamp, float clampValue, float low, float high)
{
	*valueToClamp += clampValue;
	if (*valueToClamp < low)
		*valueToClamp = low;
	if (*valueToClamp > high)
		*valueToClamp = high;
}