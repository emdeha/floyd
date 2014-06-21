#include <fstream>
#include <iostream>
#include <sstream>

#include "Boss.h"
#include "World.h"


Boss::Boss()
	: damage(0), health(0), position(1,1), prevPosition(1,1), prevTile(TILE_EMPTY), currentWaypoint(0)
{
}

///
/// Boss file is as follows:
/// line one - health
/// line two - damage
/// line three - defense
/// line four - reference to StatSprite file
///
void Boss::Init(const std::string &bossFile)
{
	std::ifstream boss(bossFile);

	if (boss.is_open())
	{
		std::string line;
		int idx = 0;
		while (std::getline(boss, line).good())
		{
			switch (idx)
			{
			case LINE_HEALTH:
				std::stringstream(line) >> health;
				break;
			case LINE_DAMAGE:
				std::stringstream(line) >> damage;
				break;
			case LINE_DEFENSE:
				std::stringstream(line) >> defense;
				break;
			case LINE_STATSPRITE_FILE:
				// Load StatSprite
				break;
			default:
				std::cerr << "Warning: Invalid line idx\n";
			}
			++idx;
		}
	}
	else
	{
		std::cerr << "Error: Opening boss file!\n"; 
		return;
	}

	boss.close();
}

void Boss::Update(World *world)
{
	prevPosition = position;
	position = wp[currentWaypoint];

	++currentWaypoint;
	if (currentWaypoint > 5)
	{
		currentWaypoint	= 0;
	}
}

void Boss::SetInitialPosition(Position newPosition)
{
	position = newPosition;

	// Scripted boss path.
	wp[0] = Position(position.x + 1, position.y - 1);
	wp[1] = Position(position.x + 2, position.y - 1);
	wp[2] = Position(position.x + 3, position.y);
	wp[3] = Position(position.x + 2, position.y + 1);
	wp[4] = Position(position.x + 1, position.y + 1);
	wp[5] = position;
}

void Boss::GoToPrevPos()
{
	position = prevPosition;
}

Position Boss::GetPosition() const
{
	return position;
}
Position Boss::GetPrevPos() const
{
	return prevPosition;
}

int Boss::GetDamage() const
{
	return damage;
}
int Boss::GetHealth() const
{
	return health;
}
int Boss::GetDefense() const
{
	return defense;
}

void Boss::ApplyDamage(int dmg)
{
	health -= dmg;
}

char Boss::GetPrevTile() const
{
	return prevTile;
}
void Boss::SetPrevTile(char newPrevTile)
{
	// TODO: Dirty hack. Better to use layers.
	prevTile = (newPrevTile != TILE_MONSTER && newPrevTile != TILE_HERO && newPrevTile != TILE_PARTICLE)
				? newPrevTile : TILE_EMPTY;
}