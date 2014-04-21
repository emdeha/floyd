#include <iostream>
#include <fstream>
#include <sstream>

#include "Hero.h"


void Hero::Init(const std::string &heroFile)
{
	std::ifstream hero(heroFile);

	if (hero.is_open())
	{
		std::string line;
		while (std::getline(hero, line, ',').good())
		{
			std::stringstream(line) >> health;
		}
	}
	else
	{
		std::cerr << "Error: Opening hero file!\n"; 
		return;
	}

	hero.close();
}