#ifndef FLOYD_UTILS_H
#define FLOYD_UTILS_H


#include <assert.h>
#include <time.h>
#include <Windows.h>


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