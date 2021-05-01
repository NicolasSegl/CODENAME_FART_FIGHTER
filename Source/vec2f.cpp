#include "vec2f.h"

vec2f& vec2f::operator+=(const vec2f& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

vec2f& vec2f::operator-=(const vec2f& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

vec2f vec2f::operator+(const vec2f& vec)
{
	return vec2f(x + vec.x, y + vec.y);
}

vec2f vec2f::operator-(const vec2f& vec)
{
	return vec2f(x - vec.x, y - vec.y);
}