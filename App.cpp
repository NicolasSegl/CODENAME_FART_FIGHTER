#include "App.h"
#include "Character.h"

#include "Client.h"
#include "Server.h"

#include <iostream>
#include <string>

App::App()
{
	// default dimensions will be 4:3 master race aspsect ratio
	// enhanced with the yet more superior resoltuin of 1024x768
	m_windowDimensions = { 1024, 768 };
	m_window.create(sf::VideoMode(m_windowDimensions.x, m_windowDimensions.y), "FPG");
	m_running = true;
	m_window.setFramerateLimit(GAME_FPS_CAP);
}

void App::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
			m_running = false;
		}
	}
}

void foo()
{

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
		//std::cout << "attemping to host server on ip " << sf::IpAddress::getLocalAddress().toString() << std::endl;
		//m_server.start();
	}

	std::thread thread(foo);

	// hit it twice
	while (m_running)
	{
		handleEvents();
		m_window.clear(sf::Color::White);

		if (input == 's')
		{
			m_server.update();
			m_server.renderClients(m_window);
		}
		else if (input == 'c')
			m_client.update();

		m_window.display();
	}

	// should close the other threads from server/client
}