#ifndef FLOYD_LEVEL_H
#define FLOYD_LEVEL_H


#include <string>
#include <vector>
#include <map>

#include "Utils.h"
#include "Floyd_Geometry/Vector.h"
#include "Cutscene.h"


struct Tile
{
public:
	char sprite;

	/// Used in situations for which the looks of the tile don't determine
	/// what happenes when you interact with it.
	char logicalSprite; 

	Position position;

public:
	Tile();
	Tile(char newSprite, char newLogicalSprite, const Position &newPosition); 

	bool IsValid() const;

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
};

/// Provides functionality for easier interaction with the map.
class LevelMap
{
private:
	std::vector<Tile> map;
	std::map<char, char> spriteForLogicalSprite;

	int width;
	int height;

public:
	LevelMap(); 

	void Init(const std::string &levelFile);

	void Display() const;

public:
	Tile GetTileAtPosition(const Position &position) const;
	void SetSpriteAtPosition(const Position &position, char sprite);
	void SetLogicalSpriteAtPosition(const Position &position, char logicalSprite);

	void SetTileAtPosition(const Position &position, const Tile &newTile);

	Position GetPositionForLogicalSprite(char sprite) const;

	void SetSpriteForLogicalSprite(char newSprite, char logicalSprite);

	std::vector<Position> GetPositionsForLogicalSprite(char logicalSprite) const;
	std::vector<Tile> GetTilesForLogicalSprite(char logicalSprite) const;

	Tile FindNearestTileToTile(const Tile &tileOther, Direction dir) const;

	bool HasTileWithLogicalSprite(char logicalSprite) const;

	std::string GetRawMap() const;

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

private:
	Position lastFrameHeroPos;
	
public:
	Level();

	void Init(const std::string &levelFile);
	void InitCutscenes(const std::vector<std::string> &cutsceneFileNames);

	void Display(World *world);
	void Update();
	void UpdateLevelMatrix(World *world); 

	Position GetStartingPos() const;
	LevelMap GetMap() const;
	void ShowEndscene();
	void ShowNPCscene();

	void SetSpriteAtPosition(const Position &position, char newSprite);
	void SetTileAtPosition(const Position &position, const Tile &newTile);
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
};


#endif