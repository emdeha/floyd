#ifndef FLOYD_ANIMATED_COMPONENT_H
#define FLOYD_ANIMATED_COMPONENT_H


#include "Component.h"
#include "../Floyd_Geometry/Vector.h"

#include <vector>


class AnimatedComponent : public IComponent
{
private:
	std::vector<Position> animPoints;
	size_t currentAnimPointIdx;

public:
	void AddAnimPoint(const Position &animPoint);
	void UpdateAnim();
	Position GetCurrentAnimPos() const;

	explicit AnimatedComponent();

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif