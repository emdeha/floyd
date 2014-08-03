#ifndef FLOYD_COMPONENT_H
#define FLOYD_COMPONENT_H


#include <time.h>
#include <vector>
#include <string>
#include <memory>

#include "Floyd_Geometry/Vector.h"
#include "Floyd_Graphics/Sprite.h"


enum ComponentType
{
	CTYPE_STAT,
	CTYPE_PARTICLE_EMITTER,
	CTYPE_TRANSFORM,
	CTYPE_OWNABLE,
	CTYPE_CONTROLLABLE,
	CTYPE_AI,
	CTYPE_COLLIDABLE,
	CTYPE_INVENTORY,
	CTYPE_QUEST_INFO,
	CTYPE_DRAWABLE,

	CTYPE_INVALID = -1
};

class World;
class Entity;
class Item;
struct Tile;

class IComponent
{
private:
	virtual void DoSerialization(std::ofstream &saveStream) const = 0;
	virtual void DoDeserialization(std::ifstream &loadStream) = 0;

public:
	ComponentType cType;
	Entity *owner;

public:
	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
	
	void SetOwner(Entity *newOwner);

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

public:
	void ApplyDamage(int dmg);

private:
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
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class TransformComponent : public IComponent
{
public:
	Position position;
	Position prevPosition;
	Position direction;
	char prevTile;

	explicit TransformComponent();
	TransformComponent(const Position &newPosition, const Position &newPrevPosition, const Position &newDirection,
					   char newPrevTile);

public:
	void GoToPrevPos();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class OwnableComponent : public IComponent
{
public:
	int ownedByID;

	explicit OwnableComponent();
	OwnableComponent(int newOwnedByID);

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class ControllableComponent : public IComponent
{
public:
	typedef void (*OnKeyPressedScript)(Entity*, char);
	OnKeyPressedScript script;

	explicit ControllableComponent();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class AIComponent : public IComponent
{
public:
	explicit AIComponent();

private:
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

public:
	void AddItem(const Item *newItem);

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class CollidableComponent : public IComponent
{
public:
	typedef void (*OnCollision)(World*, Entity*, const Tile*);
	OnCollision onCollision;

	explicit CollidableComponent();

private:
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
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class DrawableComponent : public IComponent
{
public:
	Sprite sprite;

	explicit DrawableComponent();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif