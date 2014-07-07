#ifndef FLOYD_WORLD_H
#define FLOYD_WORLD_H


#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Entity.h"
#include "Level.h"
#include "Hero.h"
#include "Boss.h"
#include "Monster.h"
#include "Particle.h"
#include "Item.h"
#include "Menu.h"


// Simple state system. Not the best but the fastest-to-implement.
enum WorldState
{
	STATE_GAMEPLAY,
	STATE_MENU,

	STATE_NONE = -1
};

class IScript;

class World
{
private:
	std::vector<Level> levels;	
	size_t currentLevelIdx;

	WorldState currentState;

	Menu startupMenu;

	Hero hero;
	Boss boss;
	std::vector<Monster> monsters;

	// When a monster emits a particle, it gets added to this array.
	// Useful for now. Don't know if I'll have to query the owner of the particle.
	std::vector<Particle> particles;

	// Should replace hero, boss, monsters, particles
	std::vector<std::shared_ptr<Entity>> entities;

	//
	std::vector<IScript*> scripts;

	//
	std::map<int, std::vector<std::string>> itemsForLevel;
	std::map<int, std::vector<std::string>> shrinesForLevel;

	//
	std::vector<Item> itemsInCurrentLevel;

	bool isRunning;

public:
	World();

	void Init();

	///
	/// @brief Initializes the default game objects when we've started a New Game.
	///
	void OnFreshStart();
	///
	/// @brief Deserializes objects when we load a saved progress.
	///
	void OnSaveLoaded();

	void Display(); 
	void PollInput();
	void Update();

	void SwitchState(WorldState newState);

	void AddParticle(const Position &position, const Position &direction, int damage, 
					 bool isEmittedFromHero);

public:
	// Entity management
	std::vector<std::shared_ptr<Entity>> GetEntitiesWithComponent(ComponentType cType);
	std::vector<IComponent*> GetComponentsOfType(ComponentType cType);

public:
	Position GetPlayerPos();// const;
	Position GetPlayerPrevPos();// const;

	//Hero& GetHero();
	Entity* GetHero();
	Boss& GetBoss();

	std::vector<Monster>& GetMonsters();
	std::vector<Particle>& GetParticles();

	bool IsRunning() const;
	void SetIsRunning(bool newIsRunning);

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

	void PrintInfo();// const;

	void Serialize();// const;
	void Deserialize();

public:
	~World();

private:
	World(const World &other);
	World& operator=(const World &other);

	void UpdateCollisions();

	void InitLevelObjects();

	void InitLevels();
	void InitItemsForLevels();
	void InitShrinesForLevels();
	void InitItemFromFile(const std::string &fileName);

	void TeleportHeroToPosition(const Position &newPosition);

private:
	void CheckHeroCollision();
	void CheckMonsterCollision();
	void CheckParticleCollision();
	void CheckBossCollision(); 
};


#endif