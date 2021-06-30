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

	void extractPlatformData(std::stringstream ss);

public:
	std::string levelName;

	Level() {}
	Level(std::string levelName);

	void loadLevel(std::string levelName);

	std::vector<sf::RectangleShape>& getPlatforms() { return m_platforms; }
};