#ifndef FLOYD_ISERIALIZABLE_H
#define FLOYD_ISERIALIZABLE_H


#include <sstream>
#include <iostream>


class ISerializable
{
protected:
	virtual void Serialize() const = 0;
	virtual void Deserialize() const = 0;
};


#endif