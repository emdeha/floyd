#include "Entity.h"

#include <algorithm>
#include <iostream>
#include <fstream>


IComponent* CreateComponentFromType(ComponentType cType)
{
	switch (cType)
	{
	case CTYPE_STAT:
		return new StatComponent();
	case CTYPE_PARTICLE_EMITTER:
		return new ParticleEmitterComponent();
	case CTYPE_TRANSFORM:
		return new TransformComponent();
	case CTYPE_OWNABLE:
		return new OwnableComponent();
	case CTYPE_CONTROLLABLE:
		return new ControllableComponent();
	case CTYPE_AI:
		return new AIComponent();
	case CTYPE_COLLIDABLE:
		return new CollidableComponent();
	case CTYPE_INVENTORY:
		return new InventoryComponent();
	case CTYPE_DRAWABLE:
		return new DrawableComponent();
	default:
		std::cerr << "Error: Invalid component type\n";
		return nullptr;
	}
}


int Entity::id = 0;

Entity::Entity()
	: components(0)
{
	++id;
}

IComponent* Entity::GetComponent(ComponentType cType) const
{
	auto foundComponent = std::find_if(components.begin(), components.end(),
									   [&cType](const std::shared_ptr<IComponent> component)
									   {
									       return component->cType == cType;
									   });

	if (foundComponent != components.end())
	{
		return foundComponent->get();
	}
	else
	{
		return nullptr;
	}
}

void Entity::AddComponent(std::shared_ptr<IComponent> newComponent)
{
	newComponent->SetOwner(this);
	components.push_back(newComponent);
}

int Entity::GetID() const
{
	return id;
}

void Entity::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		size_t componentsSize = components.size();
		saveStream.write((char*)&componentsSize, sizeof(size_t));
		for (auto component = components.begin(); component != components.end(); ++component)
		{
			saveStream.write((char*)&(*component)->cType, sizeof(ComponentType)); // TODO: duplicates cType
			(*component)->Serialize(saveStream);
		}
	}
	else
	{
		std::cerr << "Error: Cannot serialize entity with ID: " << id << "\n";
	}
}

void Entity::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		size_t componentsSize = 0;
		loadStream.read((char*)&componentsSize, sizeof(size_t));
		for (size_t idx = 0; idx < componentsSize; ++idx)
		{
			ComponentType cType = CTYPE_INVALID;
			loadStream.read((char*)&cType, sizeof(ComponentType));
			
			IComponent *newComponent = CreateComponentFromType(cType);
			newComponent->Deserialize(loadStream);
		}
	}
	else
	{
		std::cerr << "Error: Cannot deserialize entity with ID: " << id << "\n";
	}
}