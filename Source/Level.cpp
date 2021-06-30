#include "Level.h"
#include <iostream>
#include <fstream>
#include <filesystem>

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

	// image of the same name

	// this uses a relative path to find the levels folder. using one set of ../ selects the first parent directory
	std::ifstream levelFile("../levels/" + lvlName);
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
}