#include "Particle.h"

#include <math.h>
#include <fstream>
#include <iostream>


Particle::Particle() 
	: damage(0), position(0,0), direction(0,0), prevPosition(0,0), prevTile(TILE_EMPTY),
	  isEmittedFromHero(false)
{
}

void Particle::Update()
{
	// Movable update
	prevPosition = position;
	position.Move(direction);
}

int Particle::GetDamage() const
{
	return damage;
}
Position Particle::GetPosition() const
{
	return position;
}
Position Particle::GetPrevPos() const
{
	return prevPosition;
}

void Particle::GoToPrevPos() 
{
	position = prevPosition;
}

void Particle::SetPosition(Position newPosition)
{
	prevPosition = newPosition;
	position = newPosition;
}
void Particle::SetDirection(Position newDirection)
{
	direction = newDirection;
}

void Particle::SetDamage(int newDamage)
{
	damage = newDamage;
}

char Particle::GetPrevTile() const
{
	return prevTile;
}
void Particle::SetPrevTile(char newPrevTile)
{
	// TODO: Dirty hack. Better to use layers.
	prevTile = (newPrevTile != TILE_MONSTER && newPrevTile != TILE_HERO && 
				newPrevTile != TILE_PARTICLE && newPrevTile != TILE_BOSS)
				? newPrevTile : TILE_EMPTY;
}

void Particle::SetIsEmittedFromHero(bool newIsEmittedFromHero)
{
	isEmittedFromHero = newIsEmittedFromHero;
}
bool Particle::IsEmittedFromHero() const
{
	return isEmittedFromHero;
}

void Particle::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		saveStream.write((char*)&damage, sizeof(int));
		direction.Serialize(saveStream);
		position.Serialize(saveStream);
		prevPosition.Serialize(saveStream);
		saveStream.write((char*)&prevTile, sizeof(char));
		saveStream.write((char*)&isEmittedFromHero, sizeof(bool));
	}
	else
	{
		std::cerr << "Error: Cannot serialize Particle\n";
	}
}
void Particle::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		loadStream.read((char*)&damage, sizeof(int));
		direction.Deserialize(loadStream);
		position.Deserialize(loadStream);
		prevPosition.Deserialize(loadStream);
		loadStream.read((char*)&prevTile, sizeof(char));
		loadStream.read((char*)&isEmittedFromHero, sizeof(bool));
	}
	else
	{
		std::cerr << "Error: Cannot deserialize Particle\n";
	}
}