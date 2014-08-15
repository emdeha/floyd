#include "Skill.h"
#include "World.h"

#include <iostream>


/////////////
//  Skill  //
/////////////
Skill::Skill()
	: activationButton('\0'), skillName("")
{
}

Skill::Skill(char newActivationButton, const std::string &newSkillName)
	: activationButton(newActivationButton), skillName(newSkillName)
{
}

void Skill::Apply(World *world)
{
	OnApply(world);
}

void Skill::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		size_t skillNameLen = skillName.length();
		saveStream.write((char*)&skillNameLen, sizeof(skillNameLen));
		saveStream.write(&skillName[0], skillNameLen);
		saveStream.write(&activationButton, sizeof(activationButton));
	}
	else
	{
		std::cerr << "Error: Serializing Skill base class\n";
	}
	
	DoSerialization(saveStream);
}
void Skill::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		size_t skillNameLen = 0;
		loadStream.read((char*)&skillNameLen, sizeof(skillNameLen));
		skillName.resize(skillNameLen);
		loadStream.read(&skillName[0], skillNameLen);
		loadStream.read((char*)&activationButton, sizeof(char));
	}
	else
	{
		std::cerr << "Error: Deserializing Skill base class\n";
	}

	DoDeserialization(loadStream);
}

char Skill::GetActivationButton() const
{
	return activationButton;
}

const std::string& Skill::GetSkillName() const
{
	return skillName;
}


//////////////////////
//  Particle Skill  //
//////////////////////
ParticleSkill::ParticleSkill()
	: Skill(), damage(0)
{
}

ParticleSkill::ParticleSkill(char newActivationButton, const std::string &newSkillName, int newDamage)
	: Skill(newActivationButton, newSkillName), damage(newDamage)
{
}

void ParticleSkill::OnApply(World *world)
{
	Position direction(1, 0);
	Position position = world->GetPlayerPos().PositionAfterMove(direction);

	world->SpawnParticle(position, direction, damage, true);
}

void ParticleSkill::DoSerialization(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		saveStream.write((char*)&damage, sizeof(int));
	}
	else
	{
		std::cerr << "Error: Serializing ParticleSkill\n";
	}
}

void ParticleSkill::DoDeserialization(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		loadStream.read((char*)&damage, sizeof(int));
	}
	else
	{
		std::cerr << "Error: Deserializing ParticleSkill\n";
	}
}