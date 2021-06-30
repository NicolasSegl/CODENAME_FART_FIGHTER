#pragma once

#include <SFML/Graphics.hpp>

#include "utils.h"
#include "Level.h"

class EntityUpdatePacket;

class Character
{
private:
	vec2f m_vel;
	vec2i m_pos;
	bool m_onSurface;
	bool m_isJumping;

	void updateKinematicStates(std::vector<sf::RectangleShape>& platforms);
	void handleUserInput();
	void handlePlatformCollision(std::vector<sf::RectangleShape>& platforms);

public:
	sf::RectangleShape sprite;
	int id;
	bool isAlive;

	Character(int id = -1);
	Character(const Character& copyCharacter);
	Character& operator=(const Character& assignedCharacter);

	vec2i getPos() { return m_pos; }
	void setPos(vec2i newPos) { m_pos = newPos; }

	void updateFromServer(EntityUpdatePacket* packet);

	void init(int newId = -1);
	void update(std::vector<sf::RectangleShape>& platforms);
};