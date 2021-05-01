#include "App.h"
#include "Character.h"

#include "Client.h"
#include "Server.h"

#include <iostream>
#include <string>

App::App()
{
	// default dimensions will be 4:3 master race aspsect ratio
	// enhanced with the yet more superior resolution of 1024x768
	m_running = true;
	m_renderer.init(&m_window);
}

void App::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			// when the window is closed, ensure that the client/server shutdown properly
			m_client.disconnect();
			m_server.shutdown();

			m_window.close();
			m_running = false;
		}
	}
}

void App::run()
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
	}

	// hit it twice
	while (m_running)
	{
		handleEvents();
		m_renderer.clearWindow();

		if (input == 's')
		{
	//		m_server.update();
			m_server.renderClients(m_window);
		}
		else if (input == 'c')
		{
			m_client.update();
			m_renderer.renderClients(m_client);
		}

		m_renderer.updateWindow();
	}

	// should close the other threads from server/client
}