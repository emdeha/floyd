#include "Component.h"
#include "Entity.h"
#include "Skill.h"
#include "World.h"

#include <fstream>
#include <iostream>


IComponent::IComponent(int newOwnerID, ComponentType newCType)
	: cType(newCType), ownerID(newOwnerID)
{
}

IComponent::~IComponent()
{
}

void IComponent::Update(World *world)
{
	OnUpdate(world);
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
StatComponent::StatComponent(int newOwnerID)
	: health(0), defense(0), damage(0), maxHealth(0),
	  IComponent(newOwnerID, CTYPE_STAT)
{
}
StatComponent::StatComponent(int newHealth, int newDefense, int newDamage, int newMaxHealth, int newOwnerID)
	: health(newHealth), defense(newDefense), damage(newDamage), maxHealth(newMaxHealth),
	  IComponent(newOwnerID, CTYPE_STAT)
{
}

void StatComponent::OnUpdate(World *world)
{
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
ParticleEmitterComponent::ParticleEmitterComponent(int newOwnerID)
	: particleEmitInterval_s(0), lastTimeOfEmission_s(0), particlesPerEmission(0),
	  IComponent(newOwnerID, CTYPE_PARTICLE_EMITTER)
{
}
ParticleEmitterComponent::ParticleEmitterComponent(time_t newParticleEmitInterval_s,
												   time_t newLastTimeOfEmission_s,
												   int newParticlesPerEmission, int newOwnerID)
   : particleEmitInterval_s(newParticleEmitInterval_s), lastTimeOfEmission_s(newLastTimeOfEmission_s),
	 particlesPerEmission(newParticlesPerEmission), 
	 IComponent(newOwnerID, CTYPE_PARTICLE_EMITTER)
{
}

void ParticleEmitterComponent::OnUpdate(World *world)
{
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
MovableComponent::MovableComponent(int newOwnerID)
	: position(), prevPosition(), direction(), prevTile('\0'),
	  IComponent(newOwnerID, CTYPE_MOVABLE)
{
}
MovableComponent::MovableComponent(const Position &newPosition, const Position &newPrevPosition,
								   const Position &newDirection, char newPrevTile, int newOwnerID)
	: position(newPosition), prevPosition(newPrevPosition), direction(newDirection),
	  prevTile(newPrevTile),
	  IComponent(newOwnerID, CTYPE_MOVABLE)
{
}

void MovableComponent::OnUpdate(World *world)
{
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
OwnableComponent::OwnableComponent(int newOwnerID)
	: ownedByID(-1),
	  IComponent(newOwnerID, CTYPE_OWNABLE)
{
}
OwnableComponent::OwnableComponent(int newOwnedByID, int newOwnerID)
	: IComponent(newOwnerID, CTYPE_OWNABLE)
{
}

void OwnableComponent::OnUpdate(World *world)
{
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
ControllableComponent::ControllableComponent(int newOwnerID)
	: IComponent(newOwnerID, CTYPE_CONTROLLABLE)
{
}

void ControllableComponent::OnUpdate(World *world)
{
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
AIComponent::AIComponent(int newOwnerID)
	: IComponent(ownerID, CTYPE_AI)
{
}

void AIComponent::OnUpdate(World *world)
{
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
InventoryComponent::InventoryComponent(int newOwnerID)
	: skills(0), ownedItemNames(0), 
	  IComponent(newOwnerID, CTYPE_INVENTOY)
{
}

void InventoryComponent::OnUpdate(World *world)
{
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
CollidableComponent::CollidableComponent(int newOwnerID)
	: IComponent(newOwnerID, CTYPE_COLLIDABLE)
{
}

void CollidableComponent::OnUpdate(World *world)
{
}

void CollidableComponent::DoSerialization(std::ofstream &saveStream) const
{
}
void CollidableComponent::DoDeserialization(std::ifstream &loadStream)
{
}

///////////////////////////
//  QuestInfo Component  //
///////////////////////////
QuestInfoComponent::QuestInfoComponent(int newOwnerID)
	: hasTalkedToNPC(false),
	  IComponent(newOwnerID, CTYPE_QUEST_INFO)
{
}
QuestInfoComponent::QuestInfoComponent(bool newHasTalkedToNPC, int newOwnerID)
	: hasTalkedToNPC(newHasTalkedToNPC),
	  IComponent(newOwnerID, CTYPE_QUEST_INFO)
{
}

void QuestInfoComponent::OnUpdate(World *world)
{
}

void QuestInfoComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write((char*)&hasTalkedToNPC, sizeof(bool));
}
void QuestInfoComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&hasTalkedToNPC, sizeof(bool));
}