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

	static const char* levelDir;

public:
	Level() : name(""), map(0) {};

	void Init(const std::string &levelFile);

private:
	Level(const Level &other);
	Level& operator=(const Level &other);
};


#endif