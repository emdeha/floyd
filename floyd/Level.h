#ifndef FLOYD_LEVEL_H
#define FLOYD_LEVEL_H


#include <string>
#include <vector>

#include "ISerializable.h"


class Level : private ISerializable
{
private:
	std::string name;

	typedef std::vector<std::string> LevelMatrix;
	LevelMatrix map;
	// Currently supporting one cutscene per level
	LevelMatrix cutscene;
	LevelMatrix endscene;

public:
	Level() : name(""), map(0), cutscene(0), endscene(0) {};

	void Init(const std::string &levelFile);
	void InitCutscenes(const std::vector<std::string> &cutsceneFileNames);

	void Display() const;

private:
	// TODO: Clear duplicate code
	void AddCutscene(const std::string &cutsceneFile);
	void AddEndscene(const std::string &endsceneFile);
};


#endif