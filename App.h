#pragma once

#include <SFML/Graphics.hpp>
#include "utils.h"
#include "Client.h"
#include "Server.h"

class App
{
private:
	vec2i m_windowDimensions;
	sf::RenderWindow m_window;
	bool m_running;

	Client m_client;
	Server m_server;

	void handleEvents();

public:
	App();

	void run();
};