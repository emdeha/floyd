#include "stdafx.h"
#include "Button.h"

#include "../Floyd_World/World.h"

#include <iostream>


Button::Button() : name(""), label(""), key('\0'), isHidden(false)
{
}

Button::Button(const std::string &newName, const std::string &newLabel, char newKey, bool newIsHidden)
	: name(newName), label(newLabel), key(newKey), isHidden(newIsHidden)
{
}

void Button::Init()
{
	sprite = Sprite(label.length(), 1);
	sprite.LoadTextureFromRawData(label + "\n");
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

bool Button::OnKeyPressed(World *world)
{
	if ( ! isHidden)
	{
		onClick(world);
		return true;
	}

	return false;
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

const Sprite* Button::GetSprite() const
{
	return &sprite;
}