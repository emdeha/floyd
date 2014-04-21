#include <fstream>

#include "Level.h"
#include "Dirs.h"


void Level::Init(const std::string &levelFile)
{
	name = levelFile;
	std::ifstream level(worldDir + levelFile);

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
		std::cerr << "Error: Opening level file!\n";
	}
}