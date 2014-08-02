#ifndef FLOYD_TILE_H
#define FLOYD_TILE_H


#include "../Utils.h"


struct Tile
{
public:
	char sprite;

	/// Used in situations for which the looks of the tile don't determine
	/// what happenes when you interact with it.
	char logicalSprite; 

	Position position;

public:
	Tile();
	Tile(char newSprite, char newLogicalSprite, const Position &newPosition); 

	bool IsValid() const;

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);
};


#endif