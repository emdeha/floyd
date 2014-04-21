#ifndef FLOYD_LEVEL_H
#define FLOYD_LEVEL_H


#include <string>
#include <vector>

#include "ISerializable.h"


class Level : private ISerializable
{
private:
	std::string name;
	std::vector<std::string> map;

public:
	Level() : name(""), map(0) {};

	void Init(const std::string &levelFile);
};


#endif