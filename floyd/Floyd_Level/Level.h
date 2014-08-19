#ifndef FLOYD_LEVEL_H
#define FLOYD_LEVEL_H


#include <string>
#include <vector>
#include <map>

#include "../Floyd_General/Utils.h"
#include "../Floyd_Geometry/Vector.h"
#include "Cutscene.h"
#include "Tile.h"


/// Provides functionality for easier interaction with the map.
class LevelMap
{
private:
	std::vector<Tile> map;
	mutable Sprite mapAsSprite; /// @property used to get the map for drawing
								///			  without making a copy.
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

	///
	/// @brief Gets the positions of the Tiles which have logicalSprite equal to the parameter
	///
	std::vector<Position> GetPositionsForLogicalSprite(char logicalSprite) const;
	///
	/// @brief Gets the Tiles which have logicalSprite equal to the paramter.
	///
	std::vector<Tile> GetTilesForLogicalSprite(char logicalSprite) const;

	///
	/// @brief Finds the nearest corresponding teleport tile ('}','{','v','^')
	///
	Tile FindNearestTileToTile(const Tile &tileOther, Direction dir) const;

	bool HasTileWithLogicalSprite(char logicalSprite) const;

	///
	/// @brief Converts map to raw std::string. Each line eds in '\n'.
	///
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

	///
	/// @brief Gets the position of the tile from which the level starts
	///
	Position GetStartingPos() const;
	LevelMap GetMap() const;
	void ShowEndscene();
	void ShowNPCscene();

	void SetSpriteAtPosition(const Position &position, char newSprite);
	char GetSpriteAtPosition(const Position &tilePos) const;
	void SetTile(const Tile &newTile);

	bool IsExitUnblocked() const;

	bool IsExitDisplayConditionMet() const;
	void SetIsExitDisplayConditionMet(bool newIsExitDisplayConditionMet);

	void ResetLastCutsceneInterval();

	bool HasSpawnedMonstersForLevel() const;
	bool AreThereMonsterSpawnPositions() const;

	///
	/// @brief Gets the position of the nearest corresponding teleport sprite ('{','}','v','^')
	///
	Position GetNearestEntryPosForSprite(char sprite, const Position &spritePos) const;

	///
	/// @brief Gets the sprite of the current shown cutscene or level map
	///
	const Sprite* GetMapAsSprite() const;

	bool IsPositionInsideMap(const Position &position) const;

	bool HasBegan() const;
	bool HasActiveCutscenes() const;

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);

public:
	void UnblockExit();
	void ShowTeleport();
	///
	/// @brief Goes through tiles with logical sprite TILE_MONSTER_SPAWN
	///		   and spawns monsters at their positions.
	///
	void SpawnMonsters(World *world);
	///
	/// @brief Sets the dynamic world specific tiles (see Utils.h) to empty
	///
	void RemoveWorldSpecificTiles();
};


#endif