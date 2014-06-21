#ifndef FLOYD_BOSS_H
#define FLOYD_BOSS_H


#include "Utils.h"

class World;

class Boss
{
private:
	int damage;
	int health;
	int defense;

	Position position;
	Position prevPosition;

	char prevTile;

	Position wp[6];
	int currentWaypoint;

public:
	Boss();

	void Init(const std::string &bossFile);

	void Update(World *world);

	void SetInitialPosition(Position newPosition);
	void GoToPrevPos();

	Position GetPosition() const;
	Position GetPrevPos() const;

	int GetDamage() const;
	int GetHealth() const;
	int GetDefense() const;

	void ApplyDamage(int dmg);

	char GetPrevTile() const;
	void SetPrevTile(char newPrevTile);
};

#endif