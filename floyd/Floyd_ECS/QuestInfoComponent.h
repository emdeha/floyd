#ifndef FLOYD_QUEST_INFO_COMPONENT_H
#define FLOYD_QUEST_INFO_COMPONENT_H


#include "Component.h"


class QuestInfoComponent : public IComponent
{
public:
	bool hasTalkedToNPC;

	explicit QuestInfoComponent();
	QuestInfoComponent(bool newHasTalkedToNPC);

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif