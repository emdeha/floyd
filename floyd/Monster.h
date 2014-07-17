#ifndef FLOYD_MONSTER_H
#define FLOYD_MONSTER_H


#include <string>

#include "Floyd_Geometry/Vector.h"


class World;

class Monster
{
private:
	int damage;
	int health;

	Position position;
	Position prevPosition;

	time_t particleEmitInterval_s;
	time_t lastTimeOfEmission_s;

	char prevTile; // TODO: Should be someplace else

public:
	Monster();

	void Init(const std::string &monsterFile);
	// Use some kind of messages to the world?
	void Update(World *world);

	void EmitParticle(World *world);

	void SetInitialPosition(Position newPosition);
	void GoToPrevPos();

	Position GetPosition() const;
	Position GetPrevPos() const;

	int GetDamage() const;
	int GetHealth() const;

	void ApplyDamage(int dmg);

	char GetPrevTile() const;
	void SetPrevTile(char newPrevTile);

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
};


#endif