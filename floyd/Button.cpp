#include "Button.h"

#include <iostream>


Button::Button() : name(""), label("") 
{
}

Button::Button(const std::string &newName, const std::string &newLabel)
	: name(newName), label(newLabel)
{
}

void Button::Display() const
{
	std::cout << label << std::endl;
}