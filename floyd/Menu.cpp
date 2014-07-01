#include "Menu.h"
#include "World.h"
#include "Scripts.h"

#include <fstream>
#include <iostream>


void SetScriptForButton(Button *button, const std::string &scriptName)
{
	if (scriptName == "newGame")
	{
		button->SetOnClickCallback(ButtonScripts::NewGameOnClick);
	}
	else if (scriptName == "loadGame")
	{
		button->SetOnClickCallback(ButtonScripts::LoadGameOnClick);
	}
	else if (scriptName == "saveGame")
	{
		button->SetOnClickCallback(ButtonScripts::SaveGameOnClick);
	}
	else if (scriptName == "quit")
	{
		button->SetOnClickCallback(ButtonScripts::QuitGameOnClick);
	}
	else
	{
		std::cerr << "Error: Invalid script name: '" << scriptName << "'\n";
	}
}

////////////
//  Menu  //
////////////

Menu::Menu()
{
}

///
/// Menu file:
///	btnName, btnLabel, btnKey
/// UNSAFE!!!
///
void Menu::Init(const std::string &menuFile)
{
	std::ifstream menu(menuFile);

	if (menu.is_open())
	{
		std::string line;
		while (std::getline(menu, line).good())
		{
			size_t firstDelimPos = line.find(',');
			if (firstDelimPos != line.npos)
			{
				std::string btnName = line.substr(0, firstDelimPos);
				size_t secondDelimPos = line.find_last_of(',');
				std::string btnLabel = line.substr(firstDelimPos + 1, secondDelimPos - firstDelimPos - 1);
				char btnKey = line.substr(secondDelimPos + 1, line.length())[0];
				Button newButton(btnName, btnLabel);

				SetScriptForButton(&newButton, btnName);

				buttonsWithKeys.insert(std::make_pair(btnKey, newButton));
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
	for (auto button = buttonsWithKeys.begin(); button != buttonsWithKeys.end(); ++button)
	{
		button->second.Display();
	}
}

void Menu::OnKeyPressed(char key, World *world)
{
	for (auto button = buttonsWithKeys.begin(); button != buttonsWithKeys.end(); ++button)
	{
		if (button->first == key)
		{
			button->second.OnKeyPressed(world);
			return;
		}
	}
}