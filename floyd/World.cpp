#include <iostream>
#include <fstream>

#include "World.h"
#include "Dirs.h"
#include "Wrapper.h"


std::string GetLevelName(const std::string &level)
{
	// TODO: Trim all whitespaces
	
	// We assume that the level is the last number in the string
	size_t levelTokenPos = level.find_last_of(':');
	std::string levelName = level.substr(levelTokenPos + 1);
	return levelName;
}

std::vector<std::string> GetLevelArrayOfCutscenes(const std::string &level)
{
	// TODO: Trim all whitespaces

	// We assume that the cutscene begins with 'c' and ends with a number
	// We also add the cutscene extention		
	std::vector<std::string> result;

	if (level.find('c') == level.npos)
	{
		result.push_back("");
	}
	else
	{
		size_t currentCutsceneEndPos = level.find_first_of(':');
		size_t currentCutsceneStartPos = 0;
		std::string currentCutscene = level.substr(currentCutsceneStartPos, currentCutsceneEndPos);
		result.push_back(currentCutscene + levelExt);
		while (level.find(":", currentCutsceneEndPos + 1) != level.npos)
		{
			currentCutsceneStartPos = currentCutsceneEndPos + 1;
			currentCutsceneEndPos = level.find_first_of(":", currentCutsceneStartPos);
			currentCutscene = level.substr(currentCutsceneStartPos, currentCutsceneEndPos - currentCutsceneStartPos);
			result.push_back(currentCutscene + levelExt);
		}
	}
	return result;
}

void World::Init(const std::string &worldFile)
{
	std::ifstream world(worldDir + worldFile);
	
	if (world.is_open())
	{
		std::string levelDef;
		while (std::getline(world, levelDef, ';').good())
		{
			std::cout << "Loding level: " << levelDef << std::endl;
			Level newLevel;
			std::string levelName = GetLevelName(levelDef); 
			newLevel.Init(levelName + levelExt);
			std::vector<std::string> cutscenes = GetLevelArrayOfCutscenes(levelDef);
			newLevel.InitCutscenes(cutscenes);
			levels.push_back(newLevel);
		}
	}
	else
	{
		std::cerr << "Error: Opening world file\n";
		return;
	}

	Position startingPos = levels[currentLevelIdx].GetStartingPos();
	hero.SetInitialPosition(startingPos);
	world.close();
}

void World::Display() const
{
	std::cout << "Level: " << currentLevelIdx << std::endl;
	levels[currentLevelIdx].Display();
}

void World::PollInput()
{
	//std::cin >> dir; 	
	if (_kbhit())
	{
		char dir = _kbhit() ? dir = static_cast<char>(_getch()) : '`';

		switch (dir)
		{
		case 'w':
			hero.Move(DIR_UP);
			break;
		case 'a':
			hero.Move(DIR_LEFT);
			break;
		case 's':
			hero.Move(DIR_DOWN);
			break;
		case 'd':
			hero.Move(DIR_RIGHT);
			break;
		case 'q':
			std::cout << "QUIT\n";
			exit(1);
			break;
		default:
			break;
		}
	}
}

void World::Update() 
{
	UpdateCollisions();
	levels[currentLevelIdx].UpdateLevelMatrix(this);
}

Position World::GetPlayerPos() const
{
	return hero.GetPosition(); 
}
Position World::GetPlayerPrevPos() const
{
	return hero.GetPrevPos();
}

void World::UpdateCollisions()
{
	LevelMatrix currentMap = levels[currentLevelIdx].GetMap();
	Position currentHeroPos = hero.GetPosition();
	char currentTile = currentMap[currentHeroPos.y][currentHeroPos.x]; 
	switch (currentTile)
	{
	case '#':
		hero.GoToPrevPos();
		break;
	case 'E':
		currentLevelIdx++;
		Position startingPos = levels[currentLevelIdx].GetStartingPos();
		hero.SetInitialPosition(startingPos);
		break;
	}
}