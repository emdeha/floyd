#include "stdafx.h"
#include "ControllableComponent.h"

#include "../Floyd_Scripts/ScriptDispatcher.h"


ControllableComponent::ControllableComponent()
	: IComponent(CTYPE_CONTROLLABLE)
{
}

void ControllableComponent::SetOnKeyPressed(OnKeyPressedScript newOnKeyPressed, const std::string &scriptGroup)
{
	onKeyPressed = newOnKeyPressed;
	group = scriptGroup;
}
void ControllableComponent::CallOnKeyPressed(World *world, char key)
{
	if (onKeyPressed)
	{
		onKeyPressed(world, owner, key);
	}
}

void ControllableComponent::DoSerialization(std::ofstream &saveStream) const
{
}
void ControllableComponent::DoDeserialization(std::ifstream &loadStream)
{
	onKeyPressed = Floyd::GetOnKeyPressed(group);
}