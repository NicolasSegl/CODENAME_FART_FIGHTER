#include "Character.h"
#include "Packet.h"

#include <iostream>

const int MAX_CHARACTER_HORIZONTAL_VELOCITY = 7;
const float GRAVITY = 0.5;

Character::Character(int setID)
{
	isAlive = true;
	init(setID);
}

void Character::init(int newid)
{
	id = newid;
	sprite.setSize(sf::Vector2f(50, 100));
	sprite.setFillColor(sf::Color::Black);
	m_pos = { 500, 500 };
	m_vel = { 0, 0 };
	sprite.setPosition(m_pos.x, m_pos.y);
	m_onSurface = false;
}

void Character::update()
{
	updateKinematicStates();
}

void Character::updateKinematicStates()
{
	// adjust the velocity of the character
	bool moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool isJumping = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	if (!moveLeft && !moveRight)
	{
		if (m_vel.x > 0)
			clamp(&m_vel.x, -0.5, 0, MAX_CHARACTER_HORIZONTAL_VELOCITY);
		else if (m_vel.x < 0)
			clamp(&m_vel.x, 0.5, -MAX_CHARACTER_HORIZONTAL_VELOCITY, 0);
	}
	else
	{
		if (moveLeft && m_vel.x > -MAX_CHARACTER_HORIZONTAL_VELOCITY)
			m_vel.x -= 0.5;
		if (moveRight && m_vel.x < MAX_CHARACTER_HORIZONTAL_VELOCITY)
			m_vel.x += 0.5;
	}

	// 768 = window height. 100 = character_height
	if (m_pos.y + 100 >= 768)
	{
		m_pos.y = 768 - 100;
		m_onSurface = true;
	}
	else
		m_onSurface = false;

	if (!m_onSurface)
		m_vel.y += GRAVITY;
	else
		m_vel.y = 0;

	if (m_onSurface && isJumping)
		m_vel.y = -10;

	// update the position of the character
	m_pos.x += m_vel.x;
	m_pos.y += m_vel.y;
	sprite.setPosition(m_pos.x, m_pos.y);
}

void Character::updateFromServer(EntityUpdatePacket* packet)
{
	m_pos = packet->position;
	sprite.setPosition(m_pos.x, m_pos.y);
}

// note that this copy instructor is crucial for sending the initial characters to the clients
// when the server starts. without it, reading violations are thrown when trying to copy
Character::Character(const Character& copyCharacter)
{
	m_pos = copyCharacter.m_pos;
	// for some reason, SFML does not like copying one sprite into another
	// and so i am manually doing the parts we need
	sprite.setPosition(copyCharacter.sprite.getPosition());
	sprite.setSize(copyCharacter.sprite.getSize());
	sprite.setFillColor(copyCharacter.sprite.getFillColor());

	id = copyCharacter.id;
}

// this is used mainly for sending the character list to clients when a new client connects
Character& Character::operator=(const Character& assignedCharacter)
{
	if (this == &assignedCharacter)
		return *this;
	
	m_pos = assignedCharacter.m_pos;
	// for some reason, SFML does not like copying one sprite into another
	// and so i am manually doing the parts we need
	sprite.setPosition(assignedCharacter.sprite.getPosition());
	sprite.setSize(assignedCharacter.sprite.getSize());
	sprite.setFillColor(assignedCharacter.sprite.getFillColor());
	id = assignedCharacter.id;

	return *this;
}