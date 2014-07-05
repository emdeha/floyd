#include "Component.h"
#include "Entity.h"
#include "Skill.h"

#include <fstream>
#include <iostream>


IComponent::IComponent(ComponentType newCType)
	: cType(newCType)
{
}

IComponent::~IComponent()
{
}

void IComponent::Update()
{
	OnUpdate();
}

IComponent* IComponent::Copy() const
{
	return OnCopy();
}

void IComponent::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		DoSerialization(saveStream);
	}
	else
	{
		std::cerr << "Error: Serializing component: " << cType << "\n";
	}
}
void IComponent::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		DoDeserialization(loadStream);
	}
	else
	{
		std::cerr << "Error: Deserializing component: " << cType << "\n";
	}
}

//////////////////////
//  Stat Component  //
//////////////////////
StatComponent::StatComponent()
	: health(0), defense(0), damage(0), maxHealth(0),
	  IComponent(CTYPE_STAT)
{
}
StatComponent::StatComponent(int newHealth, int newDefense, int newDamage, int newMaxHealth)
	: health(newHealth), defense(newDefense), damage(newDamage), maxHealth(newMaxHealth),
	  IComponent(CTYPE_STAT)
{
}

void StatComponent::OnUpdate()
{
}

IComponent* StatComponent::OnCopy() const
{
	return new StatComponent(health, defense, damage, maxHealth);
}

void StatComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write((char*)&health, sizeof(int));
	saveStream.write((char*)&defense, sizeof(int));
	saveStream.write((char*)&damage, sizeof(int));
	saveStream.write((char*)&maxHealth, sizeof(int));
}
void StatComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&health, sizeof(int));
	loadStream.read((char*)&defense, sizeof(int));
	loadStream.read((char*)&damage, sizeof(int));
	loadStream.read((char*)&maxHealth, sizeof(int));
}

/////////////////////////////////
//  ParticleEmitter Component  //
/////////////////////////////////
ParticleEmitterComponent::ParticleEmitterComponent()
	: particleEmitInterval_s(0), lastTimeOfEmission_s(0), particlesPerEmission(0),
	  IComponent(CTYPE_PARTICLE_EMITTER)
{
}
ParticleEmitterComponent::ParticleEmitterComponent(time_t newParticleEmitInterval_s,
												   time_t newLastTimeOfEmission_s,
												   int newParticlesPerEmission)
   : particleEmitInterval_s(newParticleEmitInterval_s), lastTimeOfEmission_s(newLastTimeOfEmission_s),
	 particlesPerEmission(newParticlesPerEmission), 
	 IComponent(CTYPE_PARTICLE_EMITTER)
{
}

void ParticleEmitterComponent::OnUpdate()
{
}

IComponent* ParticleEmitterComponent::OnCopy() const
{
	return new ParticleEmitterComponent(particleEmitInterval_s, lastTimeOfEmission_s, particlesPerEmission);
}

void ParticleEmitterComponent::DoSerialization(std::ofstream &saveStream) const
{ 
	saveStream.write((char*)&particleEmitInterval_s, sizeof(time_t));
	saveStream.write((char*)&lastTimeOfEmission_s, sizeof(time_t)); // WARN: May cause bugs. Better init it with
																	//       current time.
	saveStream.write((char*)&particlesPerEmission, sizeof(int));
}
void ParticleEmitterComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&particleEmitInterval_s, sizeof(time_t));
	loadStream.read((char*)&lastTimeOfEmission_s, sizeof(time_t)); // WARN: May cause bugs. Better init it with
	   														       //       current time.
	loadStream.read((char*)&particlesPerEmission, sizeof(int));
}

/////////////////////////
//  Movable Component  //
/////////////////////////
MovableComponent::MovableComponent()
	: position(), prevPosition(), direction(), prevTile('\0'),
	  IComponent(CTYPE_MOVABLE)
{
}
MovableComponent::MovableComponent(const Position &newPosition, const Position &newPrevPosition,
								   const Position &newDirection, char newPrevTile)
	: position(newPosition), prevPosition(newPrevPosition), direction(newDirection),
	  prevTile(newPrevTile),
	  IComponent(CTYPE_MOVABLE)
{
}

void MovableComponent::OnUpdate()
{
}

IComponent* MovableComponent::OnCopy() const
{
	return new MovableComponent(position, prevPosition, direction, prevTile);
}

void MovableComponent::DoSerialization(std::ofstream &saveStream) const
{
	position.Serialize(saveStream);
	prevPosition.Serialize(saveStream);
	direction.Serialize(saveStream);
	saveStream.write((char*)&prevTile, sizeof(char));
}
void MovableComponent::DoDeserialization(std::ifstream &loadStream)
{
	position.Deserialize(loadStream);
	prevPosition.Deserialize(loadStream);
	direction.Deserialize(loadStream);
	loadStream.read((char*)&prevTile, sizeof(char));
}

