#pragma once
struct vec2f
{
	float x, y;
	vec2f(float a = 0, float b = 0)
		: x(a), y(b) {}

	vec2f& operator+=(const vec2f& vec);
	vec2f& operator-=(const vec2f& vec);
	vec2f operator+(const vec2f& vec);
	vec2f operator-(const vec2f& vec);
	bool operator==(const vec2f& vec);
	bool operator!=(const vec2f& vec);
};