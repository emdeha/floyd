#ifndef FLOYD_WORLD_H
#define FLOYD_WORLD_H


#include <string>
#include <vector>

#include "Level.h"
#include "Hero.h"
#include "Monster.h"


class World
{
private:
	std::vector<Level> levels;	
	size_t currentLevelIdx;

	Hero hero;
	std::vector<Monster> monsters;

	// TODO: Violates DRY
	//std::vector<IEventListener*> eventListeners;

public:
	World() : levels(0), currentLevelIdx(2) {}

	void Init(const std::string &worldFile);

	void Display() const; 
	void PollInput();
	void Update();

	//void NotifyEventListeners(const Event &forEvent);

public:
	Position GetPlayerPos() const;
	Position GetPlayerPrevPos() const;

	const std::vector<Monster>& GetMonsters() const;

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