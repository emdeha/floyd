#include "Button.h"
#include "World.h"

#include <iostream>


Button::Button() : name(""), label(""), isHidden(false)
{
}

Button::Button(const std::string &newName, const std::string &newLabel, bool newIsHidden)
	: name(newName), label(newLabel), isHidden(newIsHidden)
{
}

void Button::SetOnClickCallback(OnClickCallback newCallback)
{
	onClick = newCallback;
}

void Button::Display() const
{
	if ( ! isHidden)
	{
		std::cout << label << std::endl;
	}
}

void Button::OnKeyPressed(World *world)
{
	if ( ! isHidden)
	{
		onClick(world);
	}
}

void Button::SetIsHidden(bool newIsHidden)
{
	isHidden = newIsHidden;
}

std::string Button::GetName() const
{
	return name;
}