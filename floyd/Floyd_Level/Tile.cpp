#include "stdafx.h"
#include "Tile.h"

#include "../Floyd_General/Reporting.h"

#include <fstream>


Tile::Tile() : sprite(' '), logicalSprite(' '), position() {}

Tile::Tile(char newSprite, char newLogicalSprite, const Position &newPosition) 
	: sprite(newSprite), logicalSprite(newLogicalSprite), position(newPosition) {}


bool Tile::IsValid() const
{
	return position.IsPositive();
}

void Tile::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		saveStream.write((char*)&sprite, sizeof(char));
		saveStream.write((char*)&logicalSprite, sizeof(char));
		SerializePosition(position, saveStream);
	}
	else
	{
		Report::UnexpectedError("Can't serialize Tile", __LINE__, __FILE__);
	}
}
void Tile::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		loadStream.read((char*)&sprite, sizeof(char));
		loadStream.read((char*)&logicalSprite, sizeof(char));
		DeserializePosition(position, loadStream);
	}
	else
	{
		Report::UnexpectedError("Can't deserialize Tile", __LINE__, __FILE__);
	}
}