#include "App.h"
#include "Character.h"

#include "Client.h"
#include "Server.h"
#include "Level.h"

#include <iostream>
#include <string>

App::App()
{
	// default dimensions will be 4:3 master race aspsect ratio
	// enhanced with the yet more superior resolution of 1024x768
	m_running = true;
	m_renderer.init(&m_window);
}

void App::close()
{
	// when the window is closed, ensure that the client/server shutdown properly

	if (m_client.connected)
		m_client.disconnect();
	if (m_server.isHosting)
		m_server.shutdown();

	// also close the window and simply exit the program here
	m_window.close();
	exit(0);
}

void App::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
		if (event.type == sf::Event::Closed)
			close();
}

// ask the user if this will be a server of client, then setup accordingly
void App::setup()
{
	std::cout << "s for server, c for client: ";
	char input;
	std::cin >> input;
	std::cin.ignore();

	if (input == 'c')
	{
		std::cout << "enter the server ip address: ";
		std::string ipString;
		std::getline(std::cin, ipString);
		m_client.connectToIp(ipString);
		std::cout << m_client.id << std::endl;
	}
	else if (input == 's')
	{
		std::cout << "Enter server ip to be hosted on: ";
		std::string ip;
		std::getline(std::cin, ip);
		m_server.start(ip);
		m_client.connectToIp(ip);
	}
}

void App::run()
{
	setup();
	
	while (m_running)
	{
		handleEvents();
		m_renderer.clearWindow();

		m_client.update();

		// render clients AFTER rendering the level, and only if the client has actually connected
		if (m_client.connected)
		{
			m_renderer.renderLevel(m_server.getLevel());
			m_renderer.renderClients(m_client);
		}

		m_renderer.updateWindow();
	}
}