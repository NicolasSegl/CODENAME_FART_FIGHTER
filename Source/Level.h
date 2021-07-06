#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>

// read data in from a file
// use image as backdrop. render it over the platforms (add this in later)

class Level
{
private:
	std::vector<sf::RectangleShape> m_platforms;
	sf::Texture m_levelTexture;

	void extractPlatformData(std::stringstream ss);

public:
	std::string levelName;
	sf::Sprite levelBGSprite;
	bool loaded = false;

	Level() {}
	Level(std::string levelName);

	void loadLevel(std::string levelName);

	std::vector<sf::RectangleShape>& getPlatforms() { return m_platforms; }
};