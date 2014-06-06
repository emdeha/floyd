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

Tile::Tile() : sprite(' '), logicalSprite(' '), position() {}

Tile::Tile(char newSprite, char newLogicalSprite, const Position &newPosition) 
	: sprite(newSprite), logicalSprite(newLogicalSprite), position(newPosition) {}


bool Tile::IsValid() const
{
	return position.IsPositive();
}


////////////////
//  LevelMap  //
////////////////

LevelMap::LevelMap() : map(0), width(0), height(0) {}

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

			width = currX;
		}
	}
	else
	{
		std::cerr << "Error: Opening level file!\n";
		return;
	}

	height = currY;

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
	return Tile(' ', ' ', Position(-1, -1));
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

void LevelMap::SetTileAtPosition(const Position &position, const Tile &newTile)
{
	for (size_t idx = 0; idx < map.size(); ++idx)
	{
		if (map[idx].position.IsEqual(position))
		{
			map[idx] = newTile;
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

void LevelMap::SetSpriteForLogicalSprite(char newSprite, char logicalSprite)
{
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		if (tile->logicalSprite == logicalSprite)
		{
			tile->sprite = newSprite;
		}
	}
}

std::vector<Position> LevelMap::GetPositionsForLogicalSprite(char logicalSprite) const
{
	std::vector<Position> positions(0);
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		if (tile->logicalSprite == logicalSprite)
		{
			positions.push_back(tile->position);
		}
	}

	return positions;
}

std::vector<Tile> LevelMap::GetTilesForLogicalSprite(char logicalSprite) const
{
	std::vector<Tile> tiles(0);
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		if (tile->logicalSprite == logicalSprite)
		{
			tiles.push_back((*tile));
		}
	}

	return tiles;
}

Tile LevelMap::FindNearestTileToTile(const Tile &tileOther, Direction dir) const
{
	Position minPos(-1, -1);
	int minDistance = 999; // TODO: Remove magicality (or magicalitness?, maybe magicalidition...).

	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		if (tile->sprite == tileOther.sprite && tile->logicalSprite == tileOther.logicalSprite)
		{
			// That's so dirty it makes porno movies suitable for children
			bool isTileAfterOther = false;
			switch (dir)
			{
			case DIR_RIGHT:
				isTileAfterOther = tile->position.x > tileOther.position.x;  
				break;
			case DIR_LEFT:
				isTileAfterOther = tile->position.x < tileOther.position.x;
				break;
			case DIR_UP:
				isTileAfterOther = tile->position.y < tileOther.position.y;
				break;
			case DIR_DOWN:
				isTileAfterOther = tile->position.y > tileOther.position.y;
				break;
			}

			int currentDistance = tileOther.position.GetDistanceSq(tile->position); 
			if (isTileAfterOther && currentDistance < minDistance)
			{
				minPos.x = tile->position.x;
				minPos.y = tile->position.y;

				minDistance = currentDistance;
			}
		}
	}

	return Tile(tileOther.sprite, tileOther.logicalSprite, minPos);
}

size_t LevelMap::GetWidth() const
{
	return width;
}
size_t LevelMap::GetHeight() const
{
	return height;
}


/////////////
//  Level  //
/////////////

