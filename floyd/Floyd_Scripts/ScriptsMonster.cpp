#include "stdafx.h"
#include "ScriptsMonster.h"

#include "../Entity.h"
#include "../Reporting.h"
#include "../Utils.h"
#include "../World.h"
#include "../Floyd_Level/Tile.h"


void Floyd::ScriptMonster_OnCollision(World *world, Entity *owner, const Tile *collider)
{
	switch(collider->logicalSprite)
	{
	case TILE_WALL:
		owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM)->GoToPrevPos();
		break;
	}
}

void Floyd::ScriptMonster_OnUpdateAI(Entity *owner)
{
	TransformComponent *monsterTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
	AIComponent *monsterAI = owner->GetComponentDirectly<AIComponent>(CTYPE_AI);

	if (monsterAI->currOffset < monsterAI->maxOffset)
	{
		monsterTransform->position.x += monsterAI->diff;
		monsterAI->currOffset += 1;
	}
	else
	{
		monsterAI->currOffset = 0;
		monsterAI->diff *= -1;
	}
}