#ifndef FLOYD_COMPONENT_H
#define FLOYD_COMPONENT_H


#include <string>


enum ComponentType
{
	CTYPE_STAT,
	CTYPE_PARTICLE_EMITTER,
	CTYPE_PARTICLE,
	CTYPE_TRANSFORM,
	CTYPE_OWNABLE,
	CTYPE_CONTROLLABLE,
	CTYPE_AI,
	CTYPE_COLLIDABLE,
	CTYPE_INVENTORY,
	CTYPE_QUEST_INFO,
	CTYPE_DRAWABLE,
	CTYPE_ANIMATED,

	CTYPE_INVALID = -1
};

class Entity;

class IComponent
{
private:
	virtual void DoSerialization(std::ofstream &saveStream) const = 0;
	virtual void DoDeserialization(std::ifstream &loadStream) = 0;

public:
	ComponentType cType;
	std::string group; /// @property group - used to chose the correct script
					   ///			 for scriptable components.
	Entity *owner;

public:
	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
	
	///
	/// @brief Sets the owner of the component.
	///
	void SetOwner(Entity *newOwner);

public:
	IComponent(ComponentType newCType);
	virtual ~IComponent();
};


#endif