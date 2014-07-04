#ifndef FLOYD_COMPONENT_H
#define FLOYD_COMPONENT_H


#include <time.h>
#include <vector>
#include <string>

#include "Utils.h"


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

class IComponent
{
private:
	virtual void OnUpdate() = 0;

public:
	ComponentType cType;

	void Update();

	IComponent(ComponentType newCType);
	virtual ~IComponent();
};

class StatComponent : public IComponent
{
public:
	int health;
	int defense;
	int damage;
	int maxHealth;

	StatComponent(int newHealth, int newDefense, int newDamage, int newMaxHealth);
};

class ParticleEmitterComponent : public IComponent
{
public:
	time_t particleEmitInterval_s;
	time_t lastTimeOfEmission_s;
	int particlesPerEmission;

	ParticleEmitterComponent(time_t newParticleEmitInterval_s, time_t newLastTimeOfEmission_s,
							 int newParticlesPerEmission);

private:
	void OnUpdate();
};

class MovableComponent : public IComponent
{
public:
	Position position;
	Position prevPosition;
	Position direction;
	char prevTile;

	MovableComponent(const Position &newPosition, const Position &newPrevPosition, const Position &newDirection,
					 char newPrevTile);

private:
	void OnUpdate();
};

class Entity;

class OwnableComponent : public IComponent
{
public:
	Entity *owner;

	OwnableComponent(Entity *newOwner);
	~OwnableComponent();
};

class ControllableComponent : public IComponent
{
public:
	ControllableComponent();

private:
	void OnUpdate();
};

class AIComponent : public IComponent
{
public:
	AIComponent();

private:
	void OnUpdate();
};

class Skill;

class InventoryComponent : public IComponent
{
public:
	std::vector<Skill*> skills;
	std::vector<std::string> ownedItemNames;

	InventoryComponent(); // We should add them manually.
};

class CollidableComponent : public IComponent
{
public:
	CollidableComponent();

private:
	void OnUpdate();
};


#endif