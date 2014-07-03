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
		size_t nameLength = name.length();
		saveStream.write((char*)&nameLength, sizeof(size_t));
		saveStream.write(name.c_str(), nameLength * sizeof(char));

		saveStream.write((char*)&defense, sizeof(int));
		saveStream.write((char*)&damage, sizeof(int));
		saveStream.write((char*)&health, sizeof(int));
		saveStream.write((char*)&attribute, sizeof(ItemAttribute));
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
		size_t nameLength = 0;
		loadStream.read((char*)&nameLength, sizeof(size_t));
		char nameCStr[50] = "";
		loadStream.read(nameCStr, nameLength * sizeof(char));
		name.append(nameCStr);

		loadStream.read((char*)&defense, sizeof(int));
		loadStream.read((char*)&damage, sizeof(int));
		loadStream.read((char*)&health, sizeof(int));
		loadStream.read((char*)&attribute, sizeof(ItemAttribute));
		position.Deserialize(loadStream);
	}
	else
	{
		std::cerr << "Error: Cannot deserialize Item\n";
	}
}