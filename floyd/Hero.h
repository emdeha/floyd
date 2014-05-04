#ifndef FLOYD_HERO_H
#define FLOYD_HERO_H


#include <string>

#include "IEventListener.h"
#include "Utils.h"


class Hero : public IEventListener
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

	Position GetPosition() const;
	Position GetPrevPos() const;

	void SetInitialPosition(Position newPosition);

public:
	virtual void OnEvent(const Event &_event);
};

inline Position Hero::GetPosition() const
{
	return position;
}
inline Position Hero::GetPrevPos() const
{
	return prevPos;
}

inline void Hero::SetInitialPosition(Position newPosition)
{
	prevPos = newPosition;
	position = newPosition;
}

inline void Hero::GoToPrevPos()
{
	// TODO: What was the prevprev pos :?
	position = prevPos;
}

#endif