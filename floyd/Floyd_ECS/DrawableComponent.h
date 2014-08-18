#ifndef FLOYD_DRAWABLE_COMPONENT_H
#define FLOYD_DRAWABLE_COMPONENT_H


#include "Component.h"
#include "../Floyd_Graphics/Sprite.h"


class DrawableComponent : public IComponent
{
public:
	Sprite sprite;

	explicit DrawableComponent();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif