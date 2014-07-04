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
	int maxHealth;

	Position position;
	Position prevPosition;

	char prevTile;

	Position wp[6];
	int currentWaypoint;

	bool isDead;

private:
	time_t particleEmitInterval_s;
	time_t lastTimeOfEmission_s;
	int amountOfParticlesPerEmission;

public:
	Boss();

	void Init(const std::string &bossFile);

	void Update(World *world);

	void EmitParticlesInCircle(World *world);

	void SetInitialPosition(Position newPosition);

public:
	void GoToPrevPos();

	Position GetPosition() const;
	Position GetPrevPos() const;

	int GetDamage() const;
	int GetHealth() const;
	int GetDefense() const;

	int GetMaxHealth() const;

	void ApplyDamage(int dmg);

	char GetPrevTile() const;
	void SetPrevTile(char newPrevTile);

	bool IsDead() const;
	void SetIsDead(bool newIsDead);

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
};

#endif