#ifndef FLOYD_CONTROLLABLE_COMPONENT_H
#define FLOYD_CONTROLLABLE_COMPONENT_H


#include "Component.h"


class World;

class ControllableComponent : public IComponent
{
public:
	typedef void (*OnKeyPressedScript)(World*, Entity*, char);

	void SetOnKeyPressed(OnKeyPressedScript newOnKeyPressed, const std::string &scriptGroup);
	void CallOnKeyPressed(World *world, char key);

	explicit ControllableComponent();

private:
	OnKeyPressedScript onKeyPressed;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif