#include "stdafx.h"
#include "Component.h"

#include "Entity.h"
#include "../Floyd_General/Utils.h"
#include "../Floyd_General/Reporting.h"

#include <fstream>
#include <sstream>


IComponent::IComponent(ComponentType newCType)
	: cType(newCType), group("")
{
}

IComponent::~IComponent()
{
	// delete owner; // We shouldn't do this. The owner is responsible for his deletion. Maybe a shared_ptr
					 // would be better.
}

void IComponent::SetOwner(Entity *newOwner)
{
	owner = newOwner;
}

void IComponent::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		size_t groupLen = group.length();
		saveStream.write((char*)&groupLen, sizeof(groupLen));
		saveStream.write(&group[0], groupLen);
		DoSerialization(saveStream);
	}
	else
	{
		std::stringstream msg("Error: serializing component: ");
		msg << cType;
		Report::Error(msg.str(), __LINE__, __FILE__);
	}
}
void IComponent::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		size_t groupLen = 0;
		loadStream.read((char*)&groupLen, sizeof(groupLen));
		group.resize(groupLen);
		loadStream.read(&group[0], groupLen);
		DoDeserialization(loadStream);
	}
	else
	{
		std::stringstream msg("Error: deserializing component: ");
		msg << cType;
		Report::Error(msg.str(), __LINE__, __FILE__);
	}
}