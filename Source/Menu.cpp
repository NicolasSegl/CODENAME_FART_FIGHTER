#include "Menu.h"
#include "utils.h"

#include <iostream>

void Menu::addButton
(
	int width, int height, int posx, int posy,
	sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
	std::string text, int fontSize
)
{
	MenuButton* newMenuButton = new MenuButton(width, height, posx, posy, fillColour, outlineColour, outlineWidth, text, fontSize);
	m_buttons.push_back(newMenuButton);
}

void Menu::addTextInputBox
(
	int width, int height, int posx, int posy,
	sf::Color fillColour, sf::Color outlineColour, int outlineWidth, int fontSize
)
{
	MenuInputBox* newInputBox = new MenuInputBox(width, height, posx, posy, fillColour, outlineColour, outlineWidth, "", fontSize);
	m_inputBoxes.push_back(newInputBox);
}

void Menu::setBackgroundFromFile(std::string imageName)
{
	static sf::Texture texture;
	if (!texture.loadFromFile(imageName))
		std::cout << "Failed to load menu background image\n";

	m_backgroundImage->setTexture(texture);
}

void Menu::setBackground(sf::Color colour)
{
	m_backgroundShape.setFillColor(colour);
	m_backgroundShape.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
}

void Menu::setBackgroundSize(int newWidth, int newHeight)
{
	if (!m_backgroundImage)
		m_backgroundShape.setSize(sf::Vector2f(newWidth, newHeight));
}

sf::Drawable* Menu::getBackground()
{
	if (m_backgroundImage)
		return m_backgroundImage;
	return &m_backgroundShape;
}

int Menu::select(int mx, int my)
{
	// first see if any of the menus buttons were selected
	for (int button = 0; button < m_buttons.size(); button++)
		if (m_buttons[button]->select(mx, my))
			return button;

	// -1 signifies that no button was selected
	return -1;
}

void Menu::enterText(char character)
{
	for (auto& inputBox : m_inputBoxes)
		if (inputBox->isSelected)
			inputBox->enterText(character);
}