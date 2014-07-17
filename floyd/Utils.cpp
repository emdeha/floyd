#include "Utils.h"
#include "Dirs.h"

#include <random>
#include <iostream>
#include <fstream>


///////////////////////////////
//  Other utility functions  //
///////////////////////////////
time_t GetTimeSinceEpoch()
{
	return time(0);
}

int GetRandomInRange(int min, int max)
{
	if (min > max)
	{
		int tmp = min;
		min = max;
		max = tmp;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}

Position GetRandomDirection()
{
	Position direction(0,0);
	while (direction.IsEqual(Position(0,0)))
	{
		direction.x = GetRandomInRange(-1, 1);
		direction.y = GetRandomInRange(-1, 1);
	}
	return direction;
}

void ClearHandleScreen(HANDLE handle)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(handle, &csbi))
	{
		std::cerr << "GetConsoleScreenBufferInfo failed - " << GetLastError() << std::endl;
		return;
	}

	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	if (!FillConsoleOutputCharacter(handle, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
	{
		std::cerr << "FillConsoleOutputCharacter failed - " << GetLastError() << std::endl;
		return;
	}

	if (!GetConsoleScreenBufferInfo(handle, &csbi))
	{
		std::cerr << "GetConsoleScreenBufferInfo failed - " << GetLastError() << std::endl;
		return;
	}

	if (!FillConsoleOutputAttribute(handle, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
	{
		std::cerr << "FillConsoleOutputAttribute failed - " << GetLastError() << std::endl;
		return;
	}

	SetConsoleCursorPosition(handle, coordScreen);
}

std::string ResolveFileName(const std::string &fileName, const std::string &relativePath)
{
	std::string resolvedFileName = relativePath;
	resolvedFileName += fileName;
	resolvedFileName += EXT_LEVEL;
	return resolvedFileName;
}

std::string GetHealthBar(float healthQuotient)
{
	int healthPercentage = int(healthQuotient * 100);
	
	std::string healthBar(healthPercentage / 3, '!');

	return healthBar;
}

std::string RemoveChar(char ch, const std::string &str)
{
	std::string result;
	result.reserve(str.length());
	for (size_t idx = 0; idx < str.length(); ++idx)
	{
		if (str[idx] != ch)
		{
			result.push_back(str[idx]);
		}
	}

	return result;
}

std::string Trim(const std::string &str)
{
	std::string result;
	result = RemoveChar(' ', str);
	result = RemoveChar('\t', result);

	return result;
}