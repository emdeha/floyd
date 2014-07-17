#ifndef FLOYD_SPRITE_H
#define FLOYD_SPRITE_H


#include "../Floyd_Geometry/Vector.h"
#include "Texture.h"

class Sprite
{
private:
	Position position;
	size_t width;
	size_t height;
	Texture texture;

public:
	Sprite(const Position &newPosition, size_t newWidth, size_t newHeight);

	void LoadTexture(const std::string &fileName);

	Position GetPosition() const;
	size_t GetWidth() const;
	size_t GetHeight() const;
	const Texture* GetTexture() const;
};


#endif