#include "Menu.h"
#include "World.h"

#include <fstream>
#include <iostream>


Menu::Menu() : buttons(0)
{
}

///
/// Menu file:
///	btnName, btnLabel
///
void Menu::Init(const std::string &menuFile)
{
	std::ifstream menu(menuFile);

	if (menu.is_open())
	{
		std::string line;
		while (std::getline(menu, line).good())
		{
			size_t delimPos = line.find(',');
			if (delimPos != line.npos)
			{
				std::string btnName = line.substr(0, delimPos);
				std::string btnLabel = line.substr(delimPos + 1, line.length());
				Button newButton(btnName, btnLabel);
				buttons.push_back(newButton);
			}
			else
			{
				std::cerr << "Error: Parsing menu loading file\n";
			}
		}
	}

	menu.close();
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
	std::cout << "Pressed key: " << key << std::endl;
}