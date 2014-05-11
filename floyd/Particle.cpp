#include "Particle.h"


void Particle::Update()
{
	position.Move(direction);
}