#ifndef FLOYD_UTILS_H
#define FLOYD_UTILS_H


#include <assert.h>
#include <time.h>
#include <Windows.h>


enum Tiles
{
	TILE_EMPTY = ' ',
	TILE_START = 'S',
	TILE_EXIT = 'E',
	TILE_TELEPORT = 'T',
	TILE_MONSTER = 'M',
	TILE_WALL = '#',
	TILE_STASH = 'O',
	TILE_SHRINE = 'I',
	TILE_BOSS = 'B',
	TILE_DREAMS = '*',
	TILE_PARTICLE = '.',
	TILE_HERO = '|',
	TILE_EXIT_BLOCK = '@',
	TILE_HIDDEN_EXIT = 'e',
	TILE_MONSTER_SPAWN = 'm',
	TILE_NPC = 'N',
};


struct Position 
{
	int x;
	int y;

	Position() : x(0), y(0) {}
	Position(int newX, int newY) : x(newX), y(newY) {}

	bool IsEqual(const Position &other) const;

	bool IsPositive() const;

	void Move(const Position &direction);
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

void ClearHandleScreen(HANDLE handle);


#endif