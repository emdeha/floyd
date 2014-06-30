#include "Item.h"

#include <fstream>
#include <iostream>


Item::Item()
	: name(""), defense(0), damage(0), health(0), attribute(ATTRIB_NONE), position() {}

Item::Item(const std::string &newName, int newDefense, int newDamage, int newHealth, ItemAttribute newAttribute,
		   const Position &newPosition)
	: name(newName), defense(newDefense), damage(newDamage), health(newHealth), attribute(newAttribute), 
	  position(newPosition) {}

std::string Item::GetName() const
{
	return name;
}

int Item::GetDefense() const
{
	return defense;
}
int Item::GetDamage() const
{
	return damage;
}
int Item::GetHealth() const
{
	return health;
}

ItemAttribute Item::GetAttribute() const
{
	return attribute;
}

Position Item::GetPosition() const
{
	return position;
}

bool Item::IsValid() const
{
	return name != "" && defense >= 0 && damage >= 0 && health >= 0 && position.IsPositive();
}

void Item::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		saveStream << name;
		saveStream << defense;
		saveStream << damage;
		saveStream << health;
		saveStream << (int)attribute;
		position.Serialize(saveStream);
	}
	else
	{
		std::cerr << "Error: Cannot serialize Item\n";
	}
}
void Item::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		loadStream >> name;
		loadStream >> defense;
		loadStream >> damage;
		loadStream >> health;
		int itemAttrib = -999;
		loadStream >> itemAttrib;
		attribute = (ItemAttribute)itemAttrib;
		position.Deserialize(loadStream);
	}
	else
	{
		std::cerr << "Error: Cannot deserialize Item\n";
	}
}