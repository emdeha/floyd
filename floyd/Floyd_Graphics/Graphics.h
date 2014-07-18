#ifndef FLOYD_GRAPHICS_H
#define FLOYD_GRAPHICS_H


#include "../Floyd_Geometry/Vector.h"


class Sprite;

namespace Graphics
{
	// Maybe it'll be used when a bigger console could be enabled.
	// Currently we have a buffer which is 80 columns by 25 rows.
	// Currently it just sets all buffer elems to ' ' (it calls ClearBuffer).
	void AllocateBuffer();
	void ClearBuffer(); // Maybe we should call it on ClearScreen if an appropriate flag is set

	void Init();
	void ClearScreen();
	void AddSpriteToBuffer(const Sprite *sprite, const Position &spritePosition);
	void SwapBuffers();

	void DisplayBuffer();
};


#endif