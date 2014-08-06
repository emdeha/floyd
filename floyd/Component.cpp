#include "Component.h"
#include "Entity.h"
#include "Skill.h"
#include "World.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <sstream>


IComponent::IComponent(ComponentType newCType)
	: cType(newCType)
{
}

IComponent::~IComponent()
{
	// delete owner; // We shouldn't do this. The owner is responsible for his deletion. Maybe a shared_ptr
					 // would be better.
}

void IComponent::SetOwner(Entity *newOwner)
{
	owner = newOwner;
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

void StatComponent::ApplyDamage(int dmg)
{
	health -= dmg / (defense == 0 ? 1 : defense);

	InventoryComponent *inventory = owner->GetComponentDirectly<InventoryComponent>(CTYPE_INVENTORY);
	if (inventory)
	{
		inventory->UpdateInfoSprite();
	}
}

///
///	line one - health
/// line two - damage
/// line three - defense
///
// TODO: Generalize for monsters, particles, bosses, heroes
void StatComponent::InitFromFile(const std::string &fileName)
{
	std::ifstream monster(fileName);

	if (monster.is_open())
	{
		std::string line;
		int idx = 0;
		while (std::getline(monster, line).good())
		{
			switch (idx)
			{
			case LINE_HEALTH:
				std::stringstream(line) >> health;
				break;
			case LINE_DAMAGE:
				std::stringstream(line) >> damage;
				break;
			default:
				std::cerr << "Warning: Invalid line idx\n";
			}
			++idx;
		}
	}
	else
	{
		std::cerr << "Error: Opening hero file!\n"; 
		return;
	}

	monster.close();
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
	lastTimeOfEmission_s = GetTimeSinceEpoch();
}
ParticleEmitterComponent::ParticleEmitterComponent(time_t newParticleEmitInterval_s,
												   time_t newLastTimeOfEmission_s,
												   int newParticlesPerEmission)
   : particleEmitInterval_s(newParticleEmitInterval_s), lastTimeOfEmission_s(newLastTimeOfEmission_s),
	 particlesPerEmission(newParticlesPerEmission), 
	 IComponent(CTYPE_PARTICLE_EMITTER)
{
}

void ParticleEmitterComponent::EmitParticle(World *world, const Position &pos, int damage, bool isFromHero)
{
	Position particleDir = GetRandomDirection();
	world->CreateParticle(pos.PositionAfterMove(particleDir), particleDir, damage, isFromHero);
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

//////////////////////////
//  Particle Component  //
//////////////////////////
ParticleComponent::ParticleComponent()
	: isEmittedFromHero(false),
	  IComponent(CTYPE_PARTICLE)
{
}
ParticleComponent::ParticleComponent(bool newIsEmittedFromHero)
	: isEmittedFromHero(newIsEmittedFromHero),
	  IComponent(CTYPE_PARTICLE)
{
}

void ParticleComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write((char*)&isEmittedFromHero, sizeof(isEmittedFromHero));
}
void ParticleComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&isEmittedFromHero, sizeof(isEmittedFromHero));
}

///////////////////////////
//  Transform Component  //
///////////////////////////
TransformComponent::TransformComponent()
	: position(), prevPosition(), direction(),
	  IComponent(CTYPE_TRANSFORM)
{
}
TransformComponent::TransformComponent(const Position &newPosition, const Position &newPrevPosition,
								       const Position &newDirection)
	: position(newPosition), prevPosition(newPrevPosition), direction(newDirection),
	  IComponent(CTYPE_TRANSFORM)
{
}

void TransformComponent::GoToPrevPos()
{
	position = prevPosition;
}

void TransformComponent::DoSerialization(std::ofstream &saveStream) const
{
	position.Serialize(saveStream);
	prevPosition.Serialize(saveStream);
	direction.Serialize(saveStream);
}
void TransformComponent::DoDeserialization(std::ifstream &loadStream)
{
	position.Deserialize(loadStream);
	prevPosition.Deserialize(loadStream);
	direction.Deserialize(loadStream);
}

/////////////////////////
//  Ownable Component  //
/////////////////////////
OwnableComponent::OwnableComponent()
	: ownedByID(-1),
	  IComponent(CTYPE_OWNABLE)
{
}
OwnableComponent::OwnableComponent(int newOwnedByID)
	: IComponent(CTYPE_OWNABLE)
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
ControllableComponent::ControllableComponent()
	: IComponent(CTYPE_CONTROLLABLE)
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
AIComponent::AIComponent()
	: IComponent(CTYPE_AI)
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
InventoryComponent::InventoryComponent()
	: skills(0), ownedItemNames(0), 
	  IComponent(CTYPE_INVENTORY)
{
}

