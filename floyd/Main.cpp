#include <iostream>
#include <chrono>
#include <thread>

#include "World.h"
#include "Dirs.h"
#include "Graphics.h"


int main()
{
	World world;
	world.Init();
	world.OnFreshStart(); // Should be called on New Game, not here

	Graphics::Init();

	while (world.IsRunning())
	{
		world.PollInput();
		world.Update();

		Graphics::ClearScreen();

		world.Display();

		Graphics::SwapBuffers();

		// Limit that framerate, 'cos the dude's too fast.
		int sleep_ms = 33;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	}

	return 0;
}