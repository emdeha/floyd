#include "stdafx.h"
#include "Cutscene.h"

#include "../Floyd_General/Dirs.h"
#include "../Floyd_General/Reporting.h"

#include <fstream>
#include <iostream>
#include <sstream>


Cutscene::Cutscene()
	: scene(), sceneDuration_s(0), sceneLastInterval_s(0)
{
}

// First line is sceneDuration_s
void Cutscene::Init(const std::string &sceneFileName)
{
	std::ifstream sceneFile(DIR_WORLD + sceneFileName);

	if (sceneFile.is_open())
	{
		std::string line;
		size_t lineIdx = 0;
		std::string sceneRaw; 
		size_t width = 0;
		size_t height = 0;
		while (std::getline(sceneFile, line))
		{
			if (lineIdx > 0)
			{
				sceneRaw.append(line);
				sceneRaw.push_back('\n');
				++height;
				if (line.length() > width)
				{
					width = line.length();
				}
			}
			else
			{
				if (lineIdx == 0)
				{
					std::stringstream(line) >> sceneDuration_s;
				}
			}
			++lineIdx;
		}

		scene = Sprite(width, height);
		scene.LoadTextureFromRawData(sceneRaw);
	}
	else
	{
		Report::Error("Opening cutscene file!\n", __LINE__, __FILE__);
		return;
	}

	sceneFile.close();
}

const Sprite* Cutscene::GetSprite() const
{
	return &scene;
}

time_t Cutscene::GetSceneDuration() const
{
	return sceneDuration_s;
}
time_t Cutscene::GetSceneLastInterval() const
{
	return sceneLastInterval_s;
}

void Cutscene::SetSceneLastInterval(time_t newSceneLastInterval_s)
{
	sceneLastInterval_s = newSceneLastInterval_s;
}