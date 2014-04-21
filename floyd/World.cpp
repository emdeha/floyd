#include <iostream>
#include <fstream>

#include "World.h"
#include "Dirs.h"


void World::Init(const std::string &worldFile)
{
	std::ifstream world(worldDir + worldFile);
	
	if (world.is_open())
	{
		std::string levelName;
		while (std::getline(world, levelName, ',').good())
		{
			std::cout << "Loding level: " << levelName << std::endl;
			Level newLevel;
			newLevel.Init(levelName + levelExt);
			levels.push_back(newLevel);
		}
	}
	else
	{
		std::cerr << "Error: Opening world file\n";
	}
}