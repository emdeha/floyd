#include "Menu.h"
#include "World.h"


Menu::Menu() : buttons(0)
{
}

void Menu::Init(const std::string &menuFile)
{
}

void Menu::Display() const
{
	for (auto button = buttons.begin(); button != buttons.end(); ++button)
	{
		button->Display();
	}
}

void Menu::OnKeyPressed(char key, World *world)
{
	// Call the button's callback.
}