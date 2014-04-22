#include <iostream>

#include "World.h"


int main()
{
	World world;
	world.Init("world.fl");
	// TODO: Should display starting cutscene
	world.Update();
	world.Display(); 

	while (world.PollInput())
	{
		world.Update();
		world.Display();
	}

	return 0;
}