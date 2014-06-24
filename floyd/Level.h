#ifndef FLOYD_LEVEL_H
#define FLOYD_LEVEL_H


#include <string>
#include <vector>
#include <map>

#include "Utils.h"
//#include "ISerializable.h"


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

public:
	size_t GetWidth() const;
	size_t GetHeight() const;

private:
	void InitSpriteForLogicalSprite();
	void InitMap(const std::string &levelFile);
};


//enum SceneType
//{
//	SCENE_TYPE_CUTSCENE = 1,
//	SCENE_TYPE_ENDSCENE = 2,
//	SCENE_TYPE_NPCSCENE = 3,

//	SCENE_TYPE_NONE = 0,
//};


class World;

typedef std::vector<std::string> LevelMatrix;

class Level //: private ISerializable
{
private:
	std::string name;

	LevelMap tiles;

	// Currently supporting one cutscene per level
	LevelMatrix cutscene;
	LevelMatrix endscene;
	LevelMatrix npcscene;
	
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
	time_t npcSceneDuration_s;
	time_t lastNpcSceneInterval_s;
	time_t cutsceneDuration_s;
	time_t lastCutsceneInterval_s;

private:
	// TODO: Put in sth that'll manage screen
	mutable HANDLE drawBuffer;
	mutable HANDLE setBuffer;

private:
	Position lastFrameHeroPos;
	
public:
	Level();

	void Init(const std::string &levelFile);
	void InitCutscenes(const std::vector<std::string> &cutsceneFileNames);

	void Display(World *world);
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

	bool IsPositionInsideMap(const Position &position) const;

	bool HasBegan() const;

public:
	void UnblockExit();
	void ShowTeleport();
	void SpawnMonsters(World *world);

private:
	// TODO: Clear duplicate code
	// TODO: Adding scenes can be more flexible
	void AddCutscene(const std::string &cutsceneFile);
	void AddEndscene(const std::string &endsceneFile);
	void AddNPCscene(const std::string &npcsceneFile);

	//void AddScene(const std::string &fileName, SceneType type);

	// TODO: Put in something that'll manage screen
	void BeginSwapBuffers() const;
	void EndSwapBuffers() const;
};

inline LevelMap Level::GetMap() const
{
	return tiles;
}
inline void Level::ShowEndscene()
{
	isShowingEndscene = true;
}

inline void Level::SetSpriteAtPosition(const Position &position, char newSprite)
{
	assert(position.IsPositive());

	tiles.SetSpriteAtPosition(position, newSprite);
}

inline void Level::SetTileAtPosition(const Position &position, const Tile &newTile)
{
	assert(position.IsPositive());

	tiles.SetTileAtPosition(position, newTile);
}

inline bool Level::IsExitUnblocked() const
{
	return isExitUnblocked;
}

inline bool Level::IsExitDisplayConditionMet() const
{
	return isExitDisplayConditionMet;
}


#endif