Level::Level() : name(""), map(0), cutscene(0), endscene(0), npcscene(0),
		  hasBegan(false), isShowingEndscene(false), isShowingNPCscene(false),
		  npcSceneDuration_s(3), cutsceneDuration_s(5),
		  isExitUnblocked(false), isExitDisplayConditionMet(false),
		  hasSpawnedMonstersForLevel(false), hasSpawnPositions(false)
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
			world->GetHero().SetPrevTile(tiles.GetTileAtPosition(heroPos).sprite);
			tiles.SetSpriteAtPosition(heroPos, TILE_HERO);
		}
		lastFrameHeroPos = heroPos;

		auto &monsters = world->GetMonsters();
		for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
		{
			Position monsterPrevPos = monster->GetPrevPos();
			tiles.SetSpriteAtPosition(monsterPrevPos, monster->GetPrevTile());
			Position monsterPos = monster->GetPosition();
			monster->SetPrevTile(tiles.GetTileAtPosition(monsterPos).sprite);
			tiles.SetSpriteAtPosition(monsterPos, TILE_MONSTER);
		}

		auto &particles = world->GetParticles();
		for (auto particle = particles.begin(); particle != particles.end(); ++particle)
		{
			Position particlePrevPos = particle->GetPrevPos();
			tiles.SetSpriteAtPosition(particlePrevPos, particle->GetPrevTile());
			Position particlePos = particle->GetPosition();
			particle->SetPrevTile(tiles.GetTileAtPosition(particlePos).sprite);
			tiles.SetSpriteAtPosition(particlePos, TILE_PARTICLE);
		}
	}
}

Position Level::GetStartingPos() const
{
	return tiles.GetPositionForLogicalSprite(TILE_START);
}

void Level::SpawnMonsters(World *world)
{
	auto spawnPoints = tiles.GetTilesForLogicalSprite(TILE_MONSTER_SPAWN);
	for (auto spawnPoint = spawnPoints.begin(); spawnPoint != spawnPoints.end(); ++spawnPoint)
	{
		world->SpawnMonsterAtPos(spawnPoint->position);	
		SetSpriteAtPosition(spawnPoint->position, TILE_EMPTY);
	}
	hasSpawnedMonstersForLevel = true;
}

void Level::UnblockExit()
{
	tiles.SetSpriteForLogicalSprite(TILE_EMPTY, TILE_EXIT_BLOCK);
}

void Level::ShowTeleport()
{
	tiles.SetSpriteForLogicalSprite(TILE_TELEPORT, TILE_TELEPORT);
}

void Level::ShowNPCscene()
{
	lastNpcSceneInterval_s = GetTimeSinceEpoch();
	isShowingNPCscene = true;
}

void Level::SetIsExitDisplayConditionMet(bool newIsExitDisplayConditionMet)
{
	isExitDisplayConditionMet = newIsExitDisplayConditionMet;
	Position hiddenExitPos = tiles.GetPositionForLogicalSprite(TILE_HIDDEN_EXIT);
	if (isExitDisplayConditionMet && hiddenExitPos.IsPositive())
	{
		tiles.SetSpriteAtPosition(hiddenExitPos, TILE_EXIT);
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
	// TODO: Can be better
	auto spawnPoints = tiles.GetPositionsForLogicalSprite(TILE_MONSTER_SPAWN);
	return spawnPoints.size() > 0;
}

char Level::GetSpriteAtPosition(const Position &tilePos) const
{
	return tiles.GetTileAtPosition(tilePos).sprite;
}

///
/// TODO: A map you should use for the tiles. A tile struct you should create. 
///		  Faster and cleaner it will be. -- Yoda
///		  Done. -- Young Padwan
///
Position Level::GetNearestEntryPosForSprite(char sprite, const Position &spritePos) const
{
	char spriteToSearchFor = ' ';
	Direction dir;
	switch (sprite)
	{
	case TILE_GO_RIGHT:
		spriteToSearchFor = TILE_GO_LEFT;
		dir = DIR_RIGHT;
		break;
	case TILE_GO_LEFT:
		spriteToSearchFor = TILE_GO_RIGHT;
		dir = DIR_LEFT;
		break;
	case TILE_GO_UP:
		spriteToSearchFor = TILE_GO_DOWN;
		dir = DIR_UP;
		break;
	case TILE_GO_DOWN:
		spriteToSearchFor = TILE_GO_UP;
		dir = DIR_DOWN;
		break;
	default:
		return Position(-1, -1);
	};

	Tile foundTile = 
		tiles.FindNearestTileToTile(Tile(spriteToSearchFor, spriteToSearchFor, spritePos), dir); 
	return foundTile.position;
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