#pragma once

#include <vector>
#include <string>

#include "MenuButton.h"
#include "MenuInputBox.h"

class Menu
{
private:
	std::vector<MenuButton*> m_buttons;
	std::vector<MenuInputBox*> m_inputBoxes;

	sf::Sprite* m_backgroundImage = nullptr;
	sf::RectangleShape m_backgroundShape;

public:
	bool menuOpen = false;

	void addButton
	(
		int width, int height, int posx, int posy,
		sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
		std::string text = "", int fontSize = 12
	);

	void addTextInputBox
	(
		int width, int height, int posx, int posy,
		sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
		int fontSize = 12
	);

	void setBackgroundFromFile(std::string imageName);
	void setBackground(sf::Color colour);
	void setBackgroundSize(int newWidth, int newHeight);

	int select(int mx, int my);
	void enterText(char character);

	sf::Drawable* getBackground();
	std::vector<MenuButton*>& getButtons() { return m_buttons; }
	std::vector<MenuInputBox*>& getInputBoxes() { return m_inputBoxes; }
};