#include "stdafx.h"
#include "QuestInfoComponent.h"

#include <fstream>


QuestInfoComponent::QuestInfoComponent()
	: hasTalkedToNPC(false),
	  IComponent(CTYPE_QUEST_INFO)
{
}
QuestInfoComponent::QuestInfoComponent(bool newHasTalkedToNPC)
	: hasTalkedToNPC(newHasTalkedToNPC),
	  IComponent(CTYPE_QUEST_INFO)
{
}

void QuestInfoComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write((char*)&hasTalkedToNPC, sizeof(hasTalkedToNPC));
}
void QuestInfoComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&hasTalkedToNPC, sizeof(hasTalkedToNPC));
}