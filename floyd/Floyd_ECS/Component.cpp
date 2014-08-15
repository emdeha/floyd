#include "stdafx.h"
#include "Component.h"

#include "Entity.h"
#include "../Skill.h"
#include "../World.h"
#include "../Floyd_General/Utils.h"
#include "../Floyd_General/Reporting.h"
#include "../Floyd_Scripts/ScriptDispatcher.h"

#include <fstream>
#include <iostream>
#include <sstream>


IComponent::IComponent(ComponentType newCType)
	: cType(newCType), group("")
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
		size_t groupLen = group.length();
		saveStream.write((char*)&groupLen, sizeof(groupLen));
		saveStream.write(&group[0], groupLen);
		DoSerialization(saveStream);
	}
	else
	{
		std::stringstream msg("Error: serializing component: ");
		msg << cType;
		Report::Error(msg.str(), __LINE__, __FILE__);
	}
}
void IComponent::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		size_t groupLen = 0;
		loadStream.read((char*)&groupLen, sizeof(groupLen));
		group.resize(groupLen);
		loadStream.read(&group[0], groupLen);
		DoDeserialization(loadStream);
	}
	else
	{
		std::stringstream msg("Error: deserializing component: ");
		msg << cType;
		Report::Error(msg.str(), __LINE__, __FILE__);
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

Sprite* StatComponent::GetHealthBarAsSprite()
{
	float healtQuotient = float(health) / float(maxHealth);
	int healthPercentage = int(healtQuotient * 100.0f);
	
	std::string healthBar_raw(healthPercentage / 3, '!');
	healthBar_raw += '\n'; // We must exclude this from the length.

	healthBar = Sprite(healthBar_raw.length() - 1, 1);
	healthBar.LoadTextureFromRawData(healthBar_raw);

	return &healthBar;
}

///
///	line one - health
/// line two - damage
/// line three - defense
///
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
				maxHealth = health;
				break;
			case LINE_DAMAGE:
				std::stringstream(line) >> damage;
				break;
			case LINE_DEFENSE:
				std::stringstream(line) >> defense;
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
	saveStream.write((char*)&health, sizeof(health));
	saveStream.write((char*)&defense, sizeof(defense));
	saveStream.write((char*)&damage, sizeof(damage));
	saveStream.write((char*)&maxHealth, sizeof(maxHealth));
}
void StatComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&health, sizeof(health));
	loadStream.read((char*)&defense, sizeof(defense));
	loadStream.read((char*)&damage, sizeof(damage));
	loadStream.read((char*)&maxHealth, sizeof(maxHealth));
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
	world->SpawnParticle(pos.PositionAfterMove(particleDir), particleDir, damage, isFromHero);
}

void ParticleEmitterComponent::DoSerialization(std::ofstream &saveStream) const
{ 
	saveStream.write((char*)&particleEmitInterval_s, sizeof(particleEmitInterval_s));
	saveStream.write((char*)&lastTimeOfEmission_s, sizeof(lastTimeOfEmission_s));
	saveStream.write((char*)&particlesPerEmission, sizeof(particlesPerEmission));
}
void ParticleEmitterComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&particleEmitInterval_s, sizeof(particleEmitInterval_s));
	loadStream.read((char*)&lastTimeOfEmission_s, sizeof(lastTimeOfEmission_s));
	loadStream.read((char*)&particlesPerEmission, sizeof(particlesPerEmission));
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

//////////////////////////////
//  Controllable Component  //
//////////////////////////////
ControllableComponent::ControllableComponent()
	: IComponent(CTYPE_CONTROLLABLE)
{
}

void ControllableComponent::SetOnKeyPressed(OnKeyPressedScript newOnKeyPressed, const std::string &scriptGroup)
{
	onKeyPressed = newOnKeyPressed;
	group = scriptGroup;
}
void ControllableComponent::CallOnKeyPressed(World *world, char key)
{
	if (onKeyPressed)
	{
		onKeyPressed(world, owner, key);
	}
}

void ControllableComponent::DoSerialization(std::ofstream &saveStream) const
{
}
void ControllableComponent::DoDeserialization(std::ifstream &loadStream)
{
	onKeyPressed = Floyd::GetOnKeyPressed(group);
}

////////////////////
//  AI Component  //
////////////////////
AIComponent::AIComponent()
	: IComponent(CTYPE_AI)
{
}

void AIComponent::SetOnUpdateAI(OnUpdateAIScript newOnUpdateAI, const std::string &scriptGroup)
{
	onUpdateAI = newOnUpdateAI;
	group = scriptGroup;
}
void AIComponent::CallOnUpdateAI(World *world)
{
	if (onUpdateAI)
	{
		onUpdateAI(world, owner);
	}
}

void AIComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write((char*)&aiType, sizeof(aiType));
}
void AIComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&aiType, sizeof(aiType));

	onUpdateAI = Floyd::GetOnUpdateAI(group);
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
			skills.push_back(std::make_shared<ParticleSkill>(KEY_USE_SKILL, newItem->GetName(), 5));
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

	std::string health("Health: ");
	health += std::to_string(ownerHealth);
	size_t healthLen = health.length();
	size_t maxLen = healthLen;

	std::string damage("Damage: ");
	damage += std::to_string(ownerDamage);
	size_t damageLen = damage.length();
	if (damageLen > maxLen)
	{
		maxLen = damageLen;
	}

	std::string defense("Defense: ");
	defense += std::to_string(ownerDefense);
	size_t defenseLen = defense.length();
	if (defenseLen > maxLen)
	{
		maxLen = defenseLen;
	}

	std::vector<std::string> skillInfo(0);
	if (skills.size() > 0)
	{
		for (auto skill = skills.begin(); skill != skills.end(); ++skill)
		{
			std::ostringstream skillName;
			skillName << "To activate '" << (*skill)->GetSkillName() << "' press '" << (*skill)->GetActivationButton() << "'";
			std::string skillNameStr = skillName.str();
			size_t skillNameLen = skillNameStr.length();
			if (skillNameLen > maxLen)
			{
				maxLen = skillNameLen;
			}
			skillInfo.push_back(skillNameStr);
		}

		for (auto name = skillInfo.begin(); name != skillInfo.end(); ++name)
		{
			size_t nameLen = name->length();
			if (nameLen < maxLen)
			{
				name->append(maxLen - nameLen, ' ');
			}
			(*name) += '\n';
		}
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
	size_t infoHeight = 3;
	info = health + damage + defense;

	for (auto name = skillInfo.begin(); name != skillInfo.end(); ++name)
	{
		info += (*name);
		++infoHeight;
	}

	infoAsSprite = Sprite(maxLen, infoHeight);
	infoAsSprite.LoadTextureFromRawData(info);
}

const Sprite* InventoryComponent::GetInfoAsSprite() const
{
	return &infoAsSprite;
}

void InventoryComponent::DoSerialization(std::ofstream &saveStream) const
{
	size_t itemNamesCount = ownedItemNames.size();
	saveStream.write((char*)&itemNamesCount, sizeof(itemNamesCount));
	for (auto itemName = ownedItemNames.begin(); itemName != ownedItemNames.end(); ++itemName)
	{
		size_t itemNameLength = itemName->length();
		saveStream.write((char*)&itemNameLength, sizeof(itemNameLength));
		saveStream.write(itemName->c_str(), itemNameLength * sizeof(char));
	}
	size_t skillsCount = skills.size();
	saveStream.write((char*)&skillsCount, sizeof(skillsCount));
	for (auto skill = skills.begin(); skill != skills.end(); ++skill)
	{
		// Should query the type and **SAVE** the appropriate skill.
		// factoryfactoryfactory
		(*skill)->Serialize(saveStream);
	}

	infoAsSprite.Serialize(saveStream);
}
void InventoryComponent::DoDeserialization(std::ifstream &loadStream)
{
	size_t itemNamesSize = 0;
	loadStream.read((char*)&itemNamesSize, sizeof(itemNamesSize));
	for (size_t idx = 0; idx < itemNamesSize; ++idx)
	{
		size_t newItemNameLength = 0;
		loadStream.read((char*)&newItemNameLength, sizeof(newItemNameLength));
		char newItemName[50] = "";
		loadStream.read(newItemName, newItemNameLength * sizeof(char));
		ownedItemNames.push_back(newItemName);
	}
	size_t skillsSize = 0;
	loadStream.read((char*)&skillsSize, sizeof(skillsSize));
	for (size_t idx = 0; idx < skillsSize; ++idx)
	{
		// Should query the type and **CREATE** the appropriate skill.
		// factoryfactoryfactory
		std::shared_ptr<ParticleSkill> newSkill = std::make_shared<ParticleSkill>();
		newSkill->Deserialize(loadStream);
		skills.push_back(newSkill);
	}

	infoAsSprite.Deserialize(loadStream);
}

////////////////////////////
//  Collidable Component  //
////////////////////////////
CollidableComponent::CollidableComponent()
	: onCollision(nullptr),
	  IComponent(CTYPE_COLLIDABLE)
{
}

void CollidableComponent::SetOnCollision(OnCollision newOnCollision, const std::string &scriptGroup)
{
	onCollision = newOnCollision;
	group = scriptGroup;
}
void CollidableComponent::CallOnCollision(World *world, const Tile *collider)
{
	if (onCollision)
	{
		onCollision(world, owner, collider);
	}
}

void CollidableComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write(collisionInfo, INFOTYPE_COUNT);
}
void CollidableComponent::DoDeserialization(std::ifstream &loadStream)
{
	onCollision = Floyd::GetOnCollision(group);
	loadStream.read(collisionInfo, INFOTYPE_COUNT);
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
	saveStream.write((char*)&hasTalkedToNPC, sizeof(hasTalkedToNPC));
}
void QuestInfoComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&hasTalkedToNPC, sizeof(hasTalkedToNPC));
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
	sprite.Serialize(saveStream);
}
void DrawableComponent::DoDeserialization(std::ifstream &loadStream)
{
	sprite.Deserialize(loadStream);
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
	saveStream.write((char*)&currentAnimPointIdx, sizeof(currentAnimPointIdx));

	size_t animPointsSize = animPoints.size();
	saveStream.write((char*)&animPointsSize, sizeof(animPointsSize));
	for (auto animPoint = animPoints.begin(); animPoint != animPoints.end(); ++animPoint)
	{
		animPoint->Serialize(saveStream);
	}
}
void AnimatedComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&currentAnimPointIdx, sizeof(currentAnimPointIdx));

	size_t animPointsSize = 0;
	loadStream.read((char*)&animPointsSize, sizeof(animPointsSize));
	for (size_t idx = 0; idx < animPointsSize; ++idx)
	{
		Position newPoint;
		newPoint.Deserialize(loadStream);
		AddAnimPoint(newPoint);
	}
}