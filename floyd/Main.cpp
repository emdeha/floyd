#include <iostream>
#include <chrono>
#include <thread>

#include "World.h"


int main()
{
	World world;
	world.Init("world.fl");
	// TODO: Should display starting cutscene
	world.Update();
	world.Display(); 

	while (true)
	{
		world.PollInput();
		world.Update();
		world.Display();

		double sleep_secs = 0.3;
		int sleep_ms = static_cast<int>(sleep_secs * 1000.0);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	}

	return 0;
}