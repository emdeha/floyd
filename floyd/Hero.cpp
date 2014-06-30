#include <iostream>
#include <fstream>
#include <sstream>

#include "Hero.h"
#include "Item.h"
#include "Skill.h"
#include "World.h"


Hero::Hero()
	: health(0), damage(0), defense(0), position(1,1), prevPos(1,1), prevTile(' '), hasTalkedToNPC(false)
{
}

Hero::~Hero()
{
	for (auto skill = skills.begin(); skill != skills.end(); ++skill)
	{
		delete (*skill);
	}
	skills.clear();
}

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

void Hero::CheckInput(char key, World *world)
{
	for (auto skill = skills.begin(); skill != skills.end(); ++skill)
	{
		if ((*skill)->GetActivationButton() == key)
		{
			(*skill)->Apply(world);
		}
	}
}

void Hero::PrintStats() const
{
	std::cout << "\n\n";
	std::cout << "Health: " << health << '\n';
	std::cout << "Damage: " << damage << '\n';
	std::cout << "Defense: " << defense << '\n';

	size_t itemNamesSize = itemNames.size();
	if (itemNamesSize > 0)
	{
		std::cout << "Items: ";
		for (size_t idx = 0; idx < itemNamesSize; ++idx)
		{
			std::cout << itemNames[idx];
			if (idx < itemNamesSize - 1)
			{
				std::cout << ", ";
			}
		}
		std::cout << '\n';
	}
}

void Hero::AddItem(const Item *newItem)
{
	if (newItem->IsValid())
	{
		damage += newItem->GetDamage();
		defense += newItem->GetDefense();
		health += newItem->GetHealth();
		if (newItem->GetAttribute() == ATTRIB_PARTICLE)
		{
			ParticleSkill *newSkill = new ParticleSkill('r', 5);	
			skills.push_back(newSkill);
		}
		itemNames.push_back(newItem->GetName());
	}
	// Silently fail otherwise
}

void Hero::AddBuff(const Item *newBuff)
{
	if (newBuff->IsValid())
	{
		damage += newBuff->GetDamage();
		defense += newBuff->GetDefense();
		health += newBuff->GetHealth();
		if (newBuff->GetAttribute() == ATTRIB_PARTICLE) // TODO: Check if applicable for current inventory
		{
			ParticleSkill *newSkill = new ParticleSkill('r', 5);	
			skills.push_back(newSkill);
		}
	}
}

void Hero::Hurt(int dmg)
{
	health -= dmg / (defense == 0 ? 1 : defense);
}

Position Hero::GetPosition() const
{
	return position;
}
Position Hero::GetPrevPos() const
{
	return prevPos;
}

int Hero::GetHealth() const
{
	return health;
}

int Hero::GetDamage() const
{
	return damage;
}

int Hero::GetDefense() const
{
	return defense;
}

std::vector<std::string> Hero::GetItemNames() const
{
	return itemNames;
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

void Hero::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		saveStream << health;
		saveStream << damage;
		saveStream << defense;
		position.Serialize(saveStream);
		prevPos.Serialize(saveStream);
		saveStream << prevTile;
		saveStream << hasTalkedToNPC;
		saveStream << itemNames.size();
		for (auto itemName = itemNames.begin(); itemName != itemNames.end(); ++itemName)
		{
			saveStream << (*itemName);
		}
	}
	else
	{
		std::cerr << "Error: Cannot serialize Hero\n";
	}
}
void Hero::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		loadStream >> health;
		loadStream >> damage;
		loadStream >> defense;
		position.Deserialize(loadStream);
		prevPos.Deserialize(loadStream);
		loadStream >> prevTile;
		loadStream >> hasTalkedToNPC;
		size_t itemNamesSize = 0;
		loadStream >> itemNamesSize;
		for (size_t idx = 0; idx < itemNamesSize; ++idx)
		{
			std::string newItemName = "";
			loadStream >> newItemName;
			itemNames.push_back(newItemName);
		}
	}
	else
	{
		std::cerr << "Error: Cannot deserialize Hero\n";
	}
}