#pragma once

#include "utils.h"
#include "Client.h"
#include "Server.h"
#include "Renderer.h"
#include "MenuHandler.h"

class App
{
private:
	vec2i m_windowDimensions;
	sf::RenderWindow m_window;
	bool m_running;
	Renderer m_renderer;
	MenuHandler m_menuHandler;

	Client m_client;
	Server m_server;

	void handleEvents();
	void close();

public:
	App();

	void run();
};