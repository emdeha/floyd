#include "stdafx.h"
#include "Entity.h"

#include "../Floyd_Scripts/ScriptDispatcher.h"

#include "AIComponent.h"
#include "AnimatedComponent.h"
#include "CollidableComponent.h"
#include "ControllableComponent.h"
#include "DrawableComponent.h"
#include "InventoryComponent.h"
#include "ParticleComponent.h"
#include "ParticleEmitterComponent.h"
#include "QuestInfoComponent.h"
#include "TransformComponent.h"
#include "StatComponent.h"

#include <algorithm>
#include <iostream>
#include <fstream>


std::shared_ptr<IComponent> CreateComponentFromType(ComponentType cType)
{
	switch (cType)
	{
	case CTYPE_STAT:
		return std::make_shared<StatComponent>();
	case CTYPE_PARTICLE_EMITTER:
		return std::make_shared<ParticleEmitterComponent>();
	case CTYPE_PARTICLE:
		return std::make_shared<ParticleComponent>();
	case CTYPE_TRANSFORM:
		return std::make_shared<TransformComponent>();
	case CTYPE_CONTROLLABLE:
		return std::make_shared<ControllableComponent>();
	case CTYPE_AI:
		return std::make_shared<AIComponent>();
	case CTYPE_COLLIDABLE:
		return std::make_shared<CollidableComponent>();
	case CTYPE_INVENTORY:
		return std::make_shared<InventoryComponent>();
	case CTYPE_DRAWABLE:
		return std::make_shared<DrawableComponent>();
	case CTYPE_ANIMATED:
		return std::make_shared<AnimatedComponent>();
	case CTYPE_QUEST_INFO:
		return std::make_shared<QuestInfoComponent>();
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
		saveStream.write((char*)&componentsSize, sizeof(componentsSize));
		for (auto component = components.begin(); component != components.end(); ++component)
		{ 
			saveStream.write((char*)&(*component)->cType, sizeof((*component)->cType));
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
		loadStream.read((char*)&componentsSize, sizeof(componentsSize));
		for (size_t idx = 0; idx < componentsSize; ++idx)
		{
			ComponentType cType = CTYPE_INVALID;
			loadStream.read((char*)&cType, sizeof(cType));
			
			std::shared_ptr<IComponent> newComponent = CreateComponentFromType(cType);
			newComponent->Deserialize(loadStream);
			AddComponent(newComponent);
		}
	}
	else
	{
		std::cerr << "Error: Cannot deserialize entity with ID: " << id << "\n";
	}
}