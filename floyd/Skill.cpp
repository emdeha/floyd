#include "Skill.h"
#include "World.h"


/////////////
//  Skill  //
/////////////
Skill::Skill(char newActivationButton)
	: activationButton(newActivationButton)
{
}

void Skill::Apply(World *world)
{
	OnApply(world);
}

char Skill::GetActivationButton() const
{
	return activationButton;
}


//////////////////////
//  Particle Skill  //
//////////////////////
ParticleSkill::ParticleSkill(char newActivationButton, int newDamage)
	: Skill(newActivationButton), damage(newDamage)
{
}

void ParticleSkill::OnApply(World *world)
{
	Position direction(1, 0);
	world->AddParticle(world->GetHero().GetPosition(), direction, damage, true);
}