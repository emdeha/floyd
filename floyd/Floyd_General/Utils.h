#ifndef FLOYD_UTILS_H
#define FLOYD_UTILS_H


#include <assert.h>
#include <time.h>
#include <string>
#include <Windows.h>

#include "../Floyd_Geometry/Vector.h"


// Hate these tabulations but it may be clearer for the eye.
enum Tiles
{
	// World-specific
	TILE_STASH			 = 'O',
	TILE_SHRINE			 = 'I',
	TILE_HERO			 = '|',
	TILE_NPC			 = 'N',
	TILE_MONSTER		 = 'M',
	TILE_BOSS			 = 'B',
	TILE_PARTICLE		 = '.',

	// Level-specific
	TILE_EMPTY			 = ' ',

	TILE_MONSTER_SPAWN	 = 'm',
	TILE_KILL_BLOCK		 = ',',

	TILE_WALL			 = '#',
	TILE_START			 = 'S',
	TILE_EXIT_BLOCK		 = '@',
	TILE_HIDDEN_EXIT	 = 'e',
	TILE_EXIT			 = 'E',
	TILE_DREAMS			 = '*',
	TILE_TELEPORT		 = 'T',

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
	KEY_ESC				 = 27,

	KEY_P				 = 'p',
	KEY_USE_SKILL		 = 'r',

	KEY_ZERO			 = '0',
	KEY_ONE				 = '1',
	KEY_TWO				 = '2',
	KEY_THREE			 = '3',
	KEY_FOUR			 = '4',
	KEY_FIVE			 = '5',
	KEY_SIX				 = '6',
	KEY_SEVEN			 = '7',
	KEY_EIGHT			 = '8',
	KEY_NINE			 = '9',

};

enum LINES
{
	LINE_HEALTH = 0,
	LINE_DAMAGE = 1,
	LINE_DEFENSE = 2,
	LINE_STATSPRITE_FILE = 3,
};

const size_t BOSS_LEVEL = 6;


enum Direction
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
};

///
/// @brief Gets time since UNIX epoch in **seconds**.
///
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

///
/// @breaf Removes all occurences of a given character from a given string
///
std::string RemoveChar(char ch, const std::string &str);

///
/// @brief Trims a string
///
std::string Trim(const std::string &str);


#endif