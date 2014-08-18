#include "stdafx.h"
#include "DrawableComponent.h"

#include <fstream>


DrawableComponent::DrawableComponent()
	: sprite(),
	  IComponent(CTYPE_DRAWABLE)
{
}

void DrawableComponent::DoSerialization(std::ofstream &saveStream) const
{
	sprite.Serialize(saveStream);
}
void DrawableComponent::DoDeserialization(std::ifstream &loadStream)
{
	sprite.Deserialize(loadStream);
}