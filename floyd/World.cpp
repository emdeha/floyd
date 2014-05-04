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
	// We don't differentiate between endScenes ('e') and cutScenes ('c') 
	// and npcScenes ('n')
	// We also add the cutscene extention		
	std::vector<std::string> result;

	if (level.find('c') == level.npos && 
		level.find('e') == level.npos &&
		level.find('n') == level.npos)
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
			//std::cout << "Loding level: " << levelDef << std::endl;
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

	world.close();

	Position startingPos = levels[currentLevelIdx].GetStartingPos();
	hero.SetInitialPosition(startingPos);

	InitLevelObjects();
}

void World::Display() const
{
	//std::cout << "Level: " << currentLevelIdx << std::endl;
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
	for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
	{
		monster->Update();
	}
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

const std::vector<Monster>& World::GetMonsters() const
{
	return monsters;
}

///////////////////////
//  Private Methods  //
///////////////////////

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
	case 'T':
	case '*':
		levels[currentLevelIdx].ShowEndscene();
		break;
	case 'N':
		levels[currentLevelIdx].ShowNPCscene();
		hero.GoToPrevPos();
		break;
	case 'O':
		hero.GoToPrevPos();
		break;
	case 'M':
		// Send attack message?
		hero.GoToPrevPos();
		break;
	case 'E':
		currentLevelIdx++;
		Position startingPos = levels[currentLevelIdx].GetStartingPos();
		hero.SetInitialPosition(startingPos);
		monsters.clear();
		InitLevelObjects();
		return;
	}
	// TODO: So much violated DRY :/
	for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
	{
		Position monsterPos = monster->GetPosition();
		char monsterTile = currentMap[monsterPos.y][monsterPos.x];
		switch (monsterTile)
		{
		case '#':
			monster->GoToPrevPos();
			break;
		case '|':
			// Send attack message
			break;
		}
	}
}

void World::InitLevelObjects()
{
	LevelMatrix map = levels[currentLevelIdx].GetMap();

	size_t height = map.size();
	size_t width = map[0].size();
	for (size_t lineIdx = 0; lineIdx < height; ++lineIdx)
	{
		for (size_t chIdx = 0; chIdx < width; ++chIdx)
		{
			if (map[lineIdx][chIdx] == 'M')
			{
				Monster newMonster;
				newMonster.SetInitialPosition(Position(chIdx, lineIdx));
				monsters.push_back(newMonster);
			}
			// TODO: Init other objects
		}
	}
}