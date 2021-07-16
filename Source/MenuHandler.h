#pragma once

#include "Menu.h"
#include "MenuResponse.h"

class MenuHandler
{
private:
	std::vector<Menu*> m_menus;

	void initializeMainMenu();

	MenuResponseData* inputText(int character, int menu);

public:
	MenuHandler();
	bool aMenuIsOpen;

	void select(int mx, int my);
	MenuResponseData* enterText(int character);
	void close();

	std::vector<Menu*>& getMenus() { return m_menus; }
};