#ifndef FLOYD_GRAPHICS_H
#define FLOYD_GRAPHICS_H


class Sprite;

namespace Graphics
{
	void AllocateBuffer(int width, int height);

	void Init();
	void ClearScreen();
	void AddSpriteToBuffer(const Sprite *sprite);
	void SwapBuffers();
};


#endif