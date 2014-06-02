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


////////////
//  Tile  //
////////////

Tile::Tile()
	: sprite(' '), logicalSprite(' '), position() {}

Tile::Tile(char newSprite, char newLogicalSprite, const Position &newPosition) 
	: sprite(newSprite), logicalSprite(newLogicalSprite), position(newPosition) {}


bool Tile::IsValid() const
{
	return position.IsPositive();
}


////////////////
//  LevelMap  //
////////////////

LevelMap::LevelMap() 
	: map(0) {}

void LevelMap::Init(const std::string &levelFile)
{
	std::ifstream level(worldDir + levelFile);

	size_t currY = 0;
	if (level.is_open())
	{
		std::string line;
		while (std::getline(level, line))
		{
			size_t currX = 0;
			for (auto tile = line.begin(); tile != line.end(); ++tile)
			{
				char tileLogicalSprite = *tile;
				char tileSprite = *tile;
				Position tilePosition = Position(currX, currY);
				if ((*tile) == TILE_EXIT_BLOCK)
				{
					tileSprite = TILE_WALL;
				}
				if ((*tile) == TILE_HIDDEN_EXIT)
				{
					tileSprite = TILE_WALL;
				}
				if ((*tile) == TILE_TELEPORT)
				{
					tileSprite = TILE_EMPTY;
				}
				if ((*tile) == TILE_MONSTER_SPAWN)
				{
					tileSprite = TILE_WALL;
					tileLogicalSprite = TILE_MONSTER;
				}
				if ((*tile) == TILE_START)
				{
					tileSprite = TILE_HERO;
				}
				if ((*tile) == TILE_KILL_BLOCK)
				{
					tileSprite = TILE_EMPTY;
				}

				map.push_back(Tile(tileSprite, tileLogicalSprite, tilePosition));

				currX++;
			}
			currY++;
			//map.push_back(line);
			//size_t exitBlockX = line.find(TILE_EXIT_BLOCK);
			//if (exitBlockX != line.npos)
			//{
			//	exitBlockPos = Position(exitBlockX, currYPos);
			//}
			//size_t teleportX = line.find(TILE_TELEPORT);
			//if (teleportX != line.npos)
			//{
			//	teleportPos = Position(teleportX, currYPos);
			//}
			//size_t hiddenExitX = line.find(TILE_HIDDEN_EXIT);
			//if (hiddenExitX != line.npos)
			//{
			//	hiddenExitPos = Position(hiddenExitX, currYPos);
			//}
			//GetSpawnPositionsFromLine(line, currYPos);	
			//currYPos++;
		}
	}
	else
	{
		std::cerr << "Error: Opening level file!\n";
		return;
	}

	//if (teleportPos.IsPositive())
	//{
	//	SetTileAtPosition(teleportPos, TILE_EMPTY);
	//}
	//if (exitBlockPos.IsPositive())
	//{
	//	SetTileAtPosition(exitBlockPos, TILE_WALL);
	//}
	//if (hiddenExitPos.IsPositive())
	//{
	//	SetTileAtPosition(hiddenExitPos, TILE_WALL);
	//}

	//for (auto spawnPos = monsterSpawnPoints.begin(); spawnPos != monsterSpawnPoints.end();
	//	++spawnPos)
	//{
	//	SetTileAtPosition((*spawnPos), TILE_WALL);
	//}

	level.close();
}

void LevelMap::Display() const
{
	int prevTileY = 0;
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		std::cout << tile->sprite;
		if (prevTileY != tile->position.y)
		{
			prevTileY = tile->position.y;
			std::cout << std::endl;
		}
	}
}

Tile LevelMap::GetTileAtPosition(const Position &position) const
{
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		if (tile->position.IsEqual(position))
		{
			return (*tile);
		}
	}

	std::cerr << "Warning: No tile at (" << position.x << ", " << position.y << ") position!\n";
	return Tile(' ',' ',Position(-1, -1));
}

void LevelMap::SetSpriteAtPosition(const Position &position, char sprite)
{
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		if (tile->position.IsEqual(position))
		{
			tile->sprite = sprite;
			return;
		}
	}
}

