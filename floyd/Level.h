#ifndef FLOYD_LEVEL_H
#define FLOYD_LEVEL_H


#include <string>
#include <vector>

#include "Utils.h"
//#include "ISerializable.h"


class World;

typedef std::vector<std::string> LevelMatrix;

class Level //: private ISerializable
{
private:
	std::string name;

	LevelMatrix map;
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

	std::vector<Position> monsterSpawnPoints;
	Position exitBlockPos;
	Position teleportPos;
	Position hiddenExitPos;

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

	void Display();
	void UpdateLevelMatrix(World *world); 

	Position GetStartingPos() const;
	LevelMatrix GetMap() const;
	void ShowEndscene();
	void ShowNPCscene();

	void SetTileAtPosition(Position position, char newTile);

	bool IsExitUnblocked() const;

	bool IsExitDisplayConditionMet() const;
	void SetIsExitDisplayConditionMet(bool newIsExitDisplayConditionMet);

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

	// TODO: Put in something that'll manage screen
	void BeginSwapBuffers() const;
	void EndSwapBuffers() const;
	
	//
	void GetSpawnPositionsFromLine(const std::string &line, int preferredY);
};

inline LevelMatrix Level::GetMap() const
{
	return map;
}
inline void Level::ShowEndscene()
{
	isShowingEndscene = true;
}
inline void Level::ShowNPCscene()
{
	isShowingNPCscene = true;
}

inline void Level::SetTileAtPosition(Position position, char newTile)
{
	assert(position.y >= 0 && position.x >= 0);

	map[position.y][position.x] = newTile;
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