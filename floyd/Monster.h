#ifndef FLOYD_MONSTER_H
#define FLOYD_MONSTER_H


#include <string>

//#include "IEventListener.h"
#include "Utils.h"


const int maxDeltaY = 2;

class World;

class Monster// : public IEventListener
{
private:
	int damage;
	int health;

	Position position;
	Position prevPosition;
	int currentDelta;
	int deltaY;

	time_t particleEmitInterval_s;
	time_t lastTimeOfEmission_s;

	char prevTile; // TODO: Should be someplace else

public:
	Monster() : 
		damage(2), health(10), position(1,1), prevPosition(1,1),
		deltaY(1), currentDelta(0), particleEmitInterval_s(1), prevTile(' ') 
	{
		lastTimeOfEmission_s = GetTimeSinceEpoch();
	}

	void Init(const std::string &monsterFile);
	// Use some kind of messages to the world?
	void Update(World *world);

	void EmitParticle(World *world);

	void SetInitialPosition(Position newPosition);
	void GoToPrevPos();

	Position GetPosition() const;
	Position GetPrevPos() const;

	int GetDamage() const;
	int GetHealth() const;

	void ApplyDamage(int dmg);

	char GetPrevTile() const;
	void SetPrevTile(char newPrevTile);

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
inline int Monster::GetHealth() const
{
	return health;
}

inline void Monster::ApplyDamage(int dmg)
{
	health -= dmg;
}

inline char Monster::GetPrevTile() const
{
	return prevTile;
}
inline void Monster::SetPrevTile(char newPrevTile)
{
	prevTile = newPrevTile;
}


#endif