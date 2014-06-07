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

	bool hasTalkedToNPC;

public:
	Hero() : health(30), damage(3), position(1,1), prevPos(1,1), prevTile(' '), hasTalkedToNPC(false) {}

	void Init(const std::string &heroFile);

	void Move(Direction dir);
	void GoToPrevPos();

	void Hurt(int dmg);

	Position GetPosition() const;
	Position GetPrevPos() const;

	int GetDamage() const;
	int GetHealth() const;

	void SetInitialPosition(Position newPosition);

	char GetPrevTile() const;
	void SetPrevTile(char newPrevTile);

	bool HasTalkedToNPC() const;
	void SetHasTalkedToNPC(bool newHasTalkedToNPC);

public:
	//virtual void OnEvent(const Event &_event);
};

inline void Hero::Hurt(int dmg)
{
	health -= dmg;
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
	assert(newPosition.IsPositive());

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
	// TODO: Dirty hack. Better to use layers.
	prevTile = (newPrevTile != TILE_MONSTER && newPrevTile != TILE_HERO && newPrevTile != TILE_PARTICLE)
				? newPrevTile : TILE_EMPTY;
}

inline bool Hero::HasTalkedToNPC() const
{
	return hasTalkedToNPC;
}
inline void Hero::SetHasTalkedToNPC(bool newHasTalkedToNPC)
{
	hasTalkedToNPC = newHasTalkedToNPC;
}

#endif