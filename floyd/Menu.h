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
	
	void OnKeyPressed(char key, World *world);

public:
	void ShowButton(const std::string &buttonName);
	void HideButton(const std::string &buttonName);

public:
	// Used to get pairs of buttons with their order of appearance from top to bottom.
	std::vector<std::pair<int, const Button*>> GetButtonsForDrawing() const;
};


#endif