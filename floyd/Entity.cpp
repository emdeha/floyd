#include "Entity.h"

#include <algorithm>


Entity::Entity()
	: components(0)
{
}
Entity::~Entity()
{
	for (auto component = components.begin(); component != components.end(); ++component)
	{
		delete (*component);
	}
	components.clear();
}

IComponent* Entity::GetComponent(ComponentType cType) const
{
	auto foundComponent = std::find_if(components.begin(), components.end(),
									   [&cType](const IComponent *component)
									   {
									       return component->cType == cType;
									   });

	if (foundComponent != components.end())
	{
		return *foundComponent;
	}
	else
	{
		return nullptr;
	}
}

void Entity::AddComponent(IComponent &newComponent)
{
	components.push_back(newComponent.Copy());
}