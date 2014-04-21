#ifndef FLOYD_HERO_H
#define FLOYD_HERO_H


#include <string>

#include "Utils.h"


class Hero
{
private:
	int health;
	Position position;

public:
	Hero() : health(0), position(0,0) {}

	void Init(const std::string &heroFile);

	void SetPosition(const Position &newPos) 
	{
		position = newPos;
	}
};


#endif