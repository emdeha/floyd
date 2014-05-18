#include "Particle.h"

#include <math.h>


const char PARTICLE_TILE = '.';

void Particle::Update()
{
	prevPosition = position;
	position.Move(direction);
}