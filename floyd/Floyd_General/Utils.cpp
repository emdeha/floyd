#include "stdafx.h"
#include "Utils.h"

#include "Dirs.h"
#include "Reporting.h"

#include <random>
#include <fstream>
#include <sstream>


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
		std::ostringstream error;
		error << "GetConsoleScreenBufferInfo failed - " << GetLastError() << std::endl;
		Report::Error(error.str(), __LINE__, __FILE__);
		return;
	}

	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	if (!FillConsoleOutputCharacter(handle, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
	{
		std::ostringstream error;
		error << "FillConsoleOutputCharacter failed - " << GetLastError() << std::endl;
		Report::Error(error.str(), __LINE__, __FILE__);
		return;
	}

	if (!GetConsoleScreenBufferInfo(handle, &csbi))
	{
		std::ostringstream error;
		error << "GetConsoleScreenBufferInfo failed - " << GetLastError() << std::endl;
		Report::Error(error.str(), __LINE__, __FILE__);
		return;
	}

	if (!FillConsoleOutputAttribute(handle, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
	{
		std::ostringstream error;
		error << "FillConsoleOutputAttribute failed - " << GetLastError() << std::endl;
		Report::Error(error.str(), __LINE__, __FILE__);
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

void SerializePosition(const Position &position, std::ofstream &saveStream)
{
	if (saveStream.is_open())
	{
		saveStream.write((char*)&position.x, sizeof(position.x));
		saveStream.write((char*)&position.y, sizeof(position.y));
	}
	else
	{
		Report::Error("Cannot serialize Position", __LINE__, __FILE__);
	}
}

void DeserializePosition(Position &position, std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		loadStream.read((char*)&position.x, sizeof(position.x));
		loadStream.read((char*)&position.y, sizeof(position.y));
	}
	else
	{
		Report::Error("Cannot deserialize Position", __LINE__, __FILE__);
	}
}