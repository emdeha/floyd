#include "stdafx.h"
#include "StatComponent.h"

#include "Entity.h"
#include "../Floyd_General/Utils.h"
#include "../Floyd_General/Reporting.h"
#include "../Floyd_ECS/InventoryComponent.h"

#include <fstream>
#include <sstream>


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
				Report::Warning("Invalid line idx", __LINE__, __FILE__);
			}
			++idx;
		}
	}
	else
	{
		Report::Error("Cannot open hero file", __LINE__, __FILE__);
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
