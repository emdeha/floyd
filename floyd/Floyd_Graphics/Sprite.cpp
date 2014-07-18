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


Sprite::Sprite()
	: width(0), height(0)
{
}
Sprite::Sprite(size_t newWidth, size_t newHeight)
	: width(newWidth), height(newHeight)
{
}

void Sprite::LoadTexture(const std::string &fileName)
{
	texture.LoadData(fileName);
}

void Sprite::LoadTextureFromRawData(const std::string &rawData)
{
	texture.LoadRawData(rawData);
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