Position LevelMap::GetPositionForLogicalSprite(char logicalSprite) const
{
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		if (tile->logicalSprite == logicalSprite)
		{
			return tile->position;
		}
	}

	std::cerr << "Warning: No starting position defined!\n";
	return Position(-1, -1);
}


/////////////
//  Level  //
/////////////

Level::Level() : name(""), map(0), cutscene(0), endscene(0), npcscene(0),
		  hasBegan(false), isShowingEndscene(false), isShowingNPCscene(false),
		  npcSceneDuration_s(3), cutsceneDuration_s(5),
		  isExitUnblocked(false), isExitDisplayConditionMet(false),
		  monsterSpawnPoints(0), hasSpawnedMonstersForLevel(false),
		  exitBlockPos(-1,-1), teleportPos(-1,-1), hiddenExitPos(-1,-1)
{
	drawBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
	setBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (drawBuffer == INVALID_HANDLE_VALUE || setBuffer == INVALID_HANDLE_VALUE)
	{
		// std::cerr << "CreateConsoleScreenBuffer failed - " << GetLastError() << std::endl;
		return;
	}
}

void Level::Init(const std::string &levelFile)
{
	name = levelFile;
	tiles.Init(levelFile);
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
		tiles.Display();
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
			tiles.SetSpriteAtPosition(heroPrevPos, world->GetHero().GetPrevTile());
			//map[heroPrevPos.y][heroPrevPos.x] = world->GetHero().GetPrevTile(); 
			world->GetHero().SetPrevTile(tiles.GetTileAtPosition(heroPos).sprite);//map[heroPos.y][heroPos.x]);
			//map[heroPos.y][heroPos.x] = TILE_HERO;
			tiles.SetSpriteAtPosition(heroPos, TILE_HERO);
		}
		lastFrameHeroPos = heroPos;

		auto &monsters = world->GetMonsters();
		for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
		{
			Position monsterPrevPos = monster->GetPrevPos();
			//map[monsterPrevPos.y][monsterPrevPos.x] = monster->GetPrevTile();
			tiles.SetSpriteAtPosition(monsterPrevPos, monster->GetPrevTile());
			Position monsterPos = monster->GetPosition();
			monster->SetPrevTile(tiles.GetTileAtPosition(monsterPos).sprite);//map[monsterPos.y][monsterPos.x]);
			//map[monsterPos.y][monsterPos.x] = TILE_MONSTER;
			tiles.SetSpriteAtPosition(monsterPos, TILE_MONSTER);
		}

		auto &particles = world->GetParticles();
		for (auto particle = particles.begin(); particle != particles.end(); ++particle)
		{
			Position particlePrevPos = particle->GetPrevPos();
			//map[particlePrevPos.y][particlePrevPos.x] = particle->GetPrevTile();
			tiles.SetSpriteAtPosition(particlePrevPos, particle->GetPrevTile());
			Position particlePos = particle->GetPosition();
			particle->SetPrevTile(tiles.GetTileAtPosition(particlePos).sprite);//map[particlePos.y][particlePos.x]);
			//map[particlePos.y][particlePos.x] = TILE_PARTICLE;
			tiles.SetSpriteAtPosition(particlePos, TILE_PARTICLE);
		}
	}
}

Position Level::GetStartingPos() const
{
	//for (size_t y = 0; y < map.size(); ++y)
	//{
	//	for (size_t x = 0; x < map[y].size(); ++x)
	//	{
	//		if (map[y][x] == TILE_START)
	//		{
	//			return Position(x, y);	
	//		}
	//	}
	//}
	return tiles.GetPositionForLogicalSprite(TILE_START);
}

void Level::SpawnMonsters(World *world)
{
	for (auto spawnPoint = monsterSpawnPoints.begin(); spawnPoint != monsterSpawnPoints.end();
		++spawnPoint)
	{
		world->SpawnMonsterAtPos((*spawnPoint));	
		SetTileAtPosition((*spawnPoint), TILE_EMPTY);
	}
	hasSpawnedMonstersForLevel = true;
	monsterSpawnPoints.clear();
}

