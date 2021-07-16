#include "Level.h"
#include "utils.h"

#include <iostream>
#include <fstream>

// the data format (i.e. in the text files) for levels is as follows
enum class PlatformProperties
{
	Width,
	Height,
	Xpos,
	Ypos,
	R,
	G,
	B,
	TotalProperties
};

Level::Level(std::string levelName)
{
	loadLevel(levelName);
}

void Level::extractPlatformData(std::stringstream ss)
{
	std::string properties[(int)PlatformProperties::TotalProperties];
	m_platforms.push_back(sf::RectangleShape());

	// fill the properties array with all the values for the platform
	for (auto& property : properties)
		std::getline(ss, property, ',');

	// then, actually apply the properties to the sf::RectangleShape

	m_platforms[m_platforms.size() - 1].setSize(sf::Vector2f
	(
		stof(properties[(int)PlatformProperties::Width]), 
		stof(properties[(int)PlatformProperties::Height])
	));

	m_platforms[m_platforms.size() - 1].setPosition(sf::Vector2f
	(
		stof(properties[(int)PlatformProperties::Xpos]),
		stof(properties[(int)PlatformProperties::Ypos])
	));

	m_platforms[m_platforms.size() - 1].setFillColor(sf::Color
	(
		stoi(properties[(int)PlatformProperties::R]),
		stoi(properties[(int)PlatformProperties::G]),
		stoi(properties[(int)PlatformProperties::B])
	));
}

// make packet with char[64]
void Level::loadLevel(std::string lvlName)
{
	levelName = lvlName;

	// Load the platform data out of the .txt files

	// this uses a relative path to find the levels folder. using one set of ../ selects the first parent directory

	std::ifstream levelFile(CURRENT_WORKING_DIRECTORY + "\\levels\\" + lvlName + ".txt");
	std::string fileLine;

	if (levelFile.is_open())
	{
		while (std::getline(levelFile, fileLine))
			// now that the fileLine string is full, we can search through it using a string stream
			extractPlatformData(std::stringstream(fileLine));

		levelFile.close();
	}
	else
		std::cout << "Unable to open level file\n";

	// load the background image for the level. this is drawn on top of the actual platforms
	m_levelTexture.loadFromFile(CURRENT_WORKING_DIRECTORY + "\\levels\\" + lvlName + ".png");
	levelBGSprite.setTexture(m_levelTexture, true);

	loaded = true;
}