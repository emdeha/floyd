#include "Button.h"
#include "World.h"

#include <iostream>


Button::Button() : name(""), label(""), key('\0'), isHidden(false)
{
}

Button::Button(const std::string &newName, const std::string &newLabel, char newKey, bool newIsHidden)
	: name(newName), label(newLabel), key(newKey), isHidden(newIsHidden)
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
	//if ( ! isHidden)
	//{
		onClick(world);
	//}
}

void Button::SetIsHidden(bool newIsHidden)
{
	isHidden = newIsHidden;
}

std::string Button::GetName() const
{
	return name;
}

char Button::GetKey() const
{
	return key;
}

bool Button::IsHidden() const
{
	return isHidden;
}