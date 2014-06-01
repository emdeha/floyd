#include "Particle.h"

#include <math.h>


void Particle::Update()
{
	prevPosition = position;
	position.Move(direction);
}