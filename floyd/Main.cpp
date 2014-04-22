#include <iostream>

#include "World.h"


int main()
{
	World world;
	world.Init("world.fl");
	// TODO: Should display starting cutscene
	world.Display(); 

	while (world.PollInput())
	{
		world.Display();
	}

	return 0;
}