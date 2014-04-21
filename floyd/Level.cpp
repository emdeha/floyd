#include <fstream>

#include "Level.h"


const char* Level::levelDir = "data/world/";

void Level::Init(const std::string &levelFile)
{
	name = levelFile;
	std::ifstream level(levelDir + levelFile);

	if (level.is_open())
	{
		std::string line;
		while (std::getline(level, line))
		{
			map.push_back(line);
			std::cout << line << std::endl;
		}
	}
	else
	{
		std::cout << "Error: Opening level file!\n";
	}
}