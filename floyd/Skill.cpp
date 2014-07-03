#include "Skill.h"
#include "World.h"

#include <iostream>


/////////////
//  Skill  //
/////////////
Skill::Skill()
	: activationButton('\0')
{
}

Skill::Skill(char newActivationButton)
	: activationButton(newActivationButton)
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
		saveStream.write((char*)&activationButton, sizeof(char));
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


//////////////////////
//  Particle Skill  //
//////////////////////
ParticleSkill::ParticleSkill()
	: Skill(), damage(0)
{
}

ParticleSkill::ParticleSkill(char newActivationButton, int newDamage)
	: Skill(newActivationButton), damage(newDamage)
{
}

void ParticleSkill::OnApply(World *world)
{
	Position direction(1, 0);
	Position position = world->GetHero().GetPosition().PositionAfterMove(direction);

	world->AddParticle(position, direction, damage, true);
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