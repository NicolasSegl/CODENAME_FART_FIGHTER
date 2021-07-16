#include "MenuWidget.h"

#include <iostream>

MenuWidget::MenuWidget()
{
	static sf::Font font;
	if (!font.loadFromFile("NotoSerif-Regular.ttf"))
		std::cout << "failed to load ttf font file\n";
	else
		m_text.setFont(font);
}

MenuWidget::MenuWidget(int width, int height, int posx, int posy,
	sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
	std::string text, int fontSize)
	: MenuWidget()
{
	create(width, height, posx, posy, fillColour, outlineColour, outlineWidth, text, fontSize);
}

void MenuWidget::create(int width, int height, int posx, int posy,
	sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
	std::string text, int fontSize)
{
	m_widget.setSize(sf::Vector2f(width, height));
	m_widget.setPosition(sf::Vector2f(posx, posy));
	m_widget.setFillColor(fillColour);
	m_widget.setOutlineThickness(outlineWidth);
	m_widget.setOutlineColor(outlineColour);

	m_text.setCharacterSize(height / 4);
	m_text.setCharacterSize(fontSize);
	m_text.setFillColor(sf::Color::Black);
}

bool MenuWidget::select(int mx, int my)
{
	if (mx > m_widget.getPosition().x && mx < m_widget.getPosition().x + m_widget.getSize().x &&
		my > m_widget.getPosition().y && my < m_widget.getPosition().y + m_widget.getSize().y)
		return true;
	return false;
}