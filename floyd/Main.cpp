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
	world.OnFreshStart();
	world.SwitchState(STATE_GAMEPLAY);

	Graphics::AllocateBuffer();
	Graphics::Init();

	//Sprite spriteToAdd(14, 9);
	//std::string fileName = ResolveFileName("1", DIR_WORLD);
	//spriteToAdd.LoadTexture(fileName);

	//Sprite hero(1, 1);
	//hero.LoadTextureFromRawData("|\n");

	while (world.IsRunning())
	{
		//world.PollInput();
		//world.Update();

		Graphics::ClearScreen();

		//world.Display();
		//Graphics::AddSpriteToBuffer(&spriteToAdd, Position(0,0));
		//Graphics::AddSpriteToBuffer(&hero, Position(5,5));

		if (world.GetState() == STATE_GAMEPLAY) // hack. GUI elements should also be added.
		{
			auto spritesToDraw = world.GetSpritesForDrawing();
			for (auto sprite = spritesToDraw.begin(); sprite != spritesToDraw.end(); ++sprite)
			{
				Graphics::AddSpriteToBuffer(sprite->first, sprite->second);
			}
		}

		Graphics::DisplayBuffer();

		Graphics::SwapBuffers();

		// Limit that framerate, 'cos the dude's too fast.
		//int sleep_ms = 33;
		//std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	}

	return 0;
}