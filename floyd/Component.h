#ifndef FLOYD_COMPONENT_H
#define FLOYD_COMPONENT_H


#include "Utils.h"


enum ComponentType
{
	COMPONENT_MOVABLE,
	COMPONENT_CHARACTER,
	COMPONENT_PLAYER_CONTROLLABLE,
};


class IComponent
{
public:
	ComponentType type;

	IComponent(ComponentType newType);
	virtual ~IComponent() {};
};


class ComponentMovable
{
public:
	Position position;
	Position prevPosition;
};


#endif