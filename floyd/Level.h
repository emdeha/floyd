#ifndef FLOYD_LEVEL_H
#define FLOYD_LEVEL_H


#include <string>
#include <vector>
#include <map>

#include "Floyd_General/Utils.h"
#include "Floyd_Geometry/Vector.h"
#include "Cutscene.h"
#include "Floyd_Level/Tile.h"


/// Provides functionality for easier interaction with the map.
class LevelMap
{
private:
	std::vector<Tile> map;
	mutable Sprite mapAsSprite;
	std::map<char, char> spriteForLogicalSprite;

	int width;
	int height;

public:
	LevelMap(); 

	void Init(const std::string &levelFile);

public:
	Tile GetTileAtPosition(const Position &position) const;
	void SetSpriteAtPosition(const Position &position, char sprite);
	void SetLogicalSpriteAtPosition(const Position &position, char logicalSprite);

	///
	/// @brief Replaces the tile at the position of `newTile`
	///
	void SetTile(const Tile &newTile);

	Position GetPositionForLogicalSprite(char logicalSprite) const;

	void SetSpriteForLogicalSprite(char newSprite, char logicalSprite);

	std::vector<Position> GetPositionsForLogicalSprite(char logicalSprite) const;
	std::vector<Tile> GetTilesForLogicalSprite(char logicalSprite) const;

	Tile FindNearestTileToTile(const Tile &tileOther, Direction dir) const;

	bool HasTileWithLogicalSprite(char logicalSprite) const;

	std::string GetRawMap() const;
	const Sprite* AsSprite() const;

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);

public:
	size_t GetWidth() const;
	size_t GetHeight() const;

private:
	void InitSpriteForLogicalSprite();
	void InitMap(const std::string &levelFile);
};


enum SceneType
{
	SCENE_TYPE_CUTSCENE,
	SCENE_TYPE_ENDSCENE,
	SCENE_TYPE_NPCSCENE,

	SCENE_TYPE_SIZE,

	SCENE_TYPE_NONE = -1,
};


class World;

typedef std::vector<std::string> LevelMatrix;

class Level
{
private:
	std::string name;

	LevelMap tiles;
	Sprite mapAsSprite;

	// Currently supporting one cutscene per level
	Cutscene scenes[SCENE_TYPE_SIZE]; // Collects all types of cutscenes
	
private:
	bool isShowingEndscene;
	bool isShowingNPCscene;
	bool hasBegan;

private:
	bool isExitUnblocked;
	bool isExitDisplayConditionMet;
	bool hasSpawnedMonstersForLevel;
	bool hasSpawnPositions;

public:
	Level();

	void Init(const std::string &levelFile);
	void InitCutscenes(const std::vector<std::string> &cutsceneFileNames);

	void Update();

	Position GetStartingPos() const;
	LevelMap GetMap() const;
	void ShowEndscene();
	void ShowNPCscene();

	void SetSpriteAtPosition(const Position &position, char newSprite);
	void SetTile(const Tile &newTile);
	char GetSpriteAtPosition(const Position &tilePos) const;

	bool IsExitUnblocked() const;

	bool IsExitDisplayConditionMet() const;
	void SetIsExitDisplayConditionMet(bool newIsExitDisplayConditionMet);

	void ResetLastCutsceneInterval();

	bool HasSpawnedMonstersForLevel() const;
	bool AreThereMonsterSpawnPositions() const;

	Position GetNearestEntryPosForSprite(char sprite, const Position &spritePos) const;

	const Sprite* GetMapAsSprite() const;

	bool IsPositionInsideMap(const Position &position) const;

	bool HasBegan() const;
	bool HasActiveCutscenes() const;

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);

public:
	void UnblockExit();
	void ShowTeleport();
	void SpawnMonsters(World *world);
	void RemoveWorldSpecificTiles();
};


#endif