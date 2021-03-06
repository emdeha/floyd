#ifndef FLOYD_ITEM_H
#define FLOYD_ITEM_H


#include <string>

#include "../Floyd_Geometry/Vector.h"


enum ItemAttribute
{
	ATTRIB_PARTICLE = 0,

	ATTRIB_NONE = -1,
};


class Item
{
private:
	std::string name;
	int defense;
	int damage;
	int health;
	ItemAttribute attribute;
	Position position;

	bool isActive;
	bool isBuff;

public:
	Item();
	Item(const std::string &newName, int newDefense, int newDamage, int newHealth, ItemAttribute newAttribute,
		 const Position &newPosition, bool newIsActive, bool newIsBuff);
	
	std::string GetName() const;
	int GetDefense() const;
	int GetDamage() const;
	int GetHealth() const;
	ItemAttribute GetAttribute() const;
	Position GetPosition() const;

	bool IsValid() const;
	bool IsBuff() const;

	bool IsActive() const;
	void SetIsActive(bool newIsActive);

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
};


#endif