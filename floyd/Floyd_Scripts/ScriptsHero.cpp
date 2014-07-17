#include "stdafx.h"
#include "ScriptsHero.h"

#include "../Entity.h"
#include "../Reporting.h"


void Move(Direction dir, MovableComponent *heroMovable)
{
	heroMovable->prevPosition = heroMovable->position;
	
	switch (dir)
	{
	case DIR_RIGHT:
		heroMovable->position.x += 1;
		break;
	case DIR_LEFT:
		heroMovable->position.x -= 1;
		break;
	case DIR_UP:
		heroMovable->position.y -= 1;
		break;
	case DIR_DOWN:
		heroMovable->position.y += 1;
		break;
	default:
		Report::Warning("Invalid direction", __LINE__, __FILE__);
		return;
	}

	if (heroMovable->position.y < 0)
	{
		heroMovable->position.y += 1;
	}
	if (heroMovable->position.x < 0)
	{
		heroMovable->position.x += 1;
	}
	// Validate position
}

void Floyd::ScriptHero_OnKeyPressed(Entity *owner, char key)
{
	MovableComponent *heroMovable = owner->GetComponentDirectly<MovableComponent>(CTYPE_MOVABLE);

	switch (key)
	{
	case KEY_UP:
		Move(DIR_UP, heroMovable);
		break;
	case KEY_LEFT:
		Move(DIR_LEFT, heroMovable);
		break;
	case KEY_DOWN:
		Move(DIR_DOWN, heroMovable);
		break;
	case KEY_RIGHT:
		Move(DIR_RIGHT, heroMovable);
		break;
	default:
		break;
	}
}