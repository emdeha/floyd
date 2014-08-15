#include "stdafx.h"
#include "Sprite.h"

#include "../Floyd_General/Reporting.h"
#include "../Floyd_General/Utils.h"
#include "../Floyd_General/Dirs.h"

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

void Sprite::Serialize(std::ofstream &saveFile) const
{
	auto *textureData = texture.GetData();
	size_t dataSize = textureData->size();
	saveFile.write((char*)&dataSize, sizeof(dataSize));
	for (auto line = textureData->begin(); line != textureData->end(); ++line)
	{
		size_t lineSize = line->size() + 2; // Reserve space for the \n\0 sequence.
		saveFile.write((char*)&lineSize, sizeof(lineSize));
		saveFile.write(line->c_str(), (lineSize - 2) * sizeof(char));
		saveFile.write("\n\0", 2 * sizeof(char)); // This sequence determines the end of a sprite-line.
	}

	saveFile.write((char*)&width, sizeof(width));
	saveFile.write((char*)&height, sizeof(height));
}
void Sprite::Deserialize(std::ifstream &loadFile)
{
	size_t dataSize = 0;
	loadFile.read((char*)&dataSize, sizeof(dataSize));

	std::string data("");
	for (size_t idx = 0; idx < dataSize; ++idx)
	{
		size_t lineSize = 0;
		loadFile.read((char*)&lineSize, sizeof(lineSize));
		char *line = new char[lineSize];
		loadFile.read(line, lineSize * sizeof(char));
		data.append(line);
		delete []line;
	}

	texture.LoadRawData(data);

	loadFile.read((char*)&width, sizeof(width));
	loadFile.read((char*)&height, sizeof(height));
}