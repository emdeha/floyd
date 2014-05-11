#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

// TODO: Make portable
#include <Windows.h>

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
		}
	}
	else
	{
		std::cerr << "Error: Opening level file!\n";
		return;
	}

	// TODO: We assume that the level always starts at some offset from the console's
	//		 borders.
	//Position startPos = GetStartingPos();
	//if (startPos.x == -1 && startPos.y == -1)
	//{
	//	prevCharacter = ' ';
	//}
	//else
	//{
	//	prevCharacter = map[startPos.y][startPos.x];
	//}
	prevCharacter = ' ';
	level.close();
}

void Level::InitCutscenes(const std::vector<std::string> &cutsceneFileNames)
{
	for (auto iter = cutsceneFileNames.begin(); iter != cutsceneFileNames.end(); ++iter)
	{
		if (iter->find('e') != iter->npos)
		{
			//std::cout << "Loading endscene: " << (*iter) << std::endl;
			AddEndscene(*iter);
		}
		else if (iter->find('c') != iter->npos)
		{
			//std::cout << "Loading cutscene: " << (*iter) << std::endl;
			AddCutscene(*iter);
		}
		else if (iter->find('n') != iter->npos)
		{
			AddNPCscene(*iter);
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
	//HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//SMALL_RECT srcWriteRect;
	//srcWriteRect.Top = 0;
	//srcWriteRect.Left = 0;
	//srcWriteRect.Bottom = 24;
	//srcWriteRect.Right = 79;

	//COORD coordBufSize;
	//coordBufSize.Y = 25;
	//coordBufSize.X = 80;
	//COORD coordBufCoord;
	//coordBufCoord.X = 0;
	//coordBufCoord.Y = 0;

	//size_t lvlHeight = map.size();
	//size_t lvlWidth = map[0].size();
	//CHAR_INFO levelBuf[2000]; // [25][80]
	//for (size_t line = 0; line < lvlHeight; ++line)
	//{
	//	for (size_t ch = 0; ch < lvlWidth; ++ch)
	//	{
	//		levelBuf[lvlHeight * line + ch].Char.AsciiChar = ':';//map[line][ch];
	//	}
	//}

	//BOOL success = WriteConsoleOutput(
	//				hStdOut,
	//				levelBuf,
	//				coordBufSize,
	//				coordBufCoord,
	//				&srcWriteRect);

	//if (!success)
	//{
	//	std::cout << "WriteConsoleOutputFailed - " << GetLastError() << std::endl;
	//	return;
	//}

	//int sleep_secs = 5;
	//std::this_thread::sleep_for(std::chrono::milliseconds(sleep_secs * 1000));

	if (isShowingEndscene)
	{
		system("CLS");
		for (auto sceneLine = endscene.begin(); sceneLine != endscene.end(); ++sceneLine)
		{
			std::cout << (*sceneLine) << std::endl;
		}
	}
	else if (isShowingNPCscene)
	{
		system("CLS");
		for (auto sceneLine = npcscene.begin(); sceneLine != npcscene.end(); ++sceneLine)
		{
			std::cout << (*sceneLine) << std::endl;
		}
		isShowingNPCscene = false;

		time_t sleep_ms = npcSceneDuration_s * 1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	}
	else if (!hasBegan)
	{
		system("CLS");
		for (auto sceneLine = cutscene.begin(); sceneLine != cutscene.end(); ++sceneLine)
		{
			std::cout << (*sceneLine) << std::endl;
		}
		hasBegan = true;

		time_t sleep_ms = cutsceneDuration_s * 1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	}
	else
	{
		system("CLS");
		for (auto mapLine = map.begin(); mapLine != map.end(); ++mapLine)
		{
			std::cout << (*mapLine) << std::endl;
		}
	}
}

void Level::UpdateLevelMatrix(const World *world)
{
	if (hasBegan)
	{
		Position heroPos = world->GetPlayerPos();
		if (!lastFrameHeroPos.IsEqual(heroPos))
		{
			Position heroPrevPos = world->GetPlayerPrevPos();
			map[heroPrevPos.y][heroPrevPos.x] = prevCharacter; 
			prevCharacter = map[heroPos.y][heroPos.x]; 
			map[heroPos.y][heroPos.x] = '|';
		}
		lastFrameHeroPos = heroPos;

		auto monsters = world->GetMonsters();
		for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
		{
			Position monsterPos = monster->GetPosition();
			map[monsterPos.y][monsterPos.x] = 'M';
			
			Position monsterPrevPos = monster->GetPrevPos();
			map[monsterPrevPos.y][monsterPrevPos.x] = ' ';
		}

		auto particles = world->GetParticles();
		for (auto particle = particles.begin(); particle != particles.end(); ++particle)
		{
			Position particlePos = particle->GetPosition();
			map[particlePos.y][particlePos.x] = '.';

			Position particlePrevPos = particle->GetPrevPos();
			map[particlePrevPos.y][particlePrevPos.x] = ' ';
		}
	}
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
	return Position(-1, -1);
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

void Level::AddNPCscene(const std::string &npcsceneFile)
{
	// conflict with member	`npcscene`
	std::ifstream _npcscene(worldDir + npcsceneFile);

	if (_npcscene.is_open())
	{
		std::string line;
		while (std::getline(_npcscene, line))
		{
			npcscene.push_back(line);	
			std::cout << line << std::endl;
		}
	}
	else
	{
		std::cerr << "Error: Opening cutscene file!\n";
		return;
	}

	_npcscene.close();
}