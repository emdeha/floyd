#include "stdafx.h"
#include "Level.h"

#include "../Floyd_World/World.h"
#include "../Floyd_General/Dirs.h"
#include "../Floyd_General/Reporting.h"
#include "../Floyd_Level/Tile.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <Windows.h>


////////////////
//  LevelMap  //
////////////////

LevelMap::LevelMap() : map(0), width(0), height(0) {}

void LevelMap::Init(const std::string &levelFile)
{
	InitSpriteForLogicalSprite();
	InitMap(levelFile);

	std::stringstream msg;
	msg << "Initted LevelMap " << levelFile;
	Logger::Log(msg.str(), LOW);
}

void LevelMap::InitSpriteForLogicalSprite()
{
	std::ifstream assocFile(ResolveFileName(FILE_TILE_ASSOC, DIR_WORLD));

	if (assocFile.is_open())
	{
		std::string line;
		while (std::getline(assocFile, line).good())
		{
			std::string safeLine = Trim(line);
			if (line[2] == ' ')
			{
				safeLine.push_back(' '); // Enables spaces for sprites
			}

			if (safeLine.empty() || safeLine == "" || safeLine.length() != 3 || safeLine[1] != ':')
			{
				assocFile.close();
				Report::Error("Incorrect assoc-tile file", __LINE__, __FILE__);
			}

			char sprite = safeLine[0];
			char logicalSprite = safeLine[2];
			spriteForLogicalSprite.insert(std::make_pair(sprite, logicalSprite));
		}
	}
	else 
	{
		Report::UnexpectedError("Can't open assoc tile file", __LINE__, __FILE__);
		return;
	}

	assocFile.close();
}

void LevelMap::InitMap(const std::string &levelFile)
{
	std::ifstream level(DIR_WORLD + levelFile);

	size_t currY = 0;
	if (level.is_open())
	{
		std::string line;
		while (std::getline(level, line).good())
		{
			size_t currX = 0;
			for (auto tile = line.begin(); tile != line.end(); ++tile)
			{
				char tileLogicalSprite = *tile;

				// Acquires sprite according to logical sprite
				char tileSprite = *tile;
				auto tileSpriteIter = spriteForLogicalSprite.find((*tile));
				if (tileSpriteIter != spriteForLogicalSprite.end())
				{
					tileSprite = tileSpriteIter->second;
				}

				Position tilePosition = Position(currX, currY);

				map.push_back(Tile(tileSprite, tileLogicalSprite, tilePosition));

				currX++;
			}
			currY++;

			width = currX;
		}
	}
	else
	{
		Report::UnexpectedError("Can't open level file", __LINE__, __FILE__);
		return;
	}

	height = currY;

	level.close();
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

	std::stringstream warning;
	warning << "No tile at (" << position.x << ", " << position.y << ") position!\n";
	Report::Warning(warning.str(), __LINE__, __FILE__);

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

void LevelMap::SetLogicalSpriteAtPosition(const Position &position, char logicalSprite)
{
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		if (tile->position.IsEqual(position))
		{
			tile->logicalSprite = logicalSprite;
			return;
		}
	}
}

void LevelMap::SetTile(const Tile &newTile)
{
	for (size_t idx = 0; idx < map.size(); ++idx)
	{
		if (map[idx].position.IsEqual(newTile.position))
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

	Report::Warning("No starting position defined", __LINE__, __FILE__);
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

bool LevelMap::HasTileWithLogicalSprite(char logicalSprite) const
{
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		if (tile->logicalSprite == logicalSprite)
		{
			return true;
		}
	}

	return false;
}

std::string LevelMap::GetRawMap() const
{
	std::string rawMap;
	int currX = 0;
	for (auto tile = map.begin(); tile != map.end(); ++tile)
	{
		rawMap.push_back(tile->sprite);
		++currX;
		if (currX >= width)
		{
			rawMap.push_back('\n');
			currX = 0;
		}
	}
	rawMap.push_back('\n');

	return rawMap;
}

const Sprite* LevelMap::AsSprite() const
{
	mapAsSprite = Sprite(width, height);
	mapAsSprite.LoadTextureFromRawData(GetRawMap());
	return &mapAsSprite;
}

Tile LevelMap::FindNearestTileToTile(const Tile &tileOther, Direction dir) const
{
	Position minPos(-1, -1);
	int minDistance = 999;

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

void LevelMap::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		size_t mapSize = map.size();
		saveStream.write((char*)&mapSize, sizeof(size_t));
		saveStream.write((char*)&width, sizeof(int));
		saveStream.write((char*)&height, sizeof(int));

		for (auto tile = map.begin(); tile != map.end(); ++tile)
		{
			tile->Serialize(saveStream);
		}
	}
	else
	{
		Report::UnexpectedError("Can't serialize LevelMap", __LINE__, __FILE__);
	}
}
void LevelMap::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		size_t mapSize = 0;
		loadStream.read((char*)&mapSize, sizeof(size_t));
		loadStream.read((char*)&width, sizeof(int));
		loadStream.read((char*)&height, sizeof(int));

		map.clear();
		for (size_t idx = 0; idx < mapSize; ++idx)
		{
			Tile newTile;
			newTile.Deserialize(loadStream);
			map.push_back(newTile);
		}
	}
	else
	{
		Report::UnexpectedError("Can't deserialize LevelMap", __LINE__, __FILE__);
	}
}


