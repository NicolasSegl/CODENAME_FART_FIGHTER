#pragma once

#include <SFML/Graphics.hpp>

class MenuWidget
{
protected:
	sf::RectangleShape m_widget;
	sf::Text m_text;

public:
	MenuWidget();
	MenuWidget
	(
		int width, int height, int posx, int posy,
		sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
		std::string text = "", int fontSize = 36
	);

	void create
	(
		int width, int height, int posx, int posy,
		sf::Color fillColour, sf::Color outlineColour, int outlineWidth,
		std::string text = "", int fontSize = 36
	);

	virtual bool select(int mx, int my);

	void setDimensions(int width, int height) { m_widget.setSize(sf::Vector2f(width, height)); }
	void setPosition(int xpos, int ypos)	  { m_widget.setPosition(sf::Vector2f(xpos, ypos)); }
	void setFillColour(sf::Color colour)	  { m_widget.setFillColor(colour); }
	void setOutlineColour(sf::Color colour)	  { m_widget.setOutlineColor(colour); }
	void setOutlineThickness(int width)		  { m_widget.setOutlineThickness(width); }
	void setFontSize(int size)				  { m_text.setCharacterSize(size); }

	virtual void setText(std::string text) {}

	const sf::RectangleShape& getSFObject() { return m_widget; }
	const sf::Text& getSFTextObject()		{ return m_text; }
};