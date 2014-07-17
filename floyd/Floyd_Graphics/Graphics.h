#ifndef FLOYD_GRAPHICS_H
#define FLOYD_GRAPHICS_H


class Sprite;

namespace Graphics
{
	// Maybe it'll be used when a bigger console could be enabled.
	// Currently we have a buffer which is 80 columns by 25 rows.
	// Currently it just sets all buffer elems to ' ' (empty string).
	void AllocateBuffer();

	void Init();
	void ClearScreen();
	void AddSpriteToBuffer(const Sprite *sprite);
	void SwapBuffers();
};


#endif