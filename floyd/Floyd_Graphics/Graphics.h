#ifndef FLOYD_GRAPHICS_H
#define FLOYD_GRAPHICS_H


struct Sprite;

namespace Graphics
{
	// Maybe it'll be used when a bigger console could be enabled.
	// Currently we have a buffer which is 80 columns by 25 rows.
	// void AllocateBuffer(int width, int height);

	void Init();
	void ClearScreen();
	void AddSpriteToBuffer(const Sprite *sprite);
	void SwapBuffers();
};


#endif