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

class World;

class IComponent
{
private:
	virtual void OnUpdate(World *world) = 0;

	virtual void DoSerialization(std::ofstream &saveStream) const = 0;
	virtual void DoDeserialization(std::ifstream &loadStream) = 0;

public:
	ComponentType cType;
	int ownerID;

public:
	void Update(World *world);

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);

public:
	IComponent(int newOwnerID, ComponentType newCType);
	virtual ~IComponent();
};

class StatComponent : public IComponent
{
public:
	int health;
	int defense;
	int damage;
	int maxHealth;

	explicit StatComponent(int newOnwerID);
	StatComponent(int newHealth, int newDefense, int newDamage, int newMaxHealth, int newOwnerID);

private:
	void OnUpdate(World *world);

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class ParticleEmitterComponent : public IComponent
{
public:
	time_t particleEmitInterval_s;
	time_t lastTimeOfEmission_s;
	int particlesPerEmission;

	explicit ParticleEmitterComponent(int newOwnerID);
	ParticleEmitterComponent(time_t newParticleEmitInterval_s, time_t newLastTimeOfEmission_s,
							 int newParticlesPerEmission, int newOwnerID);

private:
	void OnUpdate(World *world);

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

	explicit MovableComponent(int newOwnerID);
	MovableComponent(const Position &newPosition, const Position &newPrevPosition, const Position &newDirection,
					 char newPrevTile, int newOwnerID);

private:
	void OnUpdate(World *world);

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class Entity;

class OwnableComponent : public IComponent
{
public:
	int ownedByID;

	explicit OwnableComponent(int newOwnerID);
	OwnableComponent(int newOwnedByID, int newOwnerID);

private:
	void OnUpdate(World *world);

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class ControllableComponent : public IComponent
{
public:
	explicit ControllableComponent(int newOwnerID);

private:
	void OnUpdate(World *world);

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class AIComponent : public IComponent
{
public:
	explicit AIComponent(int newOwnerID);

private:
	void OnUpdate(World *world);

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class Skill;

class InventoryComponent : public IComponent
{
public:
	std::vector<std::shared_ptr<Skill>> skills;
	std::vector<std::string> ownedItemNames;

	explicit InventoryComponent(int newOwnerID); // We should add them manually.

private:
	void OnUpdate(World *world);

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class CollidableComponent : public IComponent
{
public:
	explicit CollidableComponent(int newOwnerID);

private:
	void OnUpdate(World *world);

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class QuestInfoComponent : public IComponent
{
public:
	bool hasTalkedToNPC;

	explicit QuestInfoComponent(int newOwnerID);
	QuestInfoComponent(bool newHasTalkedToNPC, int newOwnerID);

private:
	void OnUpdate(World *world);
	IComponent* OnCopy() const;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif