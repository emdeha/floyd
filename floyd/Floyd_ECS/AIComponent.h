#ifndef FLOYD_AI_COMPONENT_H
#define FLOYD_AI_COMPONENT_H


#include "Component.h"


enum AIType
{
	AITYPE_BOSS,
	AITYPE_PARTICLE,
	AITYPE_MONSTER,

	AITYPE_INVALID = -1
};


class World;

class AIComponent : public IComponent
{
public:
	AIType aiType;

	typedef void (*OnUpdateAIScript)(World*, Entity*);

	void SetOnUpdateAI(OnUpdateAIScript newOnUpdateAI, const std::string &scriptGroup);
	void CallOnUpdateAI(World *world);

	explicit AIComponent();

private:
	OnUpdateAIScript onUpdateAI;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif