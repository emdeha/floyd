#include <iostream>
#include <fstream>
#include <sstream>

#include "Hero.h"


const int LINE_HEALTH = 0;
const int LINE_DAMAGE = 1;
const int LINE_DEFENSE = 2;

///
/// Hero file is as follows:
///	line one - health
/// line two - damage
/// line three - defense
///
void Hero::Init(const std::string &heroFile)
{
	std::ifstream hero(heroFile);

	if (hero.is_open())
	{
		std::string line;
		int idx = 0;
		while (std::getline(hero, line).good())
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
			default:
				std::cerr << "Warning: Invalid line idx\n";
			}
			++idx;
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

void Hero::PrintStats() const
{
	std::cout << "\n";
	std::cout << "Health: " << health << "\n";
	std::cout << "Damage: " << damage << "\n";
	std::cout << "Defense: " << defense << "\n";
}

int Hero::GetHealth() const
{
	return health;
}

void Hero::Hurt(int dmg)
{
	health -= dmg;
}

Position Hero::GetPosition() const
{
	return position;
}
Position Hero::GetPrevPos() const
{
	return prevPos;
}

int Hero::GetDamage() const
{
	return damage;
}

void Hero::SetInitialPosition(Position newPosition)
{
	assert(newPosition.IsPositive());

	prevPos = newPosition;
	position = newPosition;
}

void Hero::GoToPrevPos()
{
	// TODO: What was the prevprev pos :?
	position = prevPos;
}

char Hero::GetPrevTile() const
{
	return prevTile;
}
void Hero::SetPrevTile(char newPrevTile)
{
	// TODO: Dirty hack. Better to use layers.
	prevTile = (newPrevTile != TILE_MONSTER && newPrevTile != TILE_HERO && newPrevTile != TILE_PARTICLE)
				? newPrevTile : TILE_EMPTY;
}

bool Hero::HasTalkedToNPC() const
{
	return hasTalkedToNPC;
}
void Hero::SetHasTalkedToNPC(bool newHasTalkedToNPC)
{
	hasTalkedToNPC = newHasTalkedToNPC;
}

//void Hero::OnEvent(const Event &_event)
//{
//	EventType eType = _event.type;

//	switch (eType)
//	{
//	case EVENT_ON_ATTACK:
//		{
//			const OnAttackEvent &evt = static_cast<const OnAttackEvent&>(_event);
//			if (evt.chType == CHARACTER_MONSTER)
//			{
//				health -= evt.damage;
//			}
//		}
//		break;
//	case EVENT_ON_MOVE:
//		{
//			const OnMoveEvent &evt = static_cast<const OnMoveEvent&>(_event);
//			Move(evt.moveDir);
//		}
//		break;
//	}
//}