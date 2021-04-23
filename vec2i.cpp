#include "vec2i.h"

vec2i& vec2i::operator+=(const vec2i& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

vec2i& vec2i::operator-=(const vec2i& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

vec2i vec2i::operator+(const vec2i& vec)
{
	return vec2i(x + vec.x, y + vec.y);
}

vec2i vec2i::operator-(const vec2i& vec)
{
	return vec2i(x - vec.x, y - vec.y);
}