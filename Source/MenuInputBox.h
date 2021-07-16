#pragma once

#include "MenuWidget.h"

class MenuInputBox : public MenuWidget
{
private:
	std::string m_inputBoxString;

public:
	bool isSelected;

	MenuInputBox() : MenuWidget() {}
	MenuInputBox
	(
		int width, int height, int posx, int posy,
		sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
		std::string text = "", int fontSize = 12, bool selected = true
	);

	virtual bool select(int mx, int my);
	virtual void setString(std::string string);

	void enterText(int character);
};