#include "MenuInputBox.h"

#include <iostream>

#define UNICODE_BACKSPACE 0x8

MenuInputBox::MenuInputBox
(
	int width, int height, int posx, int posy,
	sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
	std::string text, int fontSize
)
	: MenuWidget(width, height, posx, posy, fillColour, outlineColour, outlineWidth, text, fontSize)
{
}

bool MenuInputBox::select(int mx, int my)
{
	if (MenuWidget::select(mx, my))
		isSelected = true;
	else
		isSelected = false;

	return isSelected;
}

void MenuInputBox::enterText(int character)
{
	if (character == UNICODE_BACKSPACE)
		m_inputBoxString.erase(m_inputBoxString.begin() + m_inputBoxString.size() - 1);
	else
		m_inputBoxString += character;

	setString(m_inputBoxString);
}

void MenuInputBox::setString(std::string string)
{
	m_text.setString(string);

	// Position the text to be left aligned and inside of the box
	int textXPos = m_widget.getPosition().x + 5;
	int textYPos = m_widget.getPosition().y - m_text.getLocalBounds().height / 2 + m_widget.getSize().y / 2;

	m_text.setPosition(sf::Vector2f(textXPos, textYPos));
}