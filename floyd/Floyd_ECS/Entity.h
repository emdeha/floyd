#ifndef FLOYD_ENTITY_H
#define FLOYD_ENTITY_H

#include "Component.h"

#include <vector>
#include <memory>
#include <string>


class Entity
{
private:
	static int id;
	std::vector<std::shared_ptr<IComponent>> components;

public:
	Entity();

	IComponent* GetComponent(ComponentType cType) const;

	///
	/// @brief Gets the component and casts it to the correct type.
	///
	template <class T> 
	T* GetComponentDirectly(ComponentType cType) const;

	///
	/// @brief Sets the component's owner to this and adds it to components.
	///
	void AddComponent(std::shared_ptr<IComponent> newComponent);

	int GetID() const;

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