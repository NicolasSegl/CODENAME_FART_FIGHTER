#pragma once

#include <SFML/Graphics.hpp>
#include "utils.h"

class Character
{
private:
	vec2f m_vel;
	vec2i m_pos;
	bool m_onSurface;

	void updateKinematicStates();

public:
	sf::RectangleShape m_sprite;

	Character();

	void init();
	void update();
};