#ifndef FLOYD_ENTITY_H
#define FLOYD_ENTITY_H

#include "Component.h"

#include <vector>


class Entity
{
private:
	static int id;
	std::vector<IComponent*> components;

public:
	Entity();
	~Entity();

	IComponent* GetComponent(ComponentType cType) const;

	// Not better than doing static cast myself...
	template <class T> 
	T* GetComponentDirectly(ComponentType cType) const;

	void AddComponent(IComponent &newComponent);

public:
	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
};

template <class T>
T* Entity::GetComponentDirectly(ComponentType cType) const
{
	return static_cast<T*>(GetComponent(cType));
}


#endif