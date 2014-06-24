#include "Cutscene.h"
#include "Dirs.h"

#include <fstream>
#include <iostream>
#include <sstream>


Cutscene::Cutscene()
	: scene(0), sceneDuration_s(0), sceneLastInterval_s(0)
{
}

// First line is sceneDuration_s
// Second line is lastSceneInterval_s
void Cutscene::Init(const std::string &sceneFileName)
{
	std::ifstream sceneFile(DIR_WORLD + sceneFileName);

	if (sceneFile.is_open())
	{
		std::string line;
		size_t lineIdx = 0;
		while (std::getline(sceneFile, line))
		{
			if (lineIdx > 1)
			{
				scene.push_back(line);	
			}
			else
			{
				if (lineIdx == 0)
				{
					std::stringstream(line) >> sceneDuration_s;
				}
				else if (lineIdx == 1)
				{
					std::stringstream(line) >> sceneLastInterval_s;
				}
			}
			++lineIdx;
		}
	}
	else
	{
		std::cerr << "Error: Opening cutscene file!\n";
		return;
	}

	sceneFile.close();
}

void Cutscene::Display() const
{
	for (auto line = scene.begin(); line != scene.end(); ++line)
	{
		std::cout << (*line) << '\n';
	}
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