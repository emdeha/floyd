#ifndef FLOYD_WORLD_H
#define FLOYD_WORLD_H


#include <string>
#include <vector>

#include "Level.h"
#include "Hero.h"


class World
{
private:
	std::vector<Level> levels;	
	size_t currentLevelIdx;

	Hero hero;

public:
	World() : levels(0), currentLevelIdx(5) {}

	void Init(const std::string &worldFile);

	void Display() const; 
	bool PollInput();
	void Update();

	Position GetPlayerPos() const;
	Position GetPlayerPrevPos() const;

private:
	World(const World &other);
	World& operator=(const World &other);

	void UpdateCollisions();
};


#endif