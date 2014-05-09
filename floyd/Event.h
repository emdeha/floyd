#ifndef FLOYD_EVENT_H
#define FLOYD_EVENT_H


#include "Utils.h"

enum CharacterType
{
	CHARACTER_MONSTER,
	CHARACTER_HERO,
	CHARACTER_BOSS,
};

enum EventType
{
	EVENT_ON_ATTACK,
	EVENT_ON_MOVE,
};

class Event
{
public:
	EventType type;

	Event(EventType _type) : type(_type) {}
};

class OnAttackEvent : public Event
{
public:
	int damage;
	CharacterType chType;

	OnAttackEvent(int _damage, CharacterType _chType) 
		: Event(EVENT_ON_ATTACK), damage(_damage), chType(_chType) {}
};

class OnMoveEvent : public Event
{
public:
	Direction moveDir;

	OnMoveEvent(Direction _moveDir)
		: Event(EVENT_ON_MOVE), moveDir(_moveDir) {}
};


#endif