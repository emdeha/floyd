#ifndef FLOYD_BUTTON_H
#define FLOYD_BUTTON_H


#include <string>
#include "../Floyd_Graphics/Sprite.h"


class World;

typedef void (*OnClickCallback)(World*);

class Button
{
private:
	std::string name;
	std::string label;
	Sprite sprite; // this is a composition from a label and a possible background
	char key;
	bool isHidden;
	OnClickCallback onClick;

public:
	Button();
	Button(const std::string &newName, const std::string &newLabel, char newKey, bool newIsHidden);

	void Init();

	void SetOnClickCallback(OnClickCallback newCallback);

	void Display() const;

	void OnKeyPressed(World *world);

public:
	void SetIsHidden(bool newIsHidden);

	std::string GetName() const;
	char GetKey() const;
	bool IsHidden() const;
	const Sprite* GetSprite() const;
};


#endif