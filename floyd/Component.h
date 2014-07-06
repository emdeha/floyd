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
	CTYPE_QUEST_INFO,

	CTYPE_INVALID = -1
};

class IComponent
{
private:
	virtual void OnUpdate() = 0;
	virtual IComponent* OnCopy() const = 0;

	virtual void DoSerialization(std::ofstream &saveStream) const = 0;
	virtual void DoDeserialization(std::ifstream &loadStream) = 0;

public:
	ComponentType cType;

public:
	void Update();

	IComponent* Copy() const;

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);

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

	explicit StatComponent();
	StatComponent(int newHealth, int newDefense, int newDamage, int newMaxHealth);

private:
	void OnUpdate();
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class ParticleEmitterComponent : public IComponent
{
public:
	time_t particleEmitInterval_s;
	time_t lastTimeOfEmission_s;
	int particlesPerEmission;

	explicit ParticleEmitterComponent();
	ParticleEmitterComponent(time_t newParticleEmitInterval_s, time_t newLastTimeOfEmission_s,
							 int newParticlesPerEmission);

private:
	void OnUpdate();
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class MovableComponent : public IComponent
{
public:
	Position position;
	Position prevPosition;
	Position direction;
	char prevTile;

	explicit MovableComponent();
	MovableComponent(const Position &newPosition, const Position &newPrevPosition, const Position &newDirection,
					 char newPrevTile);

private:
	void OnUpdate();
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class Entity;

class OwnableComponent : public IComponent
{
public:
	std::shared_ptr<Entity> owner;

	explicit OwnableComponent();
	OwnableComponent(Entity *newOwner);

private:
	void OnUpdate();
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class ControllableComponent : public IComponent
{
public:
	explicit ControllableComponent();

private:
	void OnUpdate();
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class AIComponent : public IComponent
{
public:
	explicit AIComponent();

private:
	void OnUpdate();
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class Skill;

class InventoryComponent : public IComponent
{
public:
	std::vector<std::shared_ptr<Skill>> skills;
	std::vector<std::string> ownedItemNames;

	explicit InventoryComponent(); // We should add them manually.

private:
	void OnUpdate();
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class CollidableComponent : public IComponent
{
public:
	explicit CollidableComponent();

private:
	void OnUpdate();
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class QuestInfoComponent : public IComponent
{
public:
	bool hasTalkedToNPC;

	explicit QuestInfoComponent();
	QuestInfoComponent(bool newHasTalkedToNPC);

private:
	void OnUpdate();
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif