#ifndef FLOYD_BUTTON_H
#define FLOYD_BUTTON_H


#include <string>


class World;

typedef void (*OnClickCallback)(World*);

class Button
{
private:
	std::string name;
	std::string label;
	bool isHidden;
	OnClickCallback onClick;

public:
	Button();
	Button(const std::string &newName, const std::string &newLabel, bool newIsHidden);

	void SetOnClickCallback(OnClickCallback newCallback);

	void Display() const;

	void OnKeyPressed(World *world);

public:
	void SetIsHidden(bool newIsHidden);

	std::string GetName() const;
};


#endif