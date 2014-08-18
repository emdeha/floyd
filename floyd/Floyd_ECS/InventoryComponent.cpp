#include "stdafx.h"
#include "InventoryComponent.h"

#include "StatComponent.h"
#include "Entity.h"
#include "../Floyd_World/Item.h"
#include "../Floyd_World/Skill.h"
#include "../Floyd_General/Utils.h"

#include <fstream>
#include <sstream>


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