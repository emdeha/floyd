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

void Hero::Move(Direction dir)
{
	prevPos = position;
	switch (dir)
	{
	case DIR_RIGHT:
		position.x += 1;
		break;
	case DIR_LEFT:
		position.x -= 1;
		break;
	case DIR_UP:
		position.y -= 1;
		break;
	case DIR_DOWN:
		position.y += 1;
		break;
	default:
		std::cerr << "Warning: Invalid direction!\n";
		return;
	}

	if (position.y < 0)
	{
		position.y += 1;
	}
	if (position.x < 0)
	{
		position.x += 1;
	}
	// Validate position
}

void Hero::GoToPrevPos()
{
	// TODO: What was the prevprev pos :?
	position = prevPos;
}