#ifndef FLOYD_PARTICLE_EMITTER_COMPONENT_H
#define FLOYD_PARTICLE_EMITTER_COMPONENT_H


#include "Component.h"
#include "../Floyd_Geometry/Vector.h"


class World;

class ParticleEmitterComponent : public IComponent
{
public:
	time_t particleEmitInterval_s;
	time_t lastTimeOfEmission_s;
	int particlesPerEmission;

	explicit ParticleEmitterComponent();
	ParticleEmitterComponent(time_t newParticleEmitInterval_s, time_t newLastTimeOfEmission_s,
							 int newParticlesPerEmission);
	
public:
	void EmitParticle(World *world, const Position &pos, int damage, bool isFromHero);

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif