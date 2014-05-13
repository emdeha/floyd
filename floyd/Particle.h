#ifndef FLOYD_PARTICLE_H
#define FLOYD_PARTICLE_H


#include "Utils.h"

extern const char PARTICLE_TILE;

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

public:
	Particle() : damage(1), position(0,0), direction(0,0), prevPosition(0,0) {}

	void Update();

	int GetDamage() const;
	Position GetPosition() const;
	Position GetPrevPos() const;

	void GoToPrevPos();
	
	void SetPosition(Position newPosition);
	void SetDirection(Position newDirection);
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


#endif