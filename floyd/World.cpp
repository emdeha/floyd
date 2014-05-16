#include <iostream>
#include <fstream>
#include <algorithm>

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

	//eventListeners.push_back(&hero);

	InitLevelObjects();
}

void World::Display()
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
		if (monster->GetHealth() <= 0)
		{
			levels[currentLevelIdx].SetTileAtPosition(monster->GetPosition(), ' ');
			monsters.erase(monster);
			break;
		}
		monster->Update(this);
	}
	for (auto particle = particles.begin(); particle != particles.end(); ++particle)
	{
		particle->Update();
	}
	UpdateCollisions();
	levels[currentLevelIdx].UpdateLevelMatrix(this);
}

void World::AddParticle(Position position)
{
	Particle newParticle;
	newParticle.SetPosition(position);

	Position direction(0,0);
	direction.x = GetRandomInRange(-1, 1);
	direction.y = GetRandomInRange(-1, 1);

	newParticle.SetDirection(direction);

	particles.push_back(newParticle);
}

//void World::NotifyEventListeners(const Event &forEvent)
//{
//	for (size_t idx = 0; idx < eventListeners.size(); ++idx)
//	{
//		eventListeners[idx]->OnEvent(forEvent);
//	}
//}

Position World::GetPlayerPos() const
{
	return hero.GetPosition(); 
}
Position World::GetPlayerPrevPos() const
{
	return hero.GetPrevPos();
}

Hero& World::GetHero()
{
	return hero;
}

const std::vector<Monster>& World::GetMonsters() const
{
	return monsters;
}
const std::vector<Particle>& World::GetParticles() const
{
	return particles;
}

Monster* World::GetMonsterAtPos(Position position)
{
	for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
	{
		if (monster->GetPosition().IsEqual(position))
		{
			return &(*monster);
		}
	}

	return nullptr;
}

///////////////////////
//  Private Methods  //
///////////////////////

void World::UpdateCollisions()
{
	CheckHeroCollision();
	CheckMonsterCollision();
	CheckParticleCollision();
}

void World::CheckHeroCollision()
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
		{
			Monster *currentMonster = GetMonsterAtPos(currentHeroPos);
			if (currentMonster)
			{
				currentMonster->ApplyDamage(hero.GetDamage());
			}
			hero.GoToPrevPos();
		}
		break;
	case 'E':
		currentLevelIdx++;
		Position startingPos = levels[currentLevelIdx].GetStartingPos();
		hero.SetInitialPosition(startingPos);
		monsters.clear();
		InitLevelObjects();
		return;
	}
}

void World::CheckMonsterCollision()
{
	LevelMatrix currentMap = levels[currentLevelIdx].GetMap();
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
			monster->GoToPrevPos();
			break;
		}
	}
}

void World::CheckParticleCollision()
{
	LevelMatrix currentMap = levels[currentLevelIdx].GetMap();
	auto particle = particles.begin();
	while (particle != particles.end())
	{
		Position particlePos = particle->GetPosition();
		char particleTile = currentMap[particlePos.y][particlePos.x];
		switch(particleTile)
		{
			// Particles get destroyed when they hit an object.
		case '#':
		case 'M':
		case 'O':
		case 'N':
		case 'T':
		case '*':
		case 'E':
			levels[currentLevelIdx].SetTileAtPosition(particle->GetPrevPos(), ' ');
			levels[currentLevelIdx].SetTileAtPosition(particlePos, particleTile);
			particle = particles.erase(particle);
			break;
		case '|':
			hero.Hurt(particle->GetDamage());

			levels[currentLevelIdx].SetTileAtPosition(particle->GetPrevPos(), ' ');
			levels[currentLevelIdx].SetTileAtPosition(particlePos, particleTile);
			particle = particles.erase(particle);
			break;
		default:
			++particle;
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

				//eventListeners.push_back(&monsters.back());
			}
			// TODO: Init other objects
		}
	}
}

World::~World()
{
	//for (auto iter = eventListeners.begin(); iter != eventListeners.end(); ++iter)
	//{
	//	delete (*iter);
	//}
	//eventListeners.clear();
}