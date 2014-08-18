#ifndef FLOYD_TRANSFORM_COMPONENT_H
#define FLOYD_TRANSFORM_COMPONENT_H


#include "Component.h"
#include "../Floyd_Geometry/Vector.h"


class TransformComponent : public IComponent
{
public:
	Position position;
	Position prevPosition;
	Position direction;

	explicit TransformComponent();
	TransformComponent(const Position &newPosition, const Position &newPrevPosition, const Position &newDirection);

public:
	void GoToPrevPos();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif