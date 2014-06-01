#ifndef FLOYD_PARTICLE_H
#define FLOYD_PARTICLE_H


#include "Utils.h"


class Particle
{
private:
	int damage;

	Position direction; // direction represented as a matrix of -1,0,1
						// [-1, 1][ 0, 1][ 1, 1]
						// [-1, 0]Monster[ 1, 0]
						// [-1,-1][ 0,-1][ 1,-1]
	Position position;
	Position prevPosition;

	char prevTile; // TODO: Should be someplace else

public:
	Particle() : damage(1), position(0,0), direction(0,0), prevPosition(0,0), prevTile(TILE_EMPTY) {}

	void Update();

	int GetDamage() const;
	Position GetPosition() const;
	Position GetPrevPos() const;

	void GoToPrevPos();
	
	void SetPosition(Position newPosition);
	void SetDirection(Position newDirection);

	char GetPrevTile() const;
	void SetPrevTile(char newPrevTile);
};

inline int Particle::GetDamage() const
{
	return damage;
}
inline Position Particle::GetPosition() const
{
	return position;
}
inline Position Particle::GetPrevPos() const
{
	return prevPosition;
}

inline void Particle::GoToPrevPos() 
{
	position = prevPosition;
}

inline void Particle::SetPosition(Position newPosition)
{
	position = newPosition;
}
inline void Particle::SetDirection(Position newDirection)
{
	direction = newDirection;
}

inline char Particle::GetPrevTile() const
{
	return prevTile;
}
inline void Particle::SetPrevTile(char newPrevTile)
{
	// TODO: Dirty hack. Better to use layers.
	prevTile = (newPrevTile != TILE_MONSTER && newPrevTile != TILE_HERO && newPrevTile != TILE_PARTICLE) 
				? newPrevTile : TILE_EMPTY;
}


#endif