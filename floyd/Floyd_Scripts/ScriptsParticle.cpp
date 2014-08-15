#include "stdafx.h"
#include "ScriptsParticle.h"

#include "../Floyd_ECS/Entity.h"
#include "../Floyd_General/Reporting.h"
#include "../Floyd_General/Utils.h"
#include "../Floyd_Level/Tile.h"
#include "../Floyd_World/World.h"


void Floyd::ScriptParticle_OnCollision(World *world, Entity *owner, const Tile *collider)
{
	TransformComponent *ptTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
	StatComponent *ptStat = owner->GetComponentDirectly<StatComponent>(CTYPE_STAT);
	ParticleComponent *ptParticle = owner->GetComponentDirectly<ParticleComponent>(CTYPE_PARTICLE);

	if ( ! world->GetCurrentLevel()->IsPositionInsideMap(ptTransform->position.PositionAfterMove(ptTransform->direction)))
	{
		ptStat->health = 0;
	}

	if (collider->logicalSprite == TILE_WALL || collider->logicalSprite == TILE_MONSTER || 
		collider->logicalSprite == TILE_STASH || collider->logicalSprite == TILE_NPC || 
		collider->logicalSprite == TILE_TELEPORT || collider->logicalSprite == TILE_DREAMS || 
		collider->logicalSprite == TILE_EXIT || collider->logicalSprite == TILE_HERO || 
		collider->logicalSprite == TILE_BOSS || collider->logicalSprite == TILE_EXIT_BLOCK)
	{
		if (collider->sprite == TILE_HERO) 
		{
			world->GetHero()->GetComponentDirectly<StatComponent>(CTYPE_STAT)->ApplyDamage(ptStat->damage);
		}
		else if ((collider->sprite == TILE_MONSTER || collider->sprite == TILE_BOSS) &&
				 ptParticle->isEmittedFromHero)
		{
			auto enemy =
				// We predict the particle's position. Therefore, the hit target will be the entity after the particle has
				// moved once more.
				world->GetEntityAtPos(ptTransform->position.PositionAfterMove(ptTransform->direction));
			if (enemy) 
			{
				enemy->GetComponentDirectly<StatComponent>(CTYPE_STAT)->ApplyDamage(ptStat->damage);
			}
		}

		ptStat->health = 0;
		ptTransform->GoToPrevPos();
	}
}

void Floyd::ScriptParticle_OnUpdateAI(World *world, Entity *owner)
{
	TransformComponent *particleTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);

	particleTransform->prevPosition = particleTransform->position;
	particleTransform->position.Move(particleTransform->direction);
}