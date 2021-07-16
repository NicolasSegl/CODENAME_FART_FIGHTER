#pragma once
#include <SFML/Graphics.hpp>

#include "Level.h"
#include "Client.h"
#include "MenuButton.h"
#include "Menu.h"

class Renderer
{
private:
	sf::RenderWindow* m_window;

public:
	Renderer();

	void init(sf::RenderWindow* window);
	
	void renderClients(Client& client);
	void renderLevel(Level* level);
	void renderWidget(MenuWidget* menuButton);
	void renderMenu(Menu* menu);

	void updateWindow();
	void clearWindow(sf::Color colour = sf::Color::White);
};