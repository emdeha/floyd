#ifndef FLOYD_CUTSCENE_H
#define FLOYD_CUTSCENE_H


#include <vector>
#include <string>


class Cutscene 
{
private:
	typedef std::vector<std::string> SceneMatrix;

	SceneMatrix scene;
	time_t sceneDuration_s;
	time_t sceneLastInterval_s;

public:
	Cutscene();

	void Init(const std::string &sceneFile);

	void Display() const;

	time_t GetSceneDuration() const;
	time_t GetSceneLastInterval() const;

	void SetSceneLastInterval(time_t newSceneLastInterval_s);
};


#endif