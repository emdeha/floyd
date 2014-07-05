#ifndef FLOYD_ENTITY_H
#define FLOYD_ENTITY_H

#include "Component.h"

#include <vector>


class Entity
{
private:
	std::vector<IComponent*> components;

public:
	Entity();
	~Entity();

	IComponent* GetComponent(ComponentType cType) const;
	void AddComponent(IComponent &newComponent);

public:
	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
};


#endif