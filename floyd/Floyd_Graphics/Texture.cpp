#include "stdafx.h"
#include "Texture.h"
#include "../Reporting.h"

#include <assert.h>
#include <iostream>
#include <fstream>


Texture::Texture()
{
}

void Texture::LoadData(const std::string &fileName)
{
	std::ifstream textureFile(fileName);
	
	if (textureFile.is_open())
	{
		std::string line;
		while (std::getline(textureFile, line).good())
		{ 
			data.push_back(line);
		}
	}
	else
	{
		Report::Error("Cannot load texture", __LINE__, __FILE__);
	}

	textureFile.close();
}

const std::vector<std::string>* Texture::GetData() const
{
	return &data;
}