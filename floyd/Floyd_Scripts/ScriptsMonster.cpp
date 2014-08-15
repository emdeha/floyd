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
		{
			owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM)->GoToPrevPos();
		}
		break;
	}
}

void Floyd::ScriptMonster_OnUpdateAI(World *world, Entity *owner)
{
	TransformComponent *monsterTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
	ParticleEmitterComponent *monsterEmitter =
		owner->GetComponentDirectly<ParticleEmitterComponent>(CTYPE_PARTICLE_EMITTER);
	StatComponent *monsterStat = owner->GetComponentDirectly<StatComponent>(CTYPE_STAT);

	time_t timeSinceStart_s = GetTimeSinceEpoch();
	if (timeSinceStart_s - monsterEmitter->lastTimeOfEmission_s > monsterEmitter->particleEmitInterval_s)
	{
		monsterEmitter->EmitParticle(world, monsterTransform->position, monsterStat->damage, false);
		monsterEmitter->lastTimeOfEmission_s = timeSinceStart_s;
	}
}