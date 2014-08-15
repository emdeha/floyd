#ifndef FLOYD_WORLD_H
#define FLOYD_WORLD_H


#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Entity.h"
#include "Level.h"
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

	void PollInput();
	void Update();

	void SwitchState(WorldState newState);
	WorldState GetState() const;

public:
	// Entity management
	std::vector<std::shared_ptr<Entity>> GetEntitiesWithComponent(ComponentType cType);
	std::vector<const std::shared_ptr<Entity>> GetEntitiesWithComponent_const(ComponentType cType) const;
	std::vector<IComponent*> GetComponentsOfType(ComponentType cType);
	std::vector<const IComponent*> GetComponentsOfType_const(ComponentType cType) const;

	std::shared_ptr<Entity> GetEntityAtPos(const Position &pos);
	const Entity *GetEntityByAIType_const(AIType aiType) const;

public:
	Position GetPlayerPos() const;
	Position GetPlayerPrevPos() const;

	Entity* GetHero();
	const Entity* GetHero_const() const;

	bool IsRunning() const;
	void SetIsRunning(bool newIsRunning);

	void SpawnMonsterAtPos(const Position &position);
	void SpawnParticle(const Position &position, const Position &dir, int damage,
					   bool isEmittedFromHero);

	Level* GetCurrentLevel();
	int GetCurrentLevelIdx();

	Tile GetTileAtPositionForCollision(const Position &position, const CollidableComponent *collidable) const;

	void GoToNextLevel();

	std::vector<std::pair<const Sprite*, Position>> GetSpritesForDrawing() const;

	///
	/// @brief Gets the item at the position and removes it from the array.
	///
	Item RetrieveItemAtPos(const Position &position);

	bool IsItemAtPosActive(const Position &position) const;

	bool AreMonstersDead() const;

	void TeleportHeroToPosition(const Position &newPosition);

	void Serialize() const;
	void Deserialize();

public:
	~World();

private:
	World(const World &other);
	World& operator=(const World &other);

	void InitLevelObjects();

	void InitLevels();
	void InitItemsForLevels();
	void InitShrinesForLevels();
	void InitItemFromFile(const std::string &fileName);

	// Factory class?
	void CreateHero();
	void CreateMonster(const Position &pos);
	void CreateBoss(const Position &pos);
	void CreateParticle(const Position &pos, const Position &dir, int damage,
						bool isEmittedFromHero);


	void RemoveAIEntities();
	void RemoveDeadEntities();
};


#endif