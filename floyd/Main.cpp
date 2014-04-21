#include <iostream>

#include "World.h"


int main()
{
	World world;
	world.Init("world.fl");

	while (true)
	{
		world.Display();
	}

	return 0;
}