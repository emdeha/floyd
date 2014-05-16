#ifndef FLOYD_HERO_H
#define FLOYD_HERO_H


#include <string>

//#include "IEventListener.h"
#include "Utils.h"


class Hero //: public IEventListener
{
private:
	int health;
	int damage;
	Position position;
	Position prevPos;

	char prevTile; // TODO: Should be someplace else

public:
	Hero() : health(30), damage(3), position(1,1), prevPos(1,1), prevTile(' ') {}

	void Init(const std::string &heroFile);

	void Move(Direction dir);
	void GoToPrevPos();

	void Hurt(int dmg);

	Position GetPosition() const;
	Position GetPrevPos() const;

	int GetDamage() const;

	void SetInitialPosition(Position newPosition);

	char GetPrevTile() const;
	void SetPrevTile(char newPrevTile);

public:
	//virtual void OnEvent(const Event &_event);
};

inline void Hero::Hurt(int dmg)
{
	health -= dmg;
	if (health <= 0)
	{
		// Print end scene
		std::printf("HERO DIED");
	}
}

inline Position Hero::GetPosition() const
{
	return position;
}
inline Position Hero::GetPrevPos() const
{
	return prevPos;
}

inline int Hero::GetDamage() const
{
	return damage;
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

inline char Hero::GetPrevTile() const
{
	return prevTile;
}
inline void Hero::SetPrevTile(char newPrevTile)
{
	prevTile = newPrevTile;
}


#endif