#ifndef FLOYD_VECTOR_H
#define FLOYD_VECTOR_H


#include <fstream>


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

	// Should these be a vector's responsibility?
	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
};


#endif