#include "stdafx.h"
#include "Texture.h"
#include "../Floyd_General/Reporting.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>


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

void Texture::LoadRawData(const std::string &rawData)
{
	std::istringstream issData(rawData);
	std::string line;
	while (std::getline(issData, line).good())
	{
		data.push_back(line);
	}
}

const std::vector<std::string>* Texture::GetData() const
{
	return &data;
}