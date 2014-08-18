#ifndef FLOYD_INVENTORY_COMPONENT_H
#define FLOYD_INVENTORY_COMPONENT_H


#include "Component.h"
#include "../Floyd_Graphics/Sprite.h"

#include <vector>
#include <memory>


class Skill;
class Item;

class InventoryComponent : public IComponent
{
public:
	std::vector<std::shared_ptr<Skill>> skills;
	std::vector<std::string> ownedItemNames;

	Sprite infoAsSprite;

	explicit InventoryComponent(); // We should add them manually.

public:
	void AddItem(const Item *newItem);
	const Sprite* GetInfoAsSprite() const;

	void UpdateInfoSprite();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif