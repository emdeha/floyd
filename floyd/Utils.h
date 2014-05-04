#ifndef FLOYD_UTILS_H
#define FLOYD_UTILS_H


struct Position 
{
	int x;
	int y;

	Position(int newX, int newY) : x(newX), y(newY) {}

	bool IsEqual(const Position &other)
	{
		return x == other.x && y == other.y;
	}
};

enum Direction
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
};


#endif