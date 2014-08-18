#include "stdafx.h"
#include "Vector.h"

#include "../Floyd_General/Reporting.h"

#include <assert.h>


bool Position::IsEqual(const Position &other) const
{
	return x == other.x && y == other.y;
}

bool Position::IsPositive() const
{
	return (x >= 0) && (y >= 0);
}

int Position::GetDistanceSq(const Position &other) const
{
	int diffX = x - other.x;
	int diffY = y - other.y;

	return (diffX * diffX + diffY * diffY);
}

void Position::Move(const Position &direction)
{
	// directions must be unit vectors. 
	assert(direction.x <= 1 && direction.x >= -1 &&
		   direction.y <= 1 && direction.y >= -1);

	x += direction.x;
	y += direction.y;
}

Position Position::PositionAfterMove(const Position &direction) const
{
	return Position(x + direction.x, y + direction.y);
}