#ifndef FLOYD_SPRITE_H
#define FLOYD_SPRITE_H


#include "../Floyd_Geometry/Vector.h"
#include "Texture.h"

class Sprite
{
private:
	size_t width;
	size_t height;
	Texture texture;

public:
	Sprite();
	Sprite(size_t newWidth, size_t newHeight);

	void LoadTexture(const std::string &fileName);
	void LoadTextureFromRawData(const std::string &rawData);

	size_t GetWidth() const;
	size_t GetHeight() const;
	const Texture* GetTexture() const;

public:
	void Serialize(std::ofstream &saveFile) const;
	void Deserialize(std::ifstream &loadFile);
};


#endif