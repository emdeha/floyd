#ifndef FLOYD_HERO_H
#define FLOYD_HERO_H


#include <string>

#include "Utils.h"


class Hero
{
private:
	int health;
	Position position;
	Position prevPos;

public:
	Hero() : health(0), position(1,1), prevPos(1,1) {}

	void Init(const std::string &heroFile);

	void Move(Direction dir);
	void GoToPrevPos();

	Position GetPosition() const
	{
		return position;
	}
	Position GetPrevPos() const
	{
		return prevPos;
	}

	void SetInitialPosition(Position newPosition)
	{
		prevPos = newPosition;
		position = newPosition;
	}
};


#endif