#ifndef FLOYD_UTILS_H
#define FLOYD_UTILS_H


#include <assert.h>


struct Position 
{
	int x;
	int y;

	Position(int newX, int newY) : x(newX), y(newY) {}

	bool IsEqual(const Position &other)
	{
		return x == other.x && y == other.y;
	}

	void Move(const Position &direction)
	{
		// directions must be unit vectors. 
		assert(direction.x <= 1 && direction.x >= -1 &&
			   direction.y <= 1 && direction.y >= -1);

		x += direction.x;
		y += direction.y;
	}
};

enum Direction
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
};

typedef double time_s;


#endif