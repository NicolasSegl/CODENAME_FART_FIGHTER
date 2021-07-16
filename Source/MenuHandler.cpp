#include "MenuHandler.h"
#include "utils.h"

#include <iostream>
#include <string>

// macros for different menus (to index them easier)
#define MAIN_MENU 0
#define HOST_SERVER_MENU 1
#define CONNECT_SERVER_MENU 2

// macros for different menu selections

// main menu
#define HOST_SERVER_SELECTED 0
#define CONNECT_SERVER_SELECTED 1

#define UNICODE_ENTER 0xD

MenuHandler::MenuHandler()
{
	initializeMainMenu();
	aMenuIsOpen = true;
}

void MenuHandler::initializeMainMenu()
{
	Menu* mainMenu = new Menu;
	mainMenu->menuOpen = true;
	mainMenu->setBackground(sf::Color::White);
	mainMenu->addButton(300, 150, WINDOW_WIDTH / 2 - 300 / 2, WINDOW_HEIGHT / 2 - 150 / 2,
						 sf::Color::White, sf::Color::Black, 5, "Host Server", 24);
	mainMenu->addButton(300, 150, WINDOW_WIDTH / 2 - 300 / 2, WINDOW_HEIGHT / 2 - 150 / 2 + 200,
		sf::Color::White, sf::Color::Black, 5, "Connect To Server", 24);
	m_menus.push_back(mainMenu);

	Menu* hostServerMenu = new Menu;
	hostServerMenu->setBackground(sf::Color::White);
	hostServerMenu->addTextInputBox(500, 150, WINDOW_WIDTH / 2 - 500 / 2, WINDOW_HEIGHT / 2 - 150 / 2, sf::Color::White, sf::Color::Black, 5, 36, true);
	m_menus.push_back(hostServerMenu);

	Menu* connectServerMenu = new Menu;
	connectServerMenu->setBackground(sf::Color::White);
	connectServerMenu->addTextInputBox(500, 150, WINDOW_WIDTH / 2 - 500 / 2, WINDOW_HEIGHT / 2 - 150 / 2, sf::Color::White, sf::Color::Black, 5, 36, true);
	m_menus.push_back(connectServerMenu);
}

void MenuHandler::select(int mx, int my)
{
	if (m_menus[MAIN_MENU]->menuOpen)
		switch (m_menus[MAIN_MENU]->select(mx, my))
		{
			case HOST_SERVER_SELECTED:
			{
				m_menus[MAIN_MENU]->menuOpen = false;
				m_menus[HOST_SERVER_MENU]->menuOpen = true;
				break;
			}
			case CONNECT_SERVER_SELECTED:
			{
				m_menus[MAIN_MENU]->menuOpen = false;
				m_menus[CONNECT_SERVER_MENU]->menuOpen = true;
				break;
			}
			default:
				return;
		}
}

MenuResponseData* MenuHandler::inputText(int character, int menu)
{
	if (m_menus[menu]->menuOpen)
	{
		if (character == UNICODE_ENTER)
			for (auto& inputBox : m_menus[menu]->getInputBoxes())
				if (inputBox->isSelected)
				{
					IPResponse* ipResponse = new IPResponse;

					if (menu == HOST_SERVER_MENU)
						ipResponse->response = MenuResponse::HostOnIp;
					else if (menu == CONNECT_SERVER_MENU)
						ipResponse->response = MenuResponse::ConnectToIp;

					std::strcpy(ipResponse->ip, inputBox->getSFTextObject().getString().toAnsiString().c_str());
					return ipResponse;
				}

		m_menus[menu]->enterText(character);
	}

	return nullptr;
}

MenuResponseData* MenuHandler::enterText(int character)
{
	if (m_menus[HOST_SERVER_MENU]->menuOpen)
		return inputText(character, HOST_SERVER_MENU);
	else if (m_menus[CONNECT_SERVER_MENU]->menuOpen)
		return inputText(character, CONNECT_SERVER_MENU);

	return nullptr;
}

void MenuHandler::close()
{
	aMenuIsOpen = false;
	for (auto& menu : m_menus)
		menu->menuOpen = false;
}