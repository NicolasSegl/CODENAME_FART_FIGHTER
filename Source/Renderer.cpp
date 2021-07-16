#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::init(sf::RenderWindow* window)
{
	m_window = window;
	m_window->create(sf::VideoMode(1024, 768), "FPG");
	//m_window->setFramerateLimit(GAME_FPS_CAP);
}

void Renderer::renderClients(Client& client)
{
	for (auto& character : client.getServerCharacters())
		if (character.isAlive && character.connected)
			m_window->draw(character.sprite);
}

void Renderer::renderWidget(MenuWidget* menuButton)
{
	m_window->draw(menuButton->getSFObject());
	m_window->draw(menuButton->getSFTextObject());
}

void Renderer::renderMenu(Menu* menu)
{
	if (!menu->menuOpen)
		return;

	m_window->draw(*(menu->getBackground()));
	for (auto& button : menu->getButtons())
		renderWidget(button);
	for (auto& inputBox : menu->getInputBoxes())
		renderWidget(inputBox);
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
	m_window->draw(level->levelBGSprite);
}