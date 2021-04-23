#pragma once

#include "vec2f.h"
#include "vec2i.h"

const int GAME_FPS_CAP = 60;

void clamp(float* valueToClamp, float clampValue, float low, float high);