#ifndef FLOYD_ITEM_H
#define FLOYD_ITEM_H


#include <string>

#include "Utils.h"


enum ItemAttribute
{
	ATTRIB_PARTICLE = 0,

	ATTRIB_NONE = -1,
};


// TODO: Maybe a struct would be better.
class Item
{
private:
	std::string name;
	int defense;
	int damage;
	ItemAttribute attribute;
	Position position;

public:
	Item(const std::string &newName, int newDefense, int newDamage, ItemAttribute newAttribute,
		 const Position &newPosition);
	
	std::string GetName() const;
	int GetDefense() const;
	int GetDamage() const;
	ItemAttribute GetAttribute() const;
	Position GetPosition() const;

	bool IsValid() const;
};


#endif