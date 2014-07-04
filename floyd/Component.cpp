#include "Component.h"
#include "Entity.h"


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

//////////////////////
//  Stat Component  //
//////////////////////
StatComponent::StatComponent(int newHealth, int newDefense, int newDamage, int newMaxHealth)
	: health(newHealth), defense(newDefense), damage(newDamage), maxHealth(newMaxHealth),
	  IComponent(CTYPE_STAT)
{
}

/////////////////////////////////
//  ParticleEmitter Component  //
/////////////////////////////////
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

/////////////////////////
//  Movable Component  //
/////////////////////////
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

/////////////////////////
//  Ownable Component  //
/////////////////////////
OwnableComponent::OwnableComponent(Entity *newOwner)
	: owner(newOwner), // Init it properly
	  IComponent(CTYPE_OWNABLE)
{
}

OwnableComponent::~OwnableComponent()
{
	delete owner; // Should we? Maybe the owner should delete himself.
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

///////////////////////////
//  Inventory Component  //
///////////////////////////
InventoryComponent::InventoryComponent()
	: IComponent(CTYPE_INVENTOY)
{
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