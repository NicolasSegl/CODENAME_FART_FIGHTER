#pragma once

#include "vec2f.h"
#include "vec2i.h"

#include <string>
#include <filesystem>

const int GAME_FPS_CAP = 60;
static std::string CURRENT_WORKING_DIRECTORY;

void clamp(float* valueToClamp, float clampValue, float low, float high);