void Level::UnblockExit()
{
	SetTileAtPosition(exitBlockPos, TILE_EMPTY);
}

void Level::ShowTeleport()
{
	SetTileAtPosition(teleportPos, TILE_TELEPORT);
}

void Level::ShowNPCscene()
{
	lastNpcSceneInterval_s = GetTimeSinceEpoch();
	isShowingNPCscene = true;
}

void Level::SetIsExitDisplayConditionMet(bool newIsExitDisplayConditionMet)
{
	isExitDisplayConditionMet = newIsExitDisplayConditionMet;
	if (isExitDisplayConditionMet && hiddenExitPos.IsPositive())
	{
		SetTileAtPosition(hiddenExitPos, TILE_EXIT);
	}
}

void Level::ResetLastCutsceneInterval()
{
	lastCutsceneInterval_s = GetTimeSinceEpoch();
}

bool Level::HasSpawnedMonstersForLevel() const
{
	return hasSpawnedMonstersForLevel;
}
bool Level::AreThereMonsterSpawnPositions() const
{
	return ! monsterSpawnPoints.empty();
}

char Level::GetTileAtPosition(const Position &tilePos) const
{
	return map[tilePos.y][tilePos.x];
}

///
/// The nearest tile has the least diagonal distance from the player.
/// 
/// TODO: A map you should use for the tiles. A tile struct you should create. 
///		  Faster and cleaner it will be. -- Yoda
///
Position Level::GetNearestEntryPosForTile(char tile, const Position &tilePos) const
{
	const int MIN_INITIAL = 999; 
	char tileToSearchFor = ' ';
	int preferredX = 0;
	int preferredY = 0;
	if (tile == TILE_GO_UP) 
	{
		tileToSearchFor = TILE_GO_DOWN;
		preferredX = 0;
		for (size_t yPos = 0; yPos < tilePos.y; ++yPos)
		{
			for (size_t xPos = 0; xPos < map[yPos].size(); ++xPos)
			{
				if (map[yPos][xPos] == tileToSearchFor && preferredX < xPos)
				{
					preferredX = xPos;
					preferredY = yPos;
				}
			}
		}
	}
	else if (tile == TILE_GO_DOWN)
	{
		tileToSearchFor = TILE_GO_UP;
		preferredX = MIN_INITIAL;
		for (size_t yPos = tilePos.y; yPos < map.size(); ++yPos)
		{
			for (size_t xPos = 0; xPos < map[yPos].size(); ++xPos)
			{
				if (map[yPos][xPos] == tileToSearchFor && preferredX > xPos)
				{
					preferredX = xPos;
					preferredY = yPos;
				}
			}
		}
	}
	else if (tile == TILE_GO_LEFT) 
	{
		tileToSearchFor = TILE_GO_RIGHT;
		preferredX = 0;
		for (size_t yPos = 0; yPos < map.size(); ++yPos)
		{
			for (size_t xPos = 0; xPos < tilePos.x; ++xPos)
			{
				if (map[yPos][xPos] == tileToSearchFor && preferredX < xPos)
				{
					preferredX = xPos;
					preferredY = yPos;
				}
			}
		}
	}
	else if (tile == TILE_GO_RIGHT)
	{
		tileToSearchFor = TILE_GO_LEFT;
		preferredX = MIN_INITIAL;
		for (size_t yPos = 0; yPos < map.size(); ++yPos)
		{
			for (size_t xPos = tilePos.x; xPos < map[yPos].size(); ++xPos)
			{
				if (map[yPos][xPos] == tileToSearchFor && preferredX > xPos)
				{
					preferredX = xPos;
					preferredY = yPos;
				}
			}
		}
	}

	if (tileToSearchFor != ' ')
	{
		return Position(preferredX, preferredY);
	}

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
	size_t firstPosX = line.find(TILE_MONSTER_SPAWN);
	while (firstPosX != line.npos)
	{
		monsterSpawnPoints.push_back(Position(firstPosX, preferredY));
		firstPosX = line.find(TILE_MONSTER_SPAWN, firstPosX + 1);
	}
}