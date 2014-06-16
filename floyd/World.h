#ifndef FLOYD_WORLD_H
#define FLOYD_WORLD_H


#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Level.h"
#include "Hero.h"
#include "Monster.h"
#include "Particle.h"
#include "Item.h"


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

	//
	std::map<int, std::vector<std::string>> itemsForLevel;

	//
	std::vector<Item> itemsInCurrentLevel;

	// TODO: Violates DRY
	//std::vector<IEventListener*> eventListeners;

public:
	World();

	void Init();

	void Display(); 
	void PollInput();
	void Update();

	void AddParticle(const Position &position, const Position &direction, int damage);

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

	Monster* GetMonsterAtPos(const Position &position);
	void SpawnMonsterAtPos(const Position &position);

	Level* GetCurrentLevel();
	int GetCurrentLevelIdx();

	///
	/// @brief Gets the item at the position and removes it from the array.
	///
	Item RetrieveItemAtPos(const Position &position);

	bool AreMonstersDead() const;

	void KillAllMonsters();

public:
	~World();

private:
	World(const World &other);
	World& operator=(const World &other);

	void UpdateCollisions();

	void InitLevelObjects();

	void InitLevels();
	void InitItemsForLevels();
	void InitItemFromFile(const std::string &fileName);

	void TeleportHeroToPosition(const Position &newPosition);
};


#endif