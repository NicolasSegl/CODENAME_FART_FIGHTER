#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::init(sf::RenderWindow* window)
{
	m_window = window;
	//m_windowDimensions = { 1024, 768 };
	m_window->create(sf::VideoMode(1024, 768), "FPG");
	m_window->setFramerateLimit(GAME_FPS_CAP);
}

void Renderer::renderClients(Client& client)
{
	for (auto& character : client.getServerCharacters())
		if (character.isAlive)
			m_window->draw(character.sprite);
}

void Renderer::updateWindow()
{
	m_window->display();
}

void Renderer::clearWindow(sf::Color colour)
{
	m_window->clear(colour);
}

void Renderer::renderLevel(Level* level)
{
	for (auto& platform : level->getPlatforms())
		m_window->draw(platform);
}