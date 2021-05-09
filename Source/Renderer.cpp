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
//	m_window->draw(client.character->sprite);
	for (auto& character : client.getServerCharacters())
	{
		m_window->draw(character.sprite);
	}
}

void Renderer::updateWindow()
{
	m_window->display();
}

void Renderer::clearWindow(sf::Color colour)
{
	m_window->clear(colour);
}