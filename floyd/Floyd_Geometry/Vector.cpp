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

//Position Position::GetDirection(const Position &other) const
//{
//	if ( ! this->IsEqual(other))
//	{
//		Position direction = Position(other.x - x, other.y - y);
//		int distance = std::sqrt(this->GetDistanceSq(other));
//		int roundedX = std::ceil(direction.x / distance);
//		int roundedY = std::ceil(direction.y / distance);

//		return Position(roundedX, roundedY);
//	}

//	return Position(0, 0);
//}

void Position::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		saveStream.write((char*)&x, sizeof(int));
		saveStream.write((char*)&y, sizeof(int));
	}
	else
	{
		Report::Error("Cannot serialize Position", __LINE__, __FILE__);
	}
}

void Position::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		loadStream.read((char*)&x, sizeof(int));
		loadStream.read((char*)&y, sizeof(int));
	}
	else
	{
		Report::Error("Cannot deserialize Position", __LINE__, __FILE__);
	}
}