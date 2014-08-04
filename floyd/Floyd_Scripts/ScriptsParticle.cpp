#include "stdafx.h"
#include "ScriptsParticle.h"

#include "../Entity.h"
#include "../Reporting.h"
#include "../Utils.h"
#include "../World.h"
#include "../Floyd_Level/Tile.h"


void Floyd::ScriptParticle_OnCollision(World *world, Entity *owner, const Tile *collider)
{
	switch(collider->logicalSprite)
	{
	case TILE_WALL:
		owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM)->GoToPrevPos();
		break;
	}
}

void Floyd::ScriptParticle_OnUpdateAI(World *world, Entity *owner)
{
	TransformComponent *particleTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);

	particleTransform->position.Move(particleTransform->direction);
}