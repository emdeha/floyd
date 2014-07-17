#include "stdafx.h"
#include "Graphics.h"
#include "../floyd/Utils.h"

#include <Windows.h>
#include <iostream>


const int DIM_RIGHT = 80;
const int DIM_BOTTOM = 25;

// TODO: What if we start using multiple threads?
static HANDLE drawBuffer;
static HANDLE setBuffer;


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