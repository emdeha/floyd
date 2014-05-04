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

void Hero::OnEvent(const Event &_event)
{
	EventType eType = _event.type;

	switch (eType)
	{
	case EVENT_ON_ATTACK:
		{
			const OnAttackEvent &evt = static_cast<const OnAttackEvent&>(_event);
			health -= evt.damage;
		}
		break;
	case EVENT_ON_MOVE:
		{
			const OnMoveEvent &evt = static_cast<const OnMoveEvent&>(_event);
			Move(evt.moveDir);
		}
		break;
	}
}