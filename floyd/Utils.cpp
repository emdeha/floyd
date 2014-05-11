#include "Utils.h"

#include <random>


time_t GetTimeSinceStart()
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