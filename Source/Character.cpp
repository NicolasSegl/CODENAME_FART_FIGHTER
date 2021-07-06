#include "Character.h"
#include "Packet.h"

#include <iostream>

const int MAX_CHARACTER_HORIZONTAL_VELOCITY = 7;
const float GRAVITY = 0.5;

Character::Character(int setID)
{
	init(setID);
}

void Character::init(int newid)
{
	id = newid;
	sprite.setSize(sf::Vector2f(50, 100));
	sprite.setFillColor(sf::Color::Black);
	m_pos = { 500, 300 };
	m_vel = { 0, 0 };
	sprite.setPosition(m_pos.x, m_pos.y);
	m_onSurface = false;
	isAlive = true;
}

void Character::update(std::vector<sf::RectangleShape>& platforms)
{
	updateKinematicStates(platforms);
}

void Character::handleUserInput()
{
	// adjust the velocity of the character
	bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	m_isJumping = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

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
}

// this also handles gravity and the player falling because ofi t
void Character::handlePlatformCollision(std::vector<sf::RectangleShape>& platforms)
{
	m_onSurface = false;

	// 768 = window height. 100 = character_height
	if (m_pos.y + sprite.getSize().y >= 768)
	{
		m_pos.y = 768 - 100;
		m_onSurface = true;
	}

	int characterLeft = m_pos.x;
	int characterRight = m_pos.x + sprite.getSize().x;
	int characterTop = m_pos.y;
	int characterBottom = m_pos.y + sprite.getSize().y;

	// iterate through all of the level's platforms and check for collision
	// DO NOTHING IF IT HITS THE CORNER

	for (auto& platform : platforms)
	{
		int platformLeft   = platform.getPosition().x;
		int platformRight  = platform.getPosition().x + platform.getSize().x;
		int platformTop    = platform.getPosition().y;
		int platformBottom = platform.getPosition().y + platform.getSize().y;

		// check to see if the character is on the top or bottom of the platform
		if (characterLeft < platformRight - 5 && characterRight > platformLeft + 5)
		{
			if (characterBottom >= platformTop && characterBottom <= platformTop + 25 && m_vel.y > 0)
			{
				m_pos.y = platformTop - sprite.getSize().y;
				m_onSurface = true;

				// update the character collision variables after chaning our m_pos
				characterTop = m_pos.y;
				characterBottom = m_pos.y + sprite.getSize().y;
			}
			else if (characterTop <= platformBottom && characterTop >= platformBottom - 25 && m_vel.y < 0)
			{
				m_pos.y = platformBottom;
				m_vel.y = 0;
				characterTop = m_pos.y;
				characterBottom = m_pos.y + sprite.getSize().y;
			}
		}

		// check to see if the character is colliding with the sides of the platform
		// if so, then ensure that the player cannot move in that direction
		if (characterTop < platformBottom - 5 && characterBottom > platformTop + 5)
		{
			// colliding with right side?
			if (characterLeft <= platformRight && characterRight >= platformRight && m_vel.x < 0)
			{
				m_pos.x = platformRight;
				m_vel.x = 0;
			}
			// colliding with left side?
			else if (characterRight >= platformLeft && characterLeft <= platformLeft && m_vel.x > 0)
			{
				m_pos.x = platformLeft - sprite.getSize().x;
				m_vel.x = 0;
			}

			// update the character collision variables after chaning our m_pos
			characterLeft = m_pos.x;
			characterRight = m_pos.x + sprite.getSize().x;
		}
	}

	if (!m_onSurface)
		m_vel.y += GRAVITY;
	else
		m_vel.y = 0;

	if (m_onSurface && m_isJumping)
		m_vel.y = -15;
}

void Character::updateKinematicStates(std::vector<sf::RectangleShape>& platforms)
{
	handleUserInput();
	handlePlatformCollision(platforms);

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

// if the character's state has changed in any way (animation, movement, etc), then return true
bool Character::hasChangedState()
{
	static vec2i oldPosition = m_pos;

	if (oldPosition != m_pos)
	{
		oldPosition = m_pos;
		return true;
	}
	
	return false;
}

// note that this copy instructor is crucial for sending the initial characters to the clients
// when the server starts. without it, reading violations are thrown when trying to copy
Character::Character(const Character& copyCharacter)
{
	*this = copyCharacter;
}

// this is used mainly for sending the character list to clients when a new client connects
Character& Character::operator=(const Character& assignedCharacter)
{
	if (this == &assignedCharacter)
		return *this;
	
	m_pos = assignedCharacter.m_pos;
	id = assignedCharacter.id;
	isAlive = assignedCharacter.isAlive;
	connected = assignedCharacter.connected;

	// for some reason, SFML does not like copying one sprite into another
	// and so i am manually doing the parts we need
	sprite.setPosition(assignedCharacter.sprite.getPosition());
	sprite.setSize(assignedCharacter.sprite.getSize());
	sprite.setFillColor(assignedCharacter.sprite.getFillColor());

	return *this;
}