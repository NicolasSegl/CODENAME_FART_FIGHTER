#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "MenuWidget.h"

class MenuButton : public MenuWidget
{
public:
	MenuButton() : MenuWidget() {}
	MenuButton
	(
		int width, int height, int posx, int posy,
		sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
		std::string text = "", int fontSize = 12
	);

	virtual void setText(std::string text);
};