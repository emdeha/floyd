#ifndef FLOYD_ISERIALIZABLE_H
#define FLOYD_ISERIALIZABLE_H


#include <sstream>
#include <iostream>


class ISerializable
{
protected:
	virtual void Serialize(const std::stringstream &dataStream) const
	{
		std::cout << "Serializing: " << dataStream.str() << std::endl;
	}
	virtual void Deserialize(std::stringstream &dataStream) const
	{
		std::cout << "Deserializing: " << dataStream.str() << std::endl;
	}
};


#endif