/////////////
//  Level  //
/////////////

Level::Level() : name(""), hasBegan(false), isShowingEndscene(false), isShowingNPCscene(false),
		  isExitUnblocked(false), isExitDisplayConditionMet(false), hasSpawnedMonstersForLevel(false),
		  hasSpawnPositions(false)
{
}

void Level::Init(const std::string &levelFile)
{
	name = levelFile;
	tiles.Init(levelFile);
	scenes[SCENE_TYPE_CUTSCENE].SetSceneLastInterval(GetTimeSinceEpoch());

	std::stringstream msg;
	msg << "Initted level " << levelFile;
	Logger::Log(msg.str(), LOW);
}

void Level::InitCutscenes(const std::vector<std::string> &cutsceneFileNames)
{
	for (auto iter = cutsceneFileNames.begin(); iter != cutsceneFileNames.end(); ++iter)
	{
		if (iter->find('e') != iter->npos)
		{
			scenes[SCENE_TYPE_ENDSCENE].Init((*iter));
		}
		else if (iter->find('c') != iter->npos)
		{
			scenes[SCENE_TYPE_CUTSCENE].Init((*iter));
		}
		else if (iter->find('n') != iter->npos)
		{
			scenes[SCENE_TYPE_NPCSCENE].Init((*iter));
		}
		else if ((*iter) == "")
		{
			continue;
		}
		else
		{
			Report::Error("Invalid token for cutscene", __LINE__, __FILE__);
			return;
		}
	}
}

