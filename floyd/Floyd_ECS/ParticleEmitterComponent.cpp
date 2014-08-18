#include "stdafx.h"
#include "ParticleEmitterComponent.h"

#include "../Floyd_General/Utils.h"
#include "../Floyd_World/World.h"

#include <fstream>


ParticleEmitterComponent::ParticleEmitterComponent()
	: particleEmitInterval_s(0), lastTimeOfEmission_s(0), particlesPerEmission(0),
	  IComponent(CTYPE_PARTICLE_EMITTER)
{
	lastTimeOfEmission_s = GetTimeSinceEpoch();
}
ParticleEmitterComponent::ParticleEmitterComponent(time_t newParticleEmitInterval_s,
												   time_t newLastTimeOfEmission_s,
												   int newParticlesPerEmission)
   : particleEmitInterval_s(newParticleEmitInterval_s), lastTimeOfEmission_s(newLastTimeOfEmission_s),
	 particlesPerEmission(newParticlesPerEmission), 
	 IComponent(CTYPE_PARTICLE_EMITTER)
{
}

void ParticleEmitterComponent::EmitParticle(World *world, const Position &pos, int damage, bool isFromHero)
{
	Position particleDir = GetRandomDirection();
	world->SpawnParticle(pos.PositionAfterMove(particleDir), particleDir, damage, isFromHero);
}

void ParticleEmitterComponent::DoSerialization(std::ofstream &saveStream) const
{ 
	saveStream.write((char*)&particleEmitInterval_s, sizeof(particleEmitInterval_s));
	saveStream.write((char*)&lastTimeOfEmission_s, sizeof(lastTimeOfEmission_s));
	saveStream.write((char*)&particlesPerEmission, sizeof(particlesPerEmission));
}
void ParticleEmitterComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&particleEmitInterval_s, sizeof(particleEmitInterval_s));
	loadStream.read((char*)&lastTimeOfEmission_s, sizeof(lastTimeOfEmission_s));
	loadStream.read((char*)&particlesPerEmission, sizeof(particlesPerEmission));
}