#include "App.h"
#include "Character.h"
#include "MenuHandler.h"
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
	{
		if (event.type == sf::Event::Closed)
			close();
		else if (event.type == sf::Event::MouseButtonPressed && m_menuHandler.aMenuIsOpen)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				int mx = sf::Mouse::getPosition(m_window).x;
				int my = sf::Mouse::getPosition(m_window).y;
				m_menuHandler.select(mx, my);
			}
		}
		else if (event.type == sf::Event::TextEntered)
		{
			MenuResponseData* menuResponse = m_menuHandler.enterText(event.text.unicode);
			if (menuResponse == nullptr)
				continue;

			switch (menuResponse->response)
			{
				case MenuResponse::HostOnIp:
				case MenuResponse::ConnectToIp:
				{
					if (m_client.connected)
						m_client.disconnect();
					if (m_server.isHosting)
						m_server.shutdown();

					IPResponse* ipResponse = (IPResponse*)(menuResponse);

					if (menuResponse->response == MenuResponse::HostOnIp)
						m_server.start(std::string(ipResponse->ip));

					if (m_client.connectToIp(std::string(ipResponse->ip)))
						m_menuHandler.close();
					
					delete ipResponse;
					break;
				}
				default:
					break;
			}
		}
	}
	// if enter ip menu is open...
}

void App::run()
{
	// these will be used to artificially lower the framerate, which will greatly decrease lag
	std::chrono::system_clock::time_point currentFrame = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point lastFrame    = std::chrono::system_clock::now();;
	std::chrono::duration<double, std::milli> delta; // difference of currentFrame and lastFrame

	while (m_running)
	{
		currentFrame = std::chrono::system_clock::now();
		delta = currentFrame - lastFrame;

		handleEvents();
		// do all of this only if main menu is not open:

		m_client.serviceHost();

		// this ensures that the character is only updated 60 times a second 
		if (delta.count() >= 1000 / GAME_FPS_CAP) // 1000 being the number of milliseconds
		{
			//m_renderer.clearWindow();
			lastFrame = currentFrame;

			m_renderer.renderLevel(m_client.getLevel());

			if (m_client.connected)
			{
				if (m_client.getLevel()->loaded && !m_menuHandler.aMenuIsOpen)
					m_client.updateCharacter();

				// render clients AFTER rendering the level, and only if the client has actually connected
				m_renderer.renderClients(m_client);
				m_client.sendData();
			}

			if (m_menuHandler.aMenuIsOpen)
				for (auto& menu : m_menuHandler.getMenus())
					m_renderer.renderMenu(menu);

			m_renderer.updateWindow();
		}
	}
}