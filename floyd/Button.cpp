#include "Button.h"
#include "World.h"

#include <iostream>


Button::Button() : name(""), label("") 
{
}

Button::Button(const std::string &newName, const std::string &newLabel)
	: name(newName), label(newLabel)
{
}

void Button::SetOnClickCallback(OnClickCallback newCallback)
{
	onClick = newCallback;
}

void Button::Display() const
{
	std::cout << label << std::endl;
}

void Button::OnKeyPressed(World *world)
{
	onClick(world);
}