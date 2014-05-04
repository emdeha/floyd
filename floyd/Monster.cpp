#include "Monster.h"


void Monster::Update()
{
	if (currentDelta < maxDeltaY)
	{
		prevPosition = position;
		position.x += deltaY;
		currentDelta++;
	}
	else 
	{
		deltaY *= -1;
		currentDelta = 0;
	}
}

void Monster::OnEvent(const Event &_event)
{
	EventType eType = _event.type;

	switch (eType)
	{
	case EVENT_ON_ATTACK:
		{
			const OnAttackEvent &evt = static_cast<const OnAttackEvent&>(_event);
			health -= evt.damage;
		}
		break;
	}
}