#include "stdafx.h"
#include "AIComponent.h"

#include "../Floyd_Scripts/ScriptDispatcher.h"

#include <fstream>


AIComponent::AIComponent()
	: IComponent(CTYPE_AI)
{
}

void AIComponent::SetOnUpdateAI(OnUpdateAIScript newOnUpdateAI, const std::string &scriptGroup)
{
	onUpdateAI = newOnUpdateAI;
	group = scriptGroup;
}
void AIComponent::CallOnUpdateAI(World *world)
{
	if (onUpdateAI)
	{
		onUpdateAI(world, owner);
	}
}

void AIComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write((char*)&aiType, sizeof(aiType));
}
void AIComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&aiType, sizeof(aiType));

	onUpdateAI = Floyd::GetOnUpdateAI(group);
}
