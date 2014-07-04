#ifndef FLOYD_COMPONENT_H
#define FLOYD_COMPONENT_H


enum ComponentType
{
	CTYPE_STAT,
	CTYPE_PARTICLE_EMITTER,
	CTYPE_MOVABLE,
	CTYPE_OWNABLE,
	CTYPE_CONTROLLABLE,
	CTYPE_AI,
	CTYPE_COLLIDABLE,
	CTYPE_INVENTOY,

	CTYPE_INVALID = -1
};

struct IComponent
{
	ComponentType cType;

	IComponent(ComponentType newCType);
	virtual ~IComponent();
};


#endif