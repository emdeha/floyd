#ifndef FLOYD_CUTSCENE_H
#define FLOYD_CUTSCENE_H


#include <vector>
#include <string>

#include "../Floyd_Graphics/Sprite.h"


class Cutscene 
{
private:
	Sprite scene;
	time_t sceneDuration_s;
	time_t sceneLastInterval_s;

public:
	Cutscene();

	void Init(const std::string &sceneFile);

	void Display() const;

	time_t GetSceneDuration() const;
	time_t GetSceneLastInterval() const;

	const Sprite* GetSprite() const;

	void SetSceneLastInterval(time_t newSceneLastInterval_s);
};


#endif