#include "MenuButton.h"
#include <iostream>

MenuButton::MenuButton
(
	int width, int height, int posx, int posy,
	sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
	std::string text, int fontSize
) 
	: MenuWidget(width, height, posx, posy, fillColour, outlineColour, outlineWidth, text, fontSize)
{
	if (text.length() > 0)
		setText(text);
}

void MenuButton::setText(std::string text)
{
	m_text.setString(text);

	// now actually position the text to the middle of the button
	int textXPos = m_widget.getPosition().x - m_text.getLocalBounds().width / 2 + m_widget.getSize().x / 2;
	int textYPos = m_widget.getPosition().y - m_text.getLocalBounds().height / 2 + m_widget.getSize().y / 2;
	m_text.setPosition(sf::Vector2f(textXPos, textYPos));
}