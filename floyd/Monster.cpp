#include "Monster.h"
#include "World.h"


void Monster::Update(World *world)
{
	//if (currentDelta >= maxDeltaY)
	//{
	//	deltaY *= -1;
	//	currentDelta = 0;
	//}

	//prevPosition = position;
	//position.x += deltaY;
	//currentDelta++;

	time_t timeSinceStart_s = GetTimeSinceEpoch();
	if (timeSinceStart_s - lastTimeOfEmission_s > particleEmitInterval_s)
	{
		EmitParticle(world);
		lastTimeOfEmission_s = timeSinceStart_s;
	}
}

void Monster::EmitParticle(World *world)
{
	// TODO: No need to wrap it in a method if it stays an one-liner
	world->AddParticle(position);
}

//void Monster::OnEvent(const Event &_event)
//{
//	EventType eType = _event.type;

//	switch (eType)
//	{
//	case EVENT_ON_ATTACK:
//		{
//			const OnAttackEvent &evt = static_cast<const OnAttackEvent&>(_event);
//			if (evt.chType == CHARACTER_MONSTER)
//			{
//				health -= evt.damage;
//			}
//		}
//		break;
//	}
//}