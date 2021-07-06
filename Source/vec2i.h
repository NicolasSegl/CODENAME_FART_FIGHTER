#pragma once

struct vec2i
{
	int x, y;
	vec2i(int a = 0, int b = 0)
		: x(a), y(b) {}

	vec2i& operator+=(const vec2i& vec);
	vec2i& operator-=(const vec2i& vec);
	vec2i operator+(const vec2i& vec);
	vec2i operator-(const vec2i& vec);
	bool operator==(const vec2i& vec);
	bool operator!=(const vec2i& vec);
};