void Level::Update()
{
	if (isShowingNPCscene)
	{
		if (GetTimeSinceEpoch() - scenes[SCENE_TYPE_NPCSCENE].GetSceneLastInterval() > 
			scenes[SCENE_TYPE_NPCSCENE].GetSceneDuration())
		{
			isShowingNPCscene = false;
			scenes[SCENE_TYPE_NPCSCENE].SetSceneLastInterval(GetTimeSinceEpoch());
		}
	}
	else if ( ! hasBegan)
	{
		if (GetTimeSinceEpoch() - scenes[SCENE_TYPE_CUTSCENE].GetSceneLastInterval() >
			scenes[SCENE_TYPE_CUTSCENE].GetSceneDuration())
		{
			hasBegan = true;
			scenes[SCENE_TYPE_CUTSCENE].SetSceneLastInterval(GetTimeSinceEpoch());
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
		tiles.SetTile(Tile(TILE_EMPTY, TILE_EMPTY, spawnPoint->position));
	}
	hasSpawnedMonstersForLevel = true;
}

void Level::UnblockExit()
{
	Position exitBlockPos = tiles.GetPositionForLogicalSprite(TILE_EXIT_BLOCK);
	tiles.SetTile(Tile(TILE_EMPTY, TILE_EMPTY, exitBlockPos));
	isExitUnblocked = true;
}

void Level::ShowTeleport()
{
	tiles.SetSpriteForLogicalSprite(TILE_TELEPORT, TILE_TELEPORT);
}

void Level::ShowNPCscene()
{
	scenes[SCENE_TYPE_NPCSCENE].SetSceneLastInterval(GetTimeSinceEpoch());
	isShowingNPCscene = true;
}

void Level::SetIsExitDisplayConditionMet(bool newIsExitDisplayConditionMet)
{
	isExitDisplayConditionMet = newIsExitDisplayConditionMet;
	if (isExitDisplayConditionMet)
	{
		Position hiddenExitPos = tiles.GetPositionForLogicalSprite(TILE_HIDDEN_EXIT);
		if (hiddenExitPos.IsPositive())
		{
			tiles.SetSpriteAtPosition(hiddenExitPos, TILE_EXIT);
			tiles.SetLogicalSpriteAtPosition(hiddenExitPos, TILE_EXIT);
		}
	}
}

void Level::ResetLastCutsceneInterval()
{
	scenes[SCENE_TYPE_CUTSCENE].SetSceneLastInterval(GetTimeSinceEpoch());
}

bool Level::HasSpawnedMonstersForLevel() const
{
	return hasSpawnedMonstersForLevel;
}
bool Level::AreThereMonsterSpawnPositions() const
{
	auto spawnPoints = tiles.GetPositionsForLogicalSprite(TILE_MONSTER_SPAWN);
	return spawnPoints.size() > 0;
}

char Level::GetSpriteAtPosition(const Position &tilePos) const
{
	return tiles.GetTileAtPosition(tilePos).sprite;
}

Position Level::GetNearestEntryPosForSprite(char sprite, const Position &spritePos) const
{
	char spriteToSearchFor = ' ';
	Direction dir;
	Position posNextToTP; // used to prevent endless teleporting
	switch (sprite)
	{
	case TILE_GO_RIGHT:
		spriteToSearchFor = TILE_GO_LEFT;
		dir = DIR_RIGHT;
		posNextToTP = Position(1, 0);
		break;
	case TILE_GO_LEFT:
		spriteToSearchFor = TILE_GO_RIGHT;
		dir = DIR_LEFT;
		posNextToTP = Position(-1, 0);
		break;
	case TILE_GO_UP:
		spriteToSearchFor = TILE_GO_DOWN;
		dir = DIR_UP;
		posNextToTP = Position(0, -1);
		break;
	case TILE_GO_DOWN:
		spriteToSearchFor = TILE_GO_UP;
		dir = DIR_DOWN;
		posNextToTP = Position(0, 1);
		break;
	default:
		return Position(-1, -1);
	};

	Tile foundTile = 
		tiles.FindNearestTileToTile(Tile(spriteToSearchFor, spriteToSearchFor, spritePos), dir); 
	foundTile.position.Move(posNextToTP);
	return foundTile.position;
}

const Sprite* Level::GetMapAsSprite() const
{
	if (isShowingEndscene)
	{
		return scenes[SCENE_TYPE_ENDSCENE].GetSprite();
	}
	else if (isShowingNPCscene)
	{
		return scenes[SCENE_TYPE_NPCSCENE].GetSprite();
	}
	else if ( ! hasBegan)
	{
		return scenes[SCENE_TYPE_CUTSCENE].GetSprite();
	}
	else
	{
		return tiles.AsSprite();
	}
}

bool Level::IsPositionInsideMap(const Position &position) const
{
	assert(position.IsPositive()); // Prevents overflow if when converting to size_t. (maybe)

	return (position.x > 0 && position.y > 0 &&
			size_t(position.x) < tiles.GetWidth() && size_t(position.y) < tiles.GetHeight());
}

bool Level::HasBegan() const
{
	return hasBegan;
}

bool Level::HasActiveCutscenes() const
{
	return ( ! hasBegan) || isShowingEndscene || isShowingNPCscene;
}

LevelMap Level::GetMap() const
{
	return tiles;
}
void Level::ShowEndscene()
{
	isShowingEndscene = true;
}

void Level::SetSpriteAtPosition(const Position &position, char newSprite)
{
	assert(position.IsPositive());

	tiles.SetSpriteAtPosition(position, newSprite);
}

void Level::SetTile(const Tile &newTile)
{
	assert(newTile.position.IsPositive());

	tiles.SetTile(newTile);
}

bool Level::IsExitUnblocked() const
{
	return isExitUnblocked;
}

bool Level::IsExitDisplayConditionMet() const
{
	return isExitDisplayConditionMet;
}

void Level::RemoveWorldSpecificTiles()
{
	auto removePositions = tiles.GetPositionsForLogicalSprite(TILE_MONSTER);
	if (tiles.HasTileWithLogicalSprite(TILE_BOSS))
	{
		removePositions.push_back(tiles.GetPositionForLogicalSprite(TILE_BOSS));
	}

	for (auto pos = removePositions.begin(); pos != removePositions.end(); ++pos)
	{
		tiles.SetTile(Tile(TILE_EMPTY, TILE_EMPTY, (*pos)));
	}
}

void Level::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		saveStream.write((char*)&isShowingEndscene, sizeof(bool));
		saveStream.write((char*)&isShowingNPCscene, sizeof(bool));
		saveStream.write((char*)&hasBegan, sizeof(bool));
		saveStream.write((char*)&isExitUnblocked, sizeof(bool));
		saveStream.write((char*)&isExitDisplayConditionMet, sizeof(bool));
		saveStream.write((char*)&hasSpawnedMonstersForLevel, sizeof(bool));
		tiles.Serialize(saveStream);
	}
	else
	{
		Report::UnexpectedError("Can't serialize Level", __LINE__, __FILE__);
	}
}
void Level::Deserialize(std::ifstream &loadStream) 
{
	if (loadStream.is_open())
	{
		loadStream.read((char*)&isShowingEndscene, sizeof(bool));
		loadStream.read((char*)&isShowingNPCscene, sizeof(bool));
		loadStream.read((char*)&hasBegan, sizeof(bool));
		loadStream.read((char*)&isExitUnblocked, sizeof(bool));
		loadStream.read((char*)&isExitDisplayConditionMet, sizeof(bool));
		loadStream.read((char*)&hasSpawnedMonstersForLevel, sizeof(bool));
		tiles.Deserialize(loadStream);
	}
	else
	{
		Report::UnexpectedError("Can't deserialize Level", __LINE__, __FILE__);
	}
}