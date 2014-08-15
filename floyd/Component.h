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
	CTYPE_PARTICLE,
	CTYPE_TRANSFORM,
	CTYPE_OWNABLE,
	CTYPE_CONTROLLABLE,
	CTYPE_AI,
	CTYPE_COLLIDABLE,
	CTYPE_INVENTORY,
	CTYPE_QUEST_INFO,
	CTYPE_DRAWABLE,
	CTYPE_ANIMATED,

	CTYPE_INVALID = -1
};

enum AIType
{
	AITYPE_BOSS,
	AITYPE_PARTICLE,
	AITYPE_MONSTER,

	AITYPE_INVALID = -1
};

enum InfoType
{
	INFOTYPE_LOGICAL_SPRITE,
	INFOTYPE_SPRITE,

	INFOTYPE_COUNT,

	INFOTYPE_INVALID = -1
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
	std::string group;
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

	Sprite healthBar;

	explicit StatComponent();
	StatComponent(int newHealth, int newDefense, int newDamage, int newMaxHealth);

public:
	void ApplyDamage(int dmg);

	Sprite* GetHealthBarAsSprite();

	void InitFromFile(const std::string &fileName);

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
	
public:
	void EmitParticle(World *world, const Position &pos, int damage, bool isFromHero);

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class ParticleComponent : public IComponent
{
public:
	bool isEmittedFromHero;

	explicit ParticleComponent();
	ParticleComponent(bool newIsEmittedFromHero);

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

	explicit TransformComponent();
	TransformComponent(const Position &newPosition, const Position &newPrevPosition, const Position &newDirection);

public:
	void GoToPrevPos();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class ControllableComponent : public IComponent
{
public:
	typedef void (*OnKeyPressedScript)(World*, Entity*, char);

	void SetOnKeyPressed(OnKeyPressedScript newOnKeyPressed, const std::string &scriptGroup);
	void CallOnKeyPressed(World *world, char key);

	explicit ControllableComponent();

private:
	OnKeyPressedScript onKeyPressed;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class AIComponent : public IComponent
{
public:
	AIType aiType;

	typedef void (*OnUpdateAIScript)(World*, Entity*);

	void SetOnUpdateAI(OnUpdateAIScript newOnUpdateAI, const std::string &scriptGroup);
	void CallOnUpdateAI(World *world);

	explicit AIComponent();

private:
	OnUpdateAIScript onUpdateAI;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class Skill;

class InventoryComponent : public IComponent
{
public:
	std::vector<std::shared_ptr<Skill>> skills;
	std::vector<std::string> ownedItemNames;

	Sprite infoAsSprite;

	explicit InventoryComponent(); // We should add them manually.

public:
	void AddItem(const Item *newItem);
	const Sprite* GetInfoAsSprite() const;

	void UpdateInfoSprite();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};

class CollidableComponent : public IComponent
{
public:
	char collisionInfo[INFOTYPE_COUNT];

	typedef void (*OnCollision)(World*, Entity*, const Tile*);

	void SetOnCollision(OnCollision newOnCollision, const std::string &scriptGroup);
	void CallOnCollision(World *world, const Tile *collider);

	explicit CollidableComponent();

private:
	OnCollision onCollision;

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

class AnimatedComponent : public IComponent
{
private:
	std::vector<Position> animPoints;
	size_t currentAnimPointIdx;

public:
	void AddAnimPoint(const Position &animPoint);
	void UpdateAnim();
	Position GetCurrentAnimPos() const;

	explicit AnimatedComponent();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif