#include "stdafx.h"
#include "Texture.h"

#include <assert.h>


Texture::Texture()
{
	data = nullptr;
}

void Texture::LoadData(std::stringstream &dataStream)
{
	data = new char[dataStream.str().length() + 1];
	dataStream >> data;
}

char* Texture::GetData() const
{
	assert(data);

	return data;
}

Texture::~Texture()
{
	delete [] data;
}