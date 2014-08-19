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
	time_t sceneLastInterval_s; /// @property used to keep track of the last 
								///			  time the scene was shown.

public:
	Cutscene();

	///
	/// @brief Loads the scene from a file
	///
	void Init(const std::string &sceneFile);

	time_t GetSceneDuration() const;
	time_t GetSceneLastInterval() const;

	const Sprite* GetSprite() const;

	void SetSceneLastInterval(time_t newSceneLastInterval_s);
};


#endif