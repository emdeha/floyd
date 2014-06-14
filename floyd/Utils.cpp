#include "Utils.h"
#include "Dirs.h"

#include <random>
#include <iostream>


////////////////
//  Position  //
////////////////
bool Position::IsEqual(const Position &other) const
{
	return x == other.x && y == other.y;
}

bool Position::IsPositive() const
{
	return (x >= 0) && (y >= 0);
}

int Position::GetDistanceSq(const Position &other) const
{
	int diffX = x - other.x;
	int diffY = y - other.y;

	return (diffX * diffX + diffY * diffY);
}

void Position::Move(const Position &direction)
{
	// directions must be unit vectors. 
	assert(direction.x <= 1 && direction.x >= -1 &&
		   direction.y <= 1 && direction.y >= -1);

	x += direction.x;
	y += direction.y;
}


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