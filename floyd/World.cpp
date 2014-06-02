#include <iostream>
#include <fstream>
#include <algorithm>

#include "World.h"
#include "Dirs.h"
#include "Wrapper.h"
#include "Scripts.h"


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

World::World() 
	: levels(0), currentLevelIdx(0) {}

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

	///
	/// Adding scripts
	///
	scripts.push_back(new LevelScript());

	for (auto script = scripts.begin(); script != scripts.end(); ++script)
	{
		(*script)->OnStart(this);
	}
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
		char dir = static_cast<char>(_getch());
		
		switch (dir)
		{
		case KEY_UP:
			hero.Move(DIR_UP);
			break;
		case KEY_LEFT:
			hero.Move(DIR_LEFT);
			break;
		case KEY_DOWN:
			hero.Move(DIR_DOWN);
			break;
		case KEY_RIGHT:
			hero.Move(DIR_RIGHT);
			break;
		case KEY_QUIT:
			std::cout << "QUIT\n";
			exit(1);
			break;
		case KEY_KILL_ALL:
			KillAllMonsters();
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
			levels[currentLevelIdx].SetTileAtPosition(monster->GetPosition(), TILE_EMPTY);
			monsters.erase(monster);
			break;
		}
		monster->Update(this);
	}
	for (auto particle = particles.begin(); particle != particles.end(); ++particle)
	{
		particle->Update();
	}
	//UpdateCollisions();

	for (auto script = scripts.begin(); script != scripts.end(); ++script)
	{
	//	(*script)->OnUpdate(this);
	}

	//levels[currentLevelIdx].UpdateLevelMatrix(this);
	//UpdateCollisions();
}

void World::AddParticle(Position position)
{
	Particle newParticle;
	newParticle.SetPosition(position);

	Position direction(0,0);
	while (direction.IsEqual(Position(0,0)))
	{
		direction.x = GetRandomInRange(-1, 1);
		direction.y = GetRandomInRange(-1, 1);
	}

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

std::vector<Monster>& World::GetMonsters()
{
	return monsters;
}
std::vector<Particle>& World::GetParticles()
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
void World::SpawnMonsterAtPos(Position position)
{
	Monster newMonster;
	newMonster.SetInitialPosition(position);
	monsters.push_back(newMonster);
}

Level* World::GetCurrentLevel()
{
	return &levels[currentLevelIdx];
}

int World::GetCurrentLevelIdx()
{
	return currentLevelIdx;
}

bool World::AreMonstersDead() const
{
	return monsters.empty() && (! levels[currentLevelIdx].AreThereMonsterSpawnPositions());
}

void World::KillAllMonsters()
{
	for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
	{
		levels[currentLevelIdx].SetTileAtPosition(monster->GetPosition(), TILE_EMPTY);
	}
	monsters.clear();
	for (auto particle = particles.begin(); particle != particles.end(); ++particle)
	{
		levels[currentLevelIdx].SetTileAtPosition(particle->GetPosition(), TILE_EMPTY);
	}
	particles.clear();
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
	case TILE_WALL:
		{
			hero.GoToPrevPos();
		}
		break;
	case TILE_TELEPORT:
	case TILE_DREAMS:
		{
			levels[currentLevelIdx].ShowEndscene();
		}
		break;
	case TILE_NPC:
		{
			levels[currentLevelIdx].ShowNPCscene();
			hero.SetHasTalkedToNPC(true);
			hero.GoToPrevPos();
		}
		break;
	case TILE_STASH:
		{
			hero.GoToPrevPos();
			if (!levels[currentLevelIdx].HasSpawnedMonstersForLevel())
			{
				levels[currentLevelIdx].SetIsExitDisplayConditionMet(true);
			}
		}
		break;
	case TILE_MONSTER:
		{
			Monster *currentMonster = GetMonsterAtPos(currentHeroPos);
			if (currentMonster)
			{
				currentMonster->ApplyDamage(hero.GetDamage());
			}
			hero.GoToPrevPos();
		}
		break;
	case TILE_GO_DOWN:
	case TILE_GO_UP:
	case TILE_GO_LEFT:
	case TILE_GO_RIGHT:
		{
			Position entryPos = levels[currentLevelIdx].GetNearestEntryPosForTile(currentTile, 
																				  currentHeroPos);
			if (entryPos.IsPositive())
			{
				// 02-Jun-2014: Yes, I will. Due to the lack of layers I have to remove the player sprite 
				//				from the current position.
				levels[currentLevelIdx].SetTileAtPosition(hero.GetPosition(), currentTile);
				levels[currentLevelIdx].SetTileAtPosition(hero.GetPrevPos(), hero.GetPrevTile());

				// 01-Jun-2014: Will you break the space-time continuum?
				hero.SetInitialPosition(entryPos);

				hero.SetPrevTile(levels[currentLevelIdx].GetTileAtPosition(entryPos));
			}
		}
		break;
	case TILE_EXIT:
		{
			currentLevelIdx++;
			levels[currentLevelIdx].ResetLastCutsceneInterval();
			Position startingPos = levels[currentLevelIdx].GetStartingPos();
			hero.SetInitialPosition(startingPos);
			monsters.clear();
			particles.clear();
			InitLevelObjects();
		}
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
		case TILE_WALL:
			monster->GoToPrevPos();
			break;
		case TILE_HERO:
			//monster->GoToPrevPos();
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
		// TODO: Put in separate method
		if (particlePos.y >= currentMap.size() - 1 || particlePos.x >= currentMap[0].size() - 1 ||
			particlePos.y <= 0 || particlePos.x <= 0)
		{
			levels[currentLevelIdx].SetTileAtPosition(particle->GetPrevPos(), 
													  particle->GetPrevTile());
			levels[currentLevelIdx].SetTileAtPosition(particlePos, particleTile);
			particle = particles.erase(particle);
			break;
		}
		//
		if (particleTile == TILE_WALL || particleTile == TILE_MONSTER || particleTile == TILE_STASH ||
			particleTile == TILE_NPC || particleTile == TILE_TELEPORT || particleTile == TILE_DREAMS || 
			particleTile == TILE_EXIT || particleTile == TILE_HERO)
		{
			if (particleTile == TILE_HERO)
			{
				hero.Hurt(particle->GetDamage());
			}

			// Particles get destroyed when they hit an object.
			levels[currentLevelIdx].SetTileAtPosition(particle->GetPrevPos(), 
													  particle->GetPrevTile());
			particle = particles.erase(particle);
		}
		else
		{
			++particle;
		}
	}
}

void World::InitLevelObjects()
{
	//LevelMatrix map = levels[currentLevelIdx].GetMap();

	//size_t height = map.size();
	//size_t width = map[0].size();
	//for (size_t lineIdx = 0; lineIdx < height; ++lineIdx)
	//{
	//	for (size_t chIdx = 0; chIdx < width; ++chIdx)
	//	{
	//		if (map[lineIdx][chIdx] == TILE_MONSTER)
	//		{
	//			Monster newMonster;
	//			newMonster.SetInitialPosition(Position(chIdx, lineIdx));
	//			monsters.push_back(newMonster);

	//			//eventListeners.push_back(&monsters.back());
	//		}
	//		// TODO: Init other objects
	//	}
	//}
}

World::~World()
{
	//for (auto iter = eventListeners.begin(); iter != eventListeners.end(); ++iter)
	//{
	//	delete (*iter);
	//}
	//eventListeners.clear();
	for (auto iter = scripts.begin(); iter != scripts.end(); ++iter)
	{
		delete (*iter);
	}
	scripts.clear();
}