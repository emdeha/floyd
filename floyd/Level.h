#ifndef FLOYD_LEVEL_H
#define FLOYD_LEVEL_H


#include <string>
#include <vector>

#include "Utils.h"
//#include "ISerializable.h"

class World;

typedef std::vector<std::string> LevelMatrix;

class Level //: private ISerializable
{
private:
	std::string name;

	LevelMatrix map;
	char prevCharacter; /// @brief Contains the character which was displayed before the hero stepped over it;
	// Currently supporting one cutscene per level
	LevelMatrix cutscene;
	LevelMatrix endscene;

	bool isShowingEndscene;
	mutable bool hasBegan;

public:
	Level() : name(""), map(0), cutscene(0), endscene(0), 
			  hasBegan(false), isShowingEndscene(false) {};

	void Init(const std::string &levelFile);
	void InitCutscenes(const std::vector<std::string> &cutsceneFileNames);

	void Display() const;
	void UpdateLevelMatrix(const World *world); 

	Position GetStartingPos() const;
	LevelMatrix GetMap() const
	{
		return map;
	}
	void ShowEndscene()
	{
		isShowingEndscene = true;
	}

private:
	// TODO: Clear duplicate code
	void AddCutscene(const std::string &cutsceneFile);
	void AddEndscene(const std::string &endsceneFile);
};


#endif