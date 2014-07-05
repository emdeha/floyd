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

IComponent* IComponent::Copy() const
{
	return OnCopy();
}

//////////////////////
//  Stat Component  //
//////////////////////
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

IComponent* ParticleEmitterComponent::OnCopy() const
{
	return new ParticleEmitterComponent(particleEmitInterval_s, lastTimeOfEmission_s, particlesPerEmission);
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

IComponent* MovableComponent::OnCopy() const
{
	return new MovableComponent(position, prevPosition, direction, prevTile);
}

/////////////////////////
//  Ownable Component  //
/////////////////////////
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