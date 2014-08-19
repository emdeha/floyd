#include "stdafx.h"
#include "Menu.h"

#include "../Floyd_World/World.h"
#include "../Floyd_Scripts/Scripts.h"
#include "../Floyd_General/Reporting.h"

#include <fstream>
#include <iostream>
#include <sstream>


void SetScriptForButton(Button *button, const std::string &scriptName)
{
	if (scriptName == "newGame")
	{
		button->SetOnClickCallback(ButtonScripts::NewGameOnClick);
	}
	else if (scriptName == "resumeGame")
	{
		button->SetOnClickCallback(ButtonScripts::ResumeGameOnClick);
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
		std::ostringstream error;
		error << "Invalid script name: '" << scriptName << "'\n";
		Report::Error(error.str(), __LINE__, __FILE__);
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
///	btnName, btnLabel, btnKey, isHidden
/// UNSAFE!!!
///
void Menu::Init(const std::string &menuFile)
{
	buttons.clear(); // Hack for the double initting of startupMenu. (put a break at World::Init())

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
				size_t secondDelimPos = line.find(',', firstDelimPos + 1);
				std::string btnLabel = line.substr(firstDelimPos + 1, secondDelimPos - firstDelimPos - 1);
				size_t thirdDelimPos = line.find(',', secondDelimPos + 1);
				std::string isHidden_str = line.substr(thirdDelimPos + 1, thirdDelimPos - secondDelimPos - 1);
				bool isHidden = isHidden_str[0] == '0' ? false : true; // Bad hack. SafeLexicalCast yields an error.
				char btnKey = line.substr(secondDelimPos + 1, line.length())[0];
				Button newButton(btnName, btnLabel, btnKey, isHidden);
				newButton.Init();

				SetScriptForButton(&newButton, btnName);

				buttons.push_back(newButton);
			}
			else
			{
				Report::Error("Parsing menu loading file\n", __LINE__, __FILE__);
			}
		}
	}

	menu.close();
}

void Menu::OnKeyPressed(char key, World *world)
{
	for (auto button = buttons.begin(); button != buttons.end(); ++button)
	{
		if (button->GetKey() == key)
		{
			if (button->OnKeyPressed(world))
			{
				return;
			}
		}
	}
}

void Menu::ShowButton(const std::string &buttonName)
{
	for (auto button = buttons.begin(); button != buttons.end(); ++button)
	{
		if (buttonName == button->GetName())
		{
			button->SetIsHidden(false);
			return;
		}
	}
}

void Menu::HideButton(const std::string &buttonName)
{
	for (auto button = buttons.begin(); button != buttons.end(); ++button)
	{
		if (buttonName == button->GetName())
		{
			button->SetIsHidden(true);
			return;
		}
	}
}

std::vector<std::pair<int, const Button*>> Menu::GetButtonsForDrawing() const
{
	std::vector<std::pair<int, const Button*>> result;

	size_t idx = 0;
	for (auto button = buttons.begin(); button != buttons.end(); ++button)
	{
		if ( ! button->IsHidden())
		{
			result.push_back(std::make_pair(idx, &(*button)));
			++idx;
		}
	}

	assert(result.size() > 0);

	return result;
}