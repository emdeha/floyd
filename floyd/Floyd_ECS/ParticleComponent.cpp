#include "stdafx.h"
#include "ParticleComponent.h"

#include <fstream>


ParticleComponent::ParticleComponent()
	: isEmittedFromHero(false),
	  IComponent(CTYPE_PARTICLE)
{
}
ParticleComponent::ParticleComponent(bool newIsEmittedFromHero)
	: isEmittedFromHero(newIsEmittedFromHero),
	  IComponent(CTYPE_PARTICLE)
{
}

void ParticleComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write((char*)&isEmittedFromHero, sizeof(isEmittedFromHero));
}
void ParticleComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&isEmittedFromHero, sizeof(isEmittedFromHero));
}