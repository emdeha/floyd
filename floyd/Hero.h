#ifndef FLOYD_HERO_H
#define FLOYD_HERO_H


#include <string>
#include <vector>

//#include "IEventListener.h"
#include "Utils.h"


class Item;
class Skill;
class World;

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

	std::vector<Skill*> skills;
	std::vector<std::string> itemNames; // Only for printing

public:
	Hero();
	~Hero();

	void Init(const std::string &heroFile);

	void Move(Direction dir);
	void CheckInput(char key, World *world);
	void GoToPrevPos();

	void AddItem(const Item *newItem);

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