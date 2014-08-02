#include "stdafx.h"
#include "Graphics.h"
#include "Sprite.h"
#include "../Utils.h"

#include <Windows.h>
#include <iostream>


const int DIM_RIGHT = 80;
const int DIM_BOTTOM = 25;

// TODO: What if we start using multiple threads?
static HANDLE drawBuffer;
static HANDLE setBuffer;

static char spriteBuffer[DIM_BOTTOM + 1][DIM_RIGHT + 1]; 


void Graphics::AllocateBuffer()
{
	Graphics::ClearBuffer();
}

void Graphics::ClearBuffer()
{
	for (size_t row = 0; row < DIM_BOTTOM; ++row)
	{
		for (size_t col = 0; col < DIM_RIGHT; ++col)
		{
			spriteBuffer[row][col] = ' ';
		}
		spriteBuffer[row][DIM_RIGHT] = '\0';
	}
}

void Graphics::Init()
{
	drawBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
	setBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
										  FILE_SHARE_READ | FILE_SHARE_WRITE,
										  NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	if (drawBuffer == INVALID_HANDLE_VALUE || setBuffer == INVALID_HANDLE_VALUE)
	{
		std::cerr << "CreateConsoleScreenBuffer failed - " << GetLastError() << std::endl;
		return;
	}
}

void Graphics::ClearScreen()
{
	HANDLE tempBuf = drawBuffer;
	drawBuffer = setBuffer;
	setBuffer = tempBuf;

	if (!SetStdHandle(STD_OUTPUT_HANDLE, drawBuffer))
	{
		std::cerr << "SetStdHandle failed - " << GetLastError() << std::endl;
		return;
	}

	ClearHandleScreen(drawBuffer);
}

void Graphics::AddSpriteToBuffer(const Sprite *sprite, const Position &spritePosition)
{
	size_t width = sprite->GetWidth();
	size_t height = sprite->GetHeight();
	const std::vector<std::string> *textureData = sprite->GetTexture()->GetData();

	Position spritePosition_console = Position(spritePosition.x, spritePosition.y + 1);

	assert(spritePosition_console.x >= 0 && spritePosition_console.x + width < DIM_RIGHT &&
		   spritePosition_console.y > 0 && spritePosition_console.y + height < DIM_BOTTOM);

	size_t textureX = 0;
	size_t textureY = 0;
	size_t finalY = height + spritePosition_console.y;
	size_t finalX = width + spritePosition_console.x;
	for (size_t y = spritePosition_console.y; y < finalY; ++y)
	{
		for (size_t x = spritePosition_console.x; x < finalX; ++x)
		{
			spriteBuffer[y][x] = (*textureData)[textureY][textureX];
			++textureX;
		}
		++textureY;
		textureX = 0;
	}
}

void Graphics::SwapBuffers()
{
	SMALL_RECT srctReadRect;
	srctReadRect.Top = 0;
	srctReadRect.Left = 0;
	srctReadRect.Right = DIM_RIGHT - 1;
	srctReadRect.Bottom = DIM_BOTTOM - 1;

	COORD coordBufSize;
	coordBufSize.X = DIM_RIGHT;
	coordBufSize.Y = DIM_BOTTOM;

	COORD coordBufTopLeft;
	coordBufTopLeft.X = 0;
	coordBufTopLeft.Y = 0;

	CHAR_INFO currentOutput[DIM_RIGHT * DIM_BOTTOM];
	BOOL fSuccess = ReadConsoleOutput(
		drawBuffer, currentOutput, coordBufSize, coordBufTopLeft, &srctReadRect);
	if (!fSuccess)
	{
		std::cerr << "ReadConsoleOutput failed - " << GetLastError() << std::endl;
		return;
	}

	fSuccess = WriteConsoleOutput(
		setBuffer, currentOutput, coordBufSize, coordBufTopLeft, &srctReadRect);
	if (!fSuccess)
	{
		std::cerr << "WriteConsoleOutput failed - " << GetLastError() << std::endl;
		return;
	}

	// Switch buffers
	if (!SetConsoleActiveScreenBuffer(drawBuffer))
	{
		std::cerr << "SetConsoleActiveScreenBuffer failed - " << GetLastError() << std::endl;
		return;
	}
}

void Graphics::DisplayBuffer()
{
	for (size_t row = 0; row < DIM_BOTTOM; ++row)
	{
		//std::cout << spriteBuffer[row];
		std::printf(spriteBuffer[row]); // much faster that std::cout
	}
}