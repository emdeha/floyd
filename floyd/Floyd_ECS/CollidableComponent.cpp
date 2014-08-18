#include "stdafx.h"
#include "CollidableComponent.h"

#include "../Floyd_Scripts/ScriptDispatcher.h"

#include <fstream>


CollidableComponent::CollidableComponent()
	: onCollision(nullptr),
	  IComponent(CTYPE_COLLIDABLE)
{
}

void CollidableComponent::SetOnCollision(OnCollision newOnCollision, const std::string &scriptGroup)
{
	onCollision = newOnCollision;
	group = scriptGroup;
}
void CollidableComponent::CallOnCollision(World *world, const Tile *collider)
{
	if (onCollision)
	{
		onCollision(world, owner, collider);
	}
}

void CollidableComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write(collisionInfo, INFOTYPE_COUNT);
}
void CollidableComponent::DoDeserialization(std::ifstream &loadStream)
{
	onCollision = Floyd::GetOnCollision(group);
	loadStream.read(collisionInfo, INFOTYPE_COUNT);
}