#include <iostream>
#include <chrono>
#include <thread>

#include "World.h"
#include "Dirs.h"


int main()
{
	World world;
	world.Init();
	world.OnFreshStart();

	while (true)
	{
		world.PollInput();
		world.Update();
		world.Display();

		// Limit that framerate, 'cos the dude's too fast.
		int sleep_ms = 33;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	}

	return 0;
}