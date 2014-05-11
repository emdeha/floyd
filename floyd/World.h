#ifndef FLOYD_WORLD_H
#define FLOYD_WORLD_H


#include <string>
#include <vector>

#include "Level.h"
#include "Hero.h"
#include "Monster.h"
#include "Particle.h"


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

	// TODO: Violates DRY
	//std::vector<IEventListener*> eventListeners;

public:
	World() : levels(0), currentLevelIdx(2) {}

	void Init(const std::string &worldFile);

	void Display() const; 
	void PollInput();
	void Update();

	void AddParticle(Position position);

	//void NotifyEventListeners(const Event &forEvent);

public:
	Position GetPlayerPos() const;
	Position GetPlayerPrevPos() const;

	const std::vector<Monster>& GetMonsters() const;
	const std::vector<Particle>& GetParticles() const;

	Monster* GetMonsterAtPos(Position position);

public:
	~World();

private:
	World(const World &other);
	World& operator=(const World &other);

	void UpdateCollisions();

	void InitLevelObjects();
};


#endif