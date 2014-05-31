#ifndef FLOYD_WORLD_H
#define FLOYD_WORLD_H


#include <string>
#include <vector>
#include <memory>

#include "Level.h"
#include "Hero.h"
#include "Monster.h"
#include "Particle.h"


class IScript;

class World
{
private:
	std::vector<Level> levels;	
	size_t currentLevelIdx;

	Hero hero;
	std::vector<Monster> monsters;

	// When a monster emits a particle, it gets added to this array.
	// Useful for now. Don't know if I'll have to query the owner of the particle.
	std::vector<Particle> particles;

	//
	std::vector<IScript*> scripts;

	// TODO: Violates DRY
	//std::vector<IEventListener*> eventListeners;

public:
	World() : levels(0), currentLevelIdx(4) {}

	void Init(const std::string &worldFile);

	void Display(); 
	void PollInput();
	void Update();

	void AddParticle(Position position);

	//void NotifyEventListeners(const Event &forEvent);

public:
	void CheckHeroCollision();
	void CheckMonsterCollision();
	void CheckParticleCollision();

public:
	Position GetPlayerPos() const;
	Position GetPlayerPrevPos() const;

	Hero& GetHero();

	std::vector<Monster>& GetMonsters();
	std::vector<Particle>& GetParticles();

	Monster* GetMonsterAtPos(Position position);
	void SpawnMonsterAtPos(Position position);

	Level* GetCurrentLevel();
	int GetCurrentLevelIdx();

	bool AreMonstersDead() const;

	void KillAllMonsters();

public:
	~World();

private:
	World(const World &other);
	World& operator=(const World &other);

	void UpdateCollisions();

	void InitLevelObjects();
};


#endif