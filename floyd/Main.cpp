#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include "Floyd_World/World.h"
#include "Floyd_General/Dirs.h"
#include "Floyd_Graphics/Graphics.h"
#include "Floyd_Graphics/Sprite.h"


int main()
{
	World world;
	world.Init();

	Graphics::AllocateBuffer();
	Graphics::Init();

	while (world.IsRunning())
	{
		world.PollInput();
		world.Update();

		Graphics::ClearScreen();
		Graphics::ClearBuffer();

		auto spritesToDraw = world.GetSpritesForDrawing();
		for (auto sprite = spritesToDraw.begin(); sprite != spritesToDraw.end(); ++sprite)
		{
			Graphics::AddSpriteToBuffer(sprite->first, sprite->second);
		}

		Graphics::DisplayBuffer();

		Graphics::SwapBuffers();

		// Limit that framerate, 'cos the dude's too fast.
		int sleep_ms = 33;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	}

	return 0;
}