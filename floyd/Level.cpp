#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "World.h"
#include "Level.h"
#include "Dirs.h"


void Level::Init(const std::string &levelFile)
{
	name = levelFile;
	std::ifstream level(worldDir + levelFile);

	if (level.is_open())
	{
		std::string line;
		while (std::getline(level, line))
		{
			map.push_back(line);
			std::cout << line << std::endl;
		}
	}
	else
	{
		std::cerr << "Error: Opening level file!\n";
		return;
	}

	level.close();
}

void Level::InitCutscenes(const std::vector<std::string> &cutsceneFileNames)
{
	for (auto iter = cutsceneFileNames.begin(); iter != cutsceneFileNames.end(); ++iter)
	{
		if (iter->find('e') != iter->npos)
		{
			std::cout << "Loading endscene: " << (*iter) << std::endl;
			AddEndscene(*iter);
		}
		else if (iter->find('c') != iter->npos)
		{
			std::cout << "Loading cutscene: " << (*iter) << std::endl;
			AddCutscene(*iter);
		}
		else if ((*iter) == "")
		{
			continue;
		}
		else
		{
			std::cerr << "Invalid token for cutscene!\n";
			return;
		}
	}
}

void Level::Display() const
{
	//int sleep_secs = 5;
	//std::this_thread::sleep_for(std::chrono::milliseconds(sleep_secs * 1000));
	system("CLS");
	for (auto mapLine = map.begin(); mapLine != map.end(); ++mapLine)
	{
		std::cout << (*mapLine) << std::endl;
	}

	//int sleep_secs = 1;
	//for (auto mapLine = map.begin(); mapLine != map.end(); ++mapLine)
	//{
	//	std::cout << (*mapLine) << std::endl;
	//}
	//std::this_thread::sleep_for(std::chrono::milliseconds(sleep_secs * 1000));
	//system("CLS");
	//for (auto cutLine = cutscene.begin(); cutLine != cutscene.end(); ++cutLine)
	//{
	//	std::cout << (*cutLine) << std::endl;
	//}
	//std::this_thread::sleep_for(std::chrono::milliseconds(sleep_secs * 1000));
	//system("CLS");
	//for (auto endLine = endscene.begin(); endLine != endscene.end(); ++endLine)
	//{
	//	std::cout << (*endLine) << std::endl;
	//}
	//std::this_thread::sleep_for(std::chrono::milliseconds(sleep_secs * 1000));
	//system("CLS");
}

void Level::UpdateLevelMatrix(const World *world)
{
	Position heroPrevPos = world->GetPlayerPrevPos();
	map[heroPrevPos.y][heroPrevPos.x] = prevCharacter;

	Position heroPos = world->GetPlayerPos();
	prevCharacter = map[heroPos.y][heroPos.x]; 
	if (prevCharacter == 'S')
	{
		prevCharacter = ' ';
	}
	map[heroPos.y][heroPos.x] = '|';
}

Position Level::GetStartingPos() const
{
	for (size_t y = 0; y < map.size(); ++y)
	{
		for (size_t x = 0; x < map[y].size(); ++x)
		{
			// TODO: Put special characters in constants
			if (map[y][x] == 'S')
			{
				return Position(x, y);	
			}
		}
	}

	std::cerr << "Warning: No starting position defined!\n";
	return Position(1, 1);
}

///////////////////////
//  Private methods  //
///////////////////////

void Level::AddCutscene(const std::string &cutsceneFile)
{
	// conflict with member	`cutscene`
	std::ifstream _cutscene(worldDir + cutsceneFile);

	if (_cutscene.is_open())
	{
		std::string line;
		while (std::getline(_cutscene, line))
		{
			cutscene.push_back(line);	
			std::cout << line << std::endl;
		}
	}
	else
	{
		std::cerr << "Error: Opening cutscene file!\n";
		return;
	}

	_cutscene.close();
}

void Level::AddEndscene(const std::string &endsceneFile)
{
	// conflict with member	`endscene`
	std::ifstream _endscene(worldDir + endsceneFile);

	if (_endscene.is_open())
	{
		std::string line;
		while (std::getline(_endscene, line))
		{
			endscene.push_back(line);	
			std::cout << line << std::endl;
		}
	}
	else
	{
		std::cerr << "Error: Opening cutscene file!\n";
		return;
	}

	_endscene.close();
}