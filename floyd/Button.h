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
	OnClickCallback onClick;

public:
	Button();
	Button(const std::string &newName, const std::string &newLabel);

	void SetOnClickCallback(OnClickCallback newCallback);

	void Display() const;

	void OnKeyPressed(World *world);
};


#endif