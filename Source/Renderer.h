#pragma once
#include <SFML/Graphics.hpp>

#include "Level.h"
#include "Client.h"

class Renderer
{
private:
	sf::RenderWindow* m_window;

public:
	Renderer();

	void init(sf::RenderWindow* window);
	
	void renderClients(Client& client);
	void renderLevel(Level* level);

	void updateWindow();
	void clearWindow(sf::Color colour = sf::Color::White);
};