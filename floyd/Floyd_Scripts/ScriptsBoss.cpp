#include "stdafx.h"
#include "ScriptsBoss.h"

#include "../Floyd_ECS/Entity.h"
#include "../Reporting.h"
#include "../Utils.h"
#include "../World.h"
#include "../Floyd_Level/Tile.h"


// Forgive me Father for I have sinned:
const Position dirs[] = 
{
	Position(0, -1),
	Position( 1, -1),
	Position( 1,  0),
	Position( 1,  1),
	Position( 0,  1),
	Position(-1,  1),
	Position(-1,  0),
	Position(-1, -1)
};

void Floyd::ScriptBoss_OnCollision(World *world, Entity *owner, const Tile *collider)
{
	// Does nothing :/
}

void EmitParticlesInCircle(World *world, const Position &bossPosition, int bossDamage)
{
	for (size_t idx = 0; idx <= 6; idx += 2)
	{
		world->SpawnParticle(bossPosition.PositionAfterMove(dirs[idx]), dirs[idx], bossDamage, false);
	}
}

void Floyd::ScriptBoss_OnUpdateAI(World *world, Entity *owner)
{
	TransformComponent *bossTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
	AnimatedComponent *bossAnimated = owner->GetComponentDirectly<AnimatedComponent>(CTYPE_ANIMATED);

	bossTransform->prevPosition = bossTransform->position;
	bossTransform->position = bossAnimated->GetCurrentAnimPos();

	bossAnimated->UpdateAnim();

	// Emit particles
	ParticleEmitterComponent *bossEmitter =
		owner->GetComponentDirectly<ParticleEmitterComponent>(CTYPE_PARTICLE_EMITTER);
	StatComponent *bossStat = owner->GetComponentDirectly<StatComponent>(CTYPE_STAT);

	time_t timeSinceStart_s = GetTimeSinceEpoch();
	if (timeSinceStart_s - bossEmitter->lastTimeOfEmission_s > bossEmitter->particleEmitInterval_s)
	{
		EmitParticlesInCircle(world, bossTransform->position, bossStat->damage);
		bossEmitter->lastTimeOfEmission_s = timeSinceStart_s;
	}
}