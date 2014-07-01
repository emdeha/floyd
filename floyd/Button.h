#ifndef FLOYD_BUTTON_H
#define FLOYD_BUTTON_H


#include <string>


class Button
{
private:
	std::string name;
	std::string label;

public:
	Button();
	Button(const std::string &newName, const std::string &newLabel);

	void Display() const;
};


#endif