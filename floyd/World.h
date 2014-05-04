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

public:
	World() : levels(0), currentLevelIdx(4) {}

	void Init(const std::string &worldFile);

	void Display() const; 
	void PollInput();
	void Update();

	Position GetPlayerPos() const;
	Position GetPlayerPrevPos() const;

	const std::vector<Monster>& GetMonsters() const;

private:
	World(const World &other);
	World& operator=(const World &other);

	void UpdateCollisions();

	void InitLevelObjects();
};


#endif