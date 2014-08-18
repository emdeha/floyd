#ifndef FLOYD_COLLIDABLE_COMPONENT_H
#define FLOYD_COLLIDABLE_COMPONENT_H


#include "Component.h"


enum InfoType
{
	INFOTYPE_LOGICAL_SPRITE,
	INFOTYPE_SPRITE,

	INFOTYPE_COUNT,

	INFOTYPE_INVALID = -1
};

class World;
struct Tile;

class CollidableComponent : public IComponent
{
public:
	char collisionInfo[INFOTYPE_COUNT];

	typedef void (*OnCollision)(World*, Entity*, const Tile*);

	void SetOnCollision(OnCollision newOnCollision, const std::string &scriptGroup);
	void CallOnCollision(World *world, const Tile *collider);

	explicit CollidableComponent();

private:
	OnCollision onCollision;

	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif