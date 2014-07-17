#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite(const Position &newPosition, size_t newWidth, size_t newHeight)
	: position(newPosition), width(newWidth), height(newHeight)
{
}

Position Sprite::GetPosition() const
{
	return position;
}

size_t Sprite::GetWidth() const
{
	return width;
}

size_t Sprite::GetHeight() const
{
	return height;
}

const Texture* Sprite::GetTexture() const
{
	return &texture;
}