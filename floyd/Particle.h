#ifndef FLOYD_PARTICLE_H
#define FLOYD_PARTICLE_H


#include "Floyd_Geometry/Vector.h"


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

	bool isEmittedFromHero;

public:
	Particle();

	void Update();

	int GetDamage() const;
	Position GetPosition() const;
	Position GetPrevPos() const;

	void GoToPrevPos();
	
	void SetPosition(Position newPosition);
	void SetDirection(Position newDirection);

	void SetDamage(int newDamage);

	char GetPrevTile() const;
	void SetPrevTile(char newPrevTile);

	void SetIsEmittedFromHero(bool newIsEmittedFromHero);
	bool IsEmittedFromHero() const;

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
};


#endif