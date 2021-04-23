#include "Character.h"

const int MAX_CHARACTER_HORIZONTAL_VELOCITY = 7;

Character::Character()
{
	init();
}

void Character::init()
{
	m_sprite.setSize(sf::Vector2f(50, 100));
	m_sprite.setFillColor(sf::Color::Black);
	m_pos = { 500, 500 };
	m_vel = { 0, 0 };
	m_sprite.setPosition(m_pos.x, m_pos.y);
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

	if (!m_onSurface)
		m_vel.y += 9.8;
	else
		m_vel.y = 0;

	// update the position of the character
	m_pos.x += m_vel.x;
	m_pos.y += m_vel.y;
	m_sprite.setPosition(m_pos.x, m_pos.y);
}