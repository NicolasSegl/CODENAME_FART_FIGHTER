#pragma once

#include "vec2f.h"
#include "vec2i.h"

#include <string>

const int GAME_FPS_CAP = 60;
extern std::string CURRENT_WORKING_DIRECTORY;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

void clamp(float* valueToClamp, float clampValue, float low, float high);