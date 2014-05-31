#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

// TODO: Make portable
#include <Windows.h>

#include "World.h"
#include "Level.h"
#include "Dirs.h"


const int DIM_RIGHT = 80;
const int DIM_BOTTOM = 25;


void Level::Init(const std::string &levelFile)
{
	name = levelFile;
	std::ifstream level(worldDir + levelFile);

	size_t currYPos = 0;
	if (level.is_open())
	{
		std::string line;
		while (std::getline(level, line))
		{
			map.push_back(line);
			size_t exitBlockX = line.find('@');
			if (exitBlockX != line.npos)
			{
				exitBlockPos = Position(exitBlockX, currYPos);
			}
			size_t teleportX = line.find('T');
			if (teleportX != line.npos)
			{
				teleportPos = Position(teleportX, currYPos);
			}
			GetSpawnPositionsFromLine(line, currYPos);	
			currYPos++;
		}
	}
	else
	{
		std::cerr << "Error: Opening level file!\n";
		return;
	}

	if (teleportPos.x >= 0 && teleportPos.y >= 0 &&
		exitBlockPos.x >= 0 && exitBlockPos.y >= 0)
	{
		SetTileAtPosition(teleportPos, ' ');
		SetTileAtPosition(exitBlockPos, '#');
	}

	for (auto spawnPos = monsterSpawnPoints.begin(); spawnPos != monsterSpawnPoints.end();
		++spawnPos)
	{
		SetTileAtPosition((*spawnPos), '#');
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
	//prevCharacter = ' ';
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

void Level::Display()
{
	BeginSwapBuffers();

	if (isShowingEndscene)
	{
		for (auto sceneLine = endscene.begin(); sceneLine != endscene.end(); ++sceneLine)
		{
			std::cout << (*sceneLine) << std::endl;
		}
	}
	else if (isShowingNPCscene)
	{
		for (auto sceneLine = npcscene.begin(); sceneLine != npcscene.end(); ++sceneLine)
		{
			std::cout << (*sceneLine) << std::endl;
		}
		
		if (GetTimeSinceEpoch() - lastNpcSceneInterval_s > npcSceneDuration_s)
		{
			isShowingNPCscene = false;
			lastNpcSceneInterval_s = GetTimeSinceEpoch();
		}
	}
	else if (!hasBegan)
	{
		for (auto sceneLine = cutscene.begin(); sceneLine != cutscene.end(); ++sceneLine)
		{
			std::cout << (*sceneLine) << std::endl;
		}

		if (GetTimeSinceEpoch() - lastCutsceneInterval_s > cutsceneDuration_s)
		{
			hasBegan = true;
			lastCutsceneInterval_s = GetTimeSinceEpoch();
		}
	}
	else
	{
		for (auto mapLine = map.begin(); mapLine != map.end(); ++mapLine)
		{
			std::cout << (*mapLine) << std::endl;
		}
	}

	EndSwapBuffers();
}

void Level::UpdateLevelMatrix(World *world)
{
	if (hasBegan)
	{
		Position heroPos = world->GetPlayerPos();
		if (!lastFrameHeroPos.IsEqual(heroPos))
		{
			Position heroPrevPos = world->GetPlayerPrevPos();
			map[heroPrevPos.y][heroPrevPos.x] = world->GetHero().GetPrevTile(); 
			world->GetHero().SetPrevTile(map[heroPos.y][heroPos.x]);
			map[heroPos.y][heroPos.x] = '|';
		}
		lastFrameHeroPos = heroPos;

		auto &monsters = world->GetMonsters();
		for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
		{
			Position monsterPrevPos = monster->GetPrevPos();
			map[monsterPrevPos.y][monsterPrevPos.x] = monster->GetPrevTile();
			Position monsterPos = monster->GetPosition();
			monster->SetPrevTile(map[monsterPos.y][monsterPos.x]);
			map[monsterPos.y][monsterPos.x] = 'M';
		}

		auto &particles = world->GetParticles();
		for (auto particle = particles.begin(); particle != particles.end(); ++particle)
		{
			Position particlePrevPos = particle->GetPrevPos();
			map[particlePrevPos.y][particlePrevPos.x] = particle->GetPrevTile();
			Position particlePos = particle->GetPosition();
			particle->SetPrevTile(map[particlePos.y][particlePos.x]);
			map[particlePos.y][particlePos.x] = '.';
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

void Level::SpawnMonsters(World *world)
{
	for (auto spawnPoint = monsterSpawnPoints.begin(); spawnPoint != monsterSpawnPoints.end();
		++spawnPoint)
	{
		world->SpawnMonsterAtPos((*spawnPoint));	
	}
}

void Level::UnblockExit()
{
	SetTileAtPosition(exitBlockPos, ' ');
}

void Level::ShowTeleport()
{
	SetTileAtPosition(teleportPos, 'T');
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
			//std::cout << line << std::endl;
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
			//std::cout << line << std::endl;
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
			//std::cout << line << std::endl;
		}
	}
	else
	{
		std::cerr << "Error: Opening cutscene file!\n";
		return;
	}

	_npcscene.close();
}

void Level::BeginSwapBuffers() const
{
	HANDLE tempBuf = drawBuffer;
	drawBuffer = setBuffer;
	setBuffer = tempBuf;

	if (!SetStdHandle(STD_OUTPUT_HANDLE, drawBuffer))
	{
		std::cerr << "SetStdHandle failed - " << GetLastError() << std::endl;
		return;
	}

	ClearHandleScreen(drawBuffer);
}

void Level::EndSwapBuffers() const
{
	SMALL_RECT srctReadRect;
	srctReadRect.Top = 0;
	srctReadRect.Left = 0;
	srctReadRect.Right = DIM_RIGHT - 1;
	srctReadRect.Bottom = DIM_BOTTOM - 1;

	COORD coordBufSize;
	coordBufSize.X = DIM_RIGHT;
	coordBufSize.Y = DIM_BOTTOM;

	COORD coordBufTopLeft;
	coordBufTopLeft.X = 0;
	coordBufTopLeft.Y = 0;

	CHAR_INFO currentOutput[DIM_RIGHT * DIM_BOTTOM];
	BOOL fSuccess = ReadConsoleOutput(
		drawBuffer, currentOutput, coordBufSize, coordBufTopLeft, &srctReadRect);
	if (!fSuccess)
	{
		std::cerr << "ReadConsoleOutput failed - " << GetLastError() << std::endl;
		return;
	}

	fSuccess = WriteConsoleOutput(
		setBuffer, currentOutput, coordBufSize, coordBufTopLeft, &srctReadRect);
	if (!fSuccess)
	{
		std::cerr << "WriteConsoleOutput failed - " << GetLastError() << std::endl;
		return;
	}

	// Swap buffers
	if (!SetConsoleActiveScreenBuffer(drawBuffer))
	{
		std::cerr << "SetConsoleActiveScreenBuffer failed - " << GetLastError() << std::endl;
		return;
	}
}

void Level::GetSpawnPositionsFromLine(const std::string &line, int preferredY)
{
	size_t firstPosX = line.find('m');
	while (firstPosX != line.npos)
	{
		monsterSpawnPoints.push_back(Position(firstPosX, preferredY));
		firstPosX = line.find('m', firstPosX);
	}
}