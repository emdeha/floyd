#include "stdafx.h"
#include "ScriptsBoss.h"

#include "../Entity.h"
#include "../Reporting.h"
#include "../Utils.h"
#include "../World.h"
#include "../Floyd_Level/Tile.h"


void Floyd::ScriptBoss_OnCollision(World *world, Entity *owner, const Tile *collider)
{
	// Does nothing :/
}

void Floyd::ScriptBoss_OnUpdateAI(World *world, Entity *owner)
{
	TransformComponent *bossTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
	AnimatedComponent *bossAnimated = owner->GetComponentDirectly<AnimatedComponent>(CTYPE_ANIMATED);

	bossTransform->prevPosition = bossTransform->position;
	bossTransform->position = bossAnimated->GetCurrentAnimPos();

	bossAnimated->UpdateAnim();

	// Emit particles
}