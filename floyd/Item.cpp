#include "Item.h"


Item::Item(const std::string &newName, int newDefense, int newDamage, ItemAttribute newAttribute,
		   const Position &newPosition)
	: name(newName), defense(newDefense), damage(newDamage), attribute(newAttribute), position(newPosition) {}

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
	return name != "" && defense >= 0 && damage >= 0 && position.IsPositive();
}