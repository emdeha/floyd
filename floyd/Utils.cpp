#include "Utils.h"

#include <random>
#include <iostream>


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