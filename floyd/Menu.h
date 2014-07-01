#ifndef FLOYD_MENU_H
#define FLOYD_MENU_H


#include <vector>
#include <string>

#include "Button.h"


class World;

class Menu
{
private:
	std::vector<Button> buttons;

public:
	Menu();

	void Init(const std::string &menuFile);

	void Display() const;
	
	void OnKeyPressed(char key, World *world);
};


#endif