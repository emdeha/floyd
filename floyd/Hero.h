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
	int defense;
	Position position;
	Position prevPos;

	char prevTile; // TODO: Should be someplace else

	bool hasTalkedToNPC;

public:
	Hero() : health(0), damage(0), defense(0), position(1,1), prevPos(1,1), prevTile(' '), 
			 hasTalkedToNPC(false) {}

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
	void PrintStats() const;

public:
	//virtual void OnEvent(const Event &_event);
};


#endif