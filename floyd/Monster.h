#ifndef FLOYD_MONSTER_H
#define FLOYD_MONSTER_H


#include <string>

//#include "IEventListener.h"
#include "Utils.h"


const int maxDeltaY = 2;

class Monster// : public IEventListener
{
private:
	int damage;
	int health;
	Position position;
	int currentDelta;
	int deltaY;
	Position prevPosition;

public:
	Monster() : 
		damage(2), health(10), position(1,1), prevPosition(1,1),
		deltaY(1), currentDelta(0) {}

	void Init(const std::string &monsterFile);
	void Update();

	void SetInitialPosition(Position newPosition);
	void GoToPrevPos();

	Position GetPosition() const;
	Position GetPrevPos() const;

	int GetDamage() const;

public:
	//virtual void OnEvent(const Event &_event);
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

inline int Monster::GetDamage() const
{
	return damage;
}


#endif