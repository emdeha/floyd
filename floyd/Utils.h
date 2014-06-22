#ifndef FLOYD_UTILS_H
#define FLOYD_UTILS_H


#include <assert.h>
#include <time.h>
#include <string>
#include <Windows.h>


// Hate these tabulations but it may be clearer for the eye.
enum Tiles
{
	TILE_EMPTY			 = ' ',

	TILE_START			 = 'S',
	TILE_EXIT_BLOCK		 = '@',
	TILE_HIDDEN_EXIT	 = 'e',
	TILE_EXIT			 = 'E',
	TILE_DREAMS			 = '*',
	TILE_TELEPORT		 = 'T',

	TILE_MONSTER		 = 'M',
	TILE_BOSS			 = 'B',
	TILE_MONSTER_SPAWN	 = 'm',
	TILE_PARTICLE		 = '.',
	TILE_KILL_BLOCK		 = ',',

	TILE_WALL			 = '#',
	TILE_STASH			 = 'O',
	TILE_SHRINE			 = 'I',
	TILE_HERO			 = '|',
	TILE_NPC			 = 'N',

	TILE_GO_DOWN		 = 'v',
	TILE_GO_UP			 = '^',
	TILE_GO_LEFT		 = '{',
	TILE_GO_RIGHT		 = '}',
};

enum Keys
{
	KEY_UP				 = 'w',
	KEY_DOWN			 = 's',
	KEY_RIGHT			 = 'd',
	KEY_LEFT			 = 'a',
	KEY_KILL_ALL		 = 'k',
	KEY_QUIT			 = 'q',
};

enum LINES
{
	LINE_HEALTH = 0,
	LINE_DAMAGE = 1,
	LINE_DEFENSE = 2,
	LINE_STATSPRITE_FILE = 3,
};


struct Position 
{
	int x;
	int y;

	Position() : x(0), y(0) {}
	Position(int newX, int newY) : x(newX), y(newY) {}

	bool IsEqual(const Position &other) const;
	bool IsPositive() const;

	int GetDistanceSq(const Position &other) const;

	void Move(const Position &direction);

	Position PositionAfterMove(const Position &direction) const;

	//Position GetDirection(const Position &other) const;
};

enum Direction
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
};

time_t GetTimeSinceEpoch();

int GetRandomInRange(int min, int max);
Position GetRandomDirection();

void ClearHandleScreen(HANDLE handle);

///
/// @brief Returns a file name with path to it and floyd extension
/// @example:
///		ResolveFileName("world", "data/world/") -> "data/world/" + "world" + ".fl"
///												-> "data/world/world.fl"
///
std::string ResolveFileName(const std::string &fileName, const std::string &relativePath);

// Slow but does the job. 
// Not locale independent.
template<typename T>
bool SafeLexicalCast(const std::string &str, T &other)
{
	std::istringstream cast_ss(str);
	if (cast_ss >> other)
	{
		return true;
	}
	else
	{
		std::cerr << "Error: Lexical cast not possible\n";
		return false;
	}
}

///
/// @brief Gets a health bar based on health percentage
///
std::string GetHealthBar(float healthQuotient);


#endif