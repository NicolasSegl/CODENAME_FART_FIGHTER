#include "App.h"
#include "Character.h"

#include "Client.h"
#include "Server.h"
#include "Level.h"

#include <iostream>
#include <string>
#include <chrono>

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
	
	// these will be used to artificially lower the framerate, which will greatly decrease lag
	std::chrono::system_clock::time_point currentFrame = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point lastFrame    = std::chrono::system_clock::now();;
	std::chrono::duration<double, std::milli> delta; // difference of currentFrame and lastFrame

	while (m_running)
	{
		currentFrame = std::chrono::system_clock::now();
		delta = currentFrame - lastFrame;

		handleEvents();
		m_client.serviceHost();

		// this ensures that the character is only updated 60 times a second 
		if (delta.count() >= 1000 / GAME_FPS_CAP) // 1000 being the number of milliseconds
		{
			//m_renderer.clearWindow();
			lastFrame = currentFrame;

			m_renderer.renderLevel(m_client.getLevel());

			if (m_client.connected)
			{
				if (m_client.getLevel()->loaded)
					m_client.updateCharacter();
				// render clients AFTER rendering the level, and only if the client has actually connected
				m_renderer.renderClients(m_client);
				m_client.sendData();
			}

			m_renderer.updateWindow();
		}
	}
}