#include "Particle.h"

#include <math.h>


Particle::Particle() 
	: damage(10), position(0,0), direction(0,0), prevPosition(0,0), prevTile(TILE_EMPTY) 
{
}

void Particle::Update()
{
	prevPosition = position;
	position.Move(direction);
}