/////////////////////////
//  Ownable Component  //
/////////////////////////
OwnableComponent::OwnableComponent()
	: owner(nullptr),
	  IComponent(CTYPE_OWNABLE)
{
}
OwnableComponent::OwnableComponent(Entity *newOwner)
	: owner(newOwner), // Init it properly
	  IComponent(CTYPE_OWNABLE)
{
}

void OwnableComponent::OnUpdate()
{
}

IComponent* OwnableComponent::OnCopy() const
{
	return new OwnableComponent(owner.get());
}

void OwnableComponent::DoSerialization(std::ofstream &saveStream) const
{
	// Save owner
	// Maybe the owner better be a simple enum
}
void OwnableComponent::DoDeserialization(std::ifstream &loadStream)
{
	// Load owner?
}

//////////////////////////////
//  Controllable Component  //
//////////////////////////////
ControllableComponent::ControllableComponent()
	: IComponent(CTYPE_CONTROLLABLE)
{
}

void ControllableComponent::OnUpdate()
{
}

IComponent* ControllableComponent::OnCopy() const
{
	return new ControllableComponent();
}

void ControllableComponent::DoSerialization(std::ofstream &saveStream) const
{
}
void ControllableComponent::DoDeserialization(std::ifstream &loadStream)
{
}

////////////////////
//  AI Component  //
////////////////////
AIComponent::AIComponent()
	: IComponent(CTYPE_AI)
{
}

void AIComponent::OnUpdate()
{
}

IComponent* AIComponent::OnCopy() const
{
	return new AIComponent();
}

void AIComponent::DoSerialization(std::ofstream &saveStream) const
{
}
void AIComponent::DoDeserialization(std::ifstream &loadStream)
{
}

///////////////////////////
//  Inventory Component  //
///////////////////////////
InventoryComponent::InventoryComponent()
	: skills(0), ownedItemNames(0), IComponent(CTYPE_INVENTOY)
{
}

void InventoryComponent::OnUpdate()
{
}

IComponent* InventoryComponent::OnCopy() const
{
	return new InventoryComponent();
}

void InventoryComponent::DoSerialization(std::ofstream &saveStream) const
{
	size_t itemNamesCount = ownedItemNames.size();
	saveStream.write((char*)&itemNamesCount, sizeof(size_t));
	for (auto itemName = ownedItemNames.begin(); itemName != ownedItemNames.end(); ++itemName)
	{
		size_t itemNameLength = itemName->length();
		saveStream.write((char*)&itemNameLength, sizeof(size_t));
		saveStream.write(itemName->c_str(), itemNameLength * sizeof(char));
	}
	size_t skillsCount = skills.size();
	saveStream.write((char*)&skillsCount, sizeof(size_t));
	for (auto skill = skills.begin(); skill != skills.end(); ++skill)
	{
		// Should query the type and **SAVE** the appropriate skill.
		// factoryfactoryfactory
		(*skill)->Serialize(saveStream);
	}
}
void InventoryComponent::DoDeserialization(std::ifstream &loadStream)
{
	size_t itemNamesSize = 0;
	loadStream.read((char*)&itemNamesSize, sizeof(size_t));
	for (size_t idx = 0; idx < itemNamesSize; ++idx)
	{
		size_t newItemNameLength = 0;
		loadStream.read((char*)&newItemNameLength, sizeof(size_t));
		char newItemName[50] = "";
		loadStream.read(newItemName, newItemNameLength * sizeof(char));
		ownedItemNames.push_back(newItemName);
	}
	size_t skillsSize = 0;
	loadStream.read((char*)&skillsSize, sizeof(size_t));
	for (size_t idx = 0; idx < skillsSize; ++idx)
	{
		// Should query the type and **CREATE** the appropriate skill.
		// factoryfactoryfactory
		std::shared_ptr<ParticleSkill> newSkill = std::make_shared<ParticleSkill>();
		newSkill->Deserialize(loadStream);
		skills.push_back(newSkill);
	}
}

////////////////////////////
//  Collidable Component  //
////////////////////////////
CollidableComponent::CollidableComponent()
	: IComponent(CTYPE_COLLIDABLE)
{
}

void CollidableComponent::OnUpdate()
{
}

IComponent* CollidableComponent::OnCopy() const
{
	return new CollidableComponent();
}

void CollidableComponent::DoSerialization(std::ofstream &saveStream) const
{
}
void CollidableComponent::DoDeserialization(std::ifstream &loadStream)
{
}