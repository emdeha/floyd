#ifndef FLOYD_COMPONENT_H
#define FLOYD_COMPONENT_H


#include <time.h>
#include <vector>
#include <string>
#include <memory>

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
	virtual IComponent* OnCopy() const = 0;

public:
	ComponentType cType;

public:
	void Update();

	IComponent* Copy() const;

public:
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

private:
	void OnUpdate();
	IComponent* OnCopy() const;
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
	IComponent* OnCopy() const;
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
	IComponent* OnCopy() const;
};

class Entity;

class OwnableComponent : public IComponent
{
public:
	std::shared_ptr<Entity> owner;

	OwnableComponent(Entity *newOwner);

private:
	void OnUpdate();
	IComponent* OnCopy() const;
};

class ControllableComponent : public IComponent
{
public:
	ControllableComponent();

private:
	void OnUpdate();
	IComponent* OnCopy() const;
};

class AIComponent : public IComponent
{
public:
	AIComponent();

private:
	void OnUpdate();
	IComponent* OnCopy() const;
};

class Skill;

class InventoryComponent : public IComponent
{
public:
	std::vector<std::shared_ptr<Skill>> skills;
	std::vector<std::string> ownedItemNames;

	InventoryComponent(); // We should add them manually.

private:
	void OnUpdate();
	IComponent* OnCopy() const;
};

class CollidableComponent : public IComponent
{
public:
	CollidableComponent();

private:
	void OnUpdate();
	IComponent* OnCopy() const;
};


#endif