#include "stdafx.h"
#include "TransformComponent.h"

#include "../Floyd_General/Utils.h"

#include <fstream>


TransformComponent::TransformComponent()
	: position(), prevPosition(), direction(),
	  IComponent(CTYPE_TRANSFORM)
{
}
TransformComponent::TransformComponent(const Position &newPosition, const Position &newPrevPosition,
								       const Position &newDirection)
	: position(newPosition), prevPosition(newPrevPosition), direction(newDirection),
	  IComponent(CTYPE_TRANSFORM)
{
}

void TransformComponent::GoToPrevPos()
{
	position = prevPosition;
}

void TransformComponent::DoSerialization(std::ofstream &saveStream) const
{
	SerializePosition(position, saveStream);
	SerializePosition(prevPosition, saveStream);
	SerializePosition(direction, saveStream);
}
void TransformComponent::DoDeserialization(std::ifstream &loadStream)
{
	DeserializePosition(position, loadStream);
	DeserializePosition(prevPosition, loadStream);
	DeserializePosition(direction, loadStream);
}