void InventoryComponent::AddItem(const Item *newItem)
{
	StatComponent *ownerStat = owner->GetComponentDirectly<StatComponent>(CTYPE_STAT);

	if (newItem->IsValid())
	{
		ownerStat->damage += newItem->GetDamage();
		ownerStat->defense += newItem->GetDefense();
		ownerStat->health += newItem->GetHealth();
		if (newItem->GetAttribute() == ATTRIB_PARTICLE && 
			skills.empty()) // Hack for avoiding adding duplicate skills to the Hero
		{
			skills.push_back(std::make_shared<ParticleSkill>(KEY_USE_SKILL, 5));
		}
		
		if ( ! newItem->IsBuff()) // We only want to show the names of the owned items, not the acquired buffs.
		{
			ownedItemNames.push_back(newItem->GetName());
		}

		UpdateInfoSprite();
	}
}

void InventoryComponent::UpdateInfoSprite()
{
	StatComponent *ownerStat = owner->GetComponentDirectly<StatComponent>(CTYPE_STAT);
	int ownerHealth = ownerStat->health;
	int ownerDamage = ownerStat->damage;
	int ownerDefense = ownerStat->defense;

	// TODO: Add boss health

	std::string health("Health: ");
	health += std::to_string(ownerHealth);
	int healthLen = health.length();
	int maxLen = healthLen;

	// TODO: Add boss health

	std::string damage("Damage: ");
	damage += std::to_string(ownerDamage);
	int damageLen = damage.length();
	if (damageLen > maxLen)
	{
		maxLen = damageLen;
	}

	std::string defense("Defense: ");
	defense += std::to_string(ownerDefense);
	int defenseLen = defense.length();
	if (defenseLen > maxLen)
	{
		maxLen = defenseLen;
	}

	if (healthLen < maxLen)
	{
		health.append(maxLen - healthLen, ' ');
	}
	if (damageLen < maxLen)
	{
		damage.append(maxLen - damageLen, ' ');
	}
	if (defenseLen < maxLen)
	{
		defense.append(maxLen - defenseLen, ' ');
	}

	health += '\n'; damage += '\n'; defense += '\n';

	std::string info;
	info = health + damage + defense;


	//size_t itemNamesSize = ownedItemNames.size();
	//if (itemNamesSize > 0)
	//{
	//	info << "Items: ";
	//	for (size_t idx = 0; idx < itemNamesSize; ++idx)
	//	{
	//		info << ownedItemNames[idx];
	//		if (idx < itemNamesSize - 1)
	//		{
	//			info << ", ";
	//		}
	//	}
	//	info << '\n';
	//}

	infoAsSprite = Sprite(maxLen, 3);
	infoAsSprite.LoadTextureFromRawData(info);
}

const Sprite* InventoryComponent::GetInfoAsSprite() const
{
	return &infoAsSprite;
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
	: onCollision(nullptr),
	  IComponent(CTYPE_COLLIDABLE)
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
QuestInfoComponent::QuestInfoComponent()
	: hasTalkedToNPC(false),
	  IComponent(CTYPE_QUEST_INFO)
{
}
QuestInfoComponent::QuestInfoComponent(bool newHasTalkedToNPC)
	: hasTalkedToNPC(newHasTalkedToNPC),
	  IComponent(CTYPE_QUEST_INFO)
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

//////////////////////////
//  Drawable Component  //
//////////////////////////
DrawableComponent::DrawableComponent()
	: sprite(),
	  IComponent(CTYPE_DRAWABLE)
{
}

void DrawableComponent::DoSerialization(std::ofstream &saveStream) const
{
}
void DrawableComponent::DoDeserialization(std::ifstream &loadStream)
{
}

//////////////////////////
//  Animated Component  //
//////////////////////////
AnimatedComponent::AnimatedComponent()
	: animPoints(0), currentAnimPointIdx(0),
	  IComponent(CTYPE_ANIMATED)
{
}

void AnimatedComponent::AddAnimPoint(const Position &animPoint)
{
	animPoints.push_back(animPoint);
}

void AnimatedComponent::UpdateAnim()
{
	++currentAnimPointIdx;
	if (currentAnimPointIdx >= animPoints.size())
	{
		currentAnimPointIdx	= 0;
	}
}

Position AnimatedComponent::GetCurrentAnimPos() const
{
	return animPoints[currentAnimPointIdx];
}

void AnimatedComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write((char*)currentAnimPointIdx, sizeof(currentAnimPointIdx));
}
void AnimatedComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)currentAnimPointIdx, sizeof(currentAnimPointIdx));
}