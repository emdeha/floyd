#ifndef FLOYD_WORLD_H
#define FLOYD_WORLD_H


#include <string>
#include <vector>

#include "Level.h"


class World
{
private:
	std::vector<Level> levels;	

public:
	World() : levels(0) {}

	void Init(const std::string &worldFile);

private:
	World(const World &other);
	World& operator=(const World &other);
};


#endif