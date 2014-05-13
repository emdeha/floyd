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

	char prevCharacter;
	
private:
	bool isShowingEndscene;
	bool isShowingNPCscene;
	bool hasBegan;

private:
	time_t npcSceneDuration_s;
	time_t lastNpcSceneInterval_s;
	time_t cutsceneDuration_s;
	time_t lastCutsceneInterval_s;

private:
	mutable HANDLE drawBuffer;
	mutable HANDLE setBuffer;

private:
	Position lastFrameHeroPos;
	
public:
	Level() : name(""), map(0), cutscene(0), endscene(0), npcscene(0), prevCharacter(' '), 
			  hasBegan(false), isShowingEndscene(false), isShowingNPCscene(false),
			  npcSceneDuration_s(5), cutsceneDuration_s(10), lastFrameHeroPos(0,0)
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

		lastNpcSceneInterval_s = GetTimeSinceEpoch();
		lastCutsceneInterval_s = GetTimeSinceEpoch();
	}

	void Init(const std::string &levelFile);
	void InitCutscenes(const std::vector<std::string> &cutsceneFileNames);

	// Maybe it shouldn't be const
	void Display();
	void UpdateLevelMatrix(const World *world); 

	Position GetStartingPos() const;
	LevelMatrix GetMap() const;
	void ShowEndscene();
	void ShowNPCscene();

	void SetTileAtPosition(Position position, char newTile);

private:
	// TODO: Clear duplicate code
	// TODO: Adding scenes can be more flexible
	void AddCutscene(const std::string &cutsceneFile);
	void AddEndscene(const std::string &endsceneFile);
	void AddNPCscene(const std::string &npcsceneFile);

	void BeginSwapBuffers() const;
	void EndSwapBuffers() const;
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
	map[position.y][position.x] = newTile;
}


#endif