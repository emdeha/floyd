#ifndef FLOYD_VECTOR_H
#define FLOYD_VECTOR_H


struct Position 
{
public:
	int x;
	int y;

public:
	Position() : x(0), y(0) {}
	Position(int newX, int newY) : x(newX), y(newY) {}

	///
	/// @brief Checks if two positions have equal x and y properties.
	///
	bool IsEqual(const Position &other) const;
	///
	/// @brief Checks if both x and y of a position are positive.
	///
	bool IsPositive() const;

	///
	/// @brief Gets the **square** distance between two positions.
	///
	int GetDistanceSq(const Position &other) const;

	///
	/// @brief Moves a position by a given direction.
	///
	void Move(const Position &direction);

	///
	/// @brief Gets the position which we'll have after we move by a given direction.
	///
	Position PositionAfterMove(const Position &direction) const;
};


#endif