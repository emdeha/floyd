#include "stdafx.h"
#include "Sprite.h"
#include "../Reporting.h"
#include "../Utils.h"
#include "../Dirs.h"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>


Sprite::Sprite(const Position &newPosition, size_t newWidth, size_t newHeight)
	: position(newPosition), width(newWidth), height(newHeight)
{
}

void Sprite::LoadTexture(const std::string &fileName)
{
	texture.LoadData(fileName);
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