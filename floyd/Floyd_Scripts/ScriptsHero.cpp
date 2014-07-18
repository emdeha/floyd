#include "stdafx.h"
#include "ScriptsHero.h"

#include "../Entity.h"
#include "../Reporting.h"
#include "../Utils.h"


void Move(Direction dir, TransformComponent *heroTransform)
{
	heroTransform->prevPosition = heroTransform->position;
	
	switch (dir)
	{
	case DIR_RIGHT:
		heroTransform->position.x += 1;
		break;
	case DIR_LEFT:
		heroTransform->position.x -= 1;
		break;
	case DIR_UP:
		heroTransform->position.y -= 1;
		break;
	case DIR_DOWN:
		heroTransform->position.y += 1;
		break;
	default:
		Report::Warning("Invalid direction", __LINE__, __FILE__);
		return;
	}

	if (heroTransform->position.y < 0)
	{
		heroTransform->position.y += 1;
	}
	if (heroTransform->position.x < 0)
	{
		heroTransform->position.x += 1;
	}
	// Validate position
}

void Floyd::ScriptHero_OnKeyPressed(Entity *owner, char key)
{
	TransformComponent *heroTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);

	switch (key)
	{
	case KEY_UP:
		Move(DIR_UP, heroTransform);
		break;
	case KEY_LEFT:
		Move(DIR_LEFT, heroTransform);
		break;
	case KEY_DOWN:
		Move(DIR_DOWN, heroTransform);
		break;
	case KEY_RIGHT:
		Move(DIR_RIGHT, heroTransform);
		break;
	default:
		break;
	}
}