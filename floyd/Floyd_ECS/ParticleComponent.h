#ifndef FLOYD_PARTICLE_COMPONENT_H
#define FLOYD_PARTICLE_COMPONENT_H


#include "Component.h"


class ParticleComponent : public IComponent
{
public:
	bool isEmittedFromHero;

	explicit ParticleComponent();
	ParticleComponent(bool newIsEmittedFromHero);

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif