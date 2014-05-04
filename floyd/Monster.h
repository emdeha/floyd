#ifndef FLOYD_MONSTER_H
#define FLOYD_MONSTER_H


#include <string>

#include "Utils.h"


const int maxDeltaY = 2;

class Monster
{
private:
	int attack;
	int health;
	Position position;
	int currentDelta;
	int deltaY;
	Position prevPosition;

public:
	Monster() : 
		attack(0), health(0), position(1,1), prevPosition(1,1),
		deltaY(1), currentDelta(0) {}

	void Init(const std::string &monsterFile);
	void Update();

	void SetInitialPosition(Position newPosition);
	void GoToPrevPos();

	Position GetPosition() const;
	Position GetPrevPos() const;
};

inline void Monster::SetInitialPosition(Position newPosition)
{
	position = newPosition;
}

inline void Monster::GoToPrevPos()
{
	position = prevPosition;
}

inline Position Monster::GetPosition() const
{
	return position;
}
inline Position Monster::GetPrevPos() const
{
	return prevPosition;
}


#endif