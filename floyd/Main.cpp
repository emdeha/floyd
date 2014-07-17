#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include "World.h"
#include "Dirs.h"
#include "Floyd_Graphics/Graphics.h"
#include "Floyd_Graphics/Sprite.h"


int main()
{
	World world;
	world.Init();

	Graphics::AllocateBuffer();
	Graphics::Init();

	Sprite spriteToAdd(Position(0, 0), 14, 9);
	std::string fileName = ResolveFileName("1", DIR_WORLD);
	spriteToAdd.LoadTexture(fileName);

	Graphics::AddSpriteToBuffer(&spriteToAdd);

	while (world.IsRunning())
	{
		//world.PollInput();
		//world.Update();

		Graphics::ClearScreen();

		//world.Display();
		Graphics::DisplayBuffer();

		Graphics::SwapBuffers();

		// Limit that framerate, 'cos the dude's too fast.
		//int sleep_ms = 33;
		//std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	}

	return 0;
}