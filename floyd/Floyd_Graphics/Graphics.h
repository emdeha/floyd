#ifndef FLOYD_GRAPHICS_H
#define FLOYD_GRAPHICS_H


#include "../Floyd_Geometry/Vector.h"


class Sprite;

///
/// @brief Graphics provides an interface for generic display of sprites.
///		   Currently, it uses the console and an internal buffer of characters to which it renders all 
///		   sprites. This buffer is called **spriteBuffer**.
///		   It also implements buffer switching. We draw in the **drawBuffer** while showing
///		   the **setBuffer**.
///
namespace Graphics
{
	/// 
	/// @brief Maybe it'll be used when a bigger console could be enabled.
	///		   Currently we have a buffer which is 80 columns by 25 rows.
	/// 	   Currently it just sets all buffer elems to ' ' (it calls ClearBuffer).
	///
	void AllocateBuffer();
	///
	/// @brief Sets the contents of the sprite buffer to ' ' and null-terminates every line.
	///
	void ClearBuffer();

	///
	/// @brief Initializes the buffer handles.
	///
	void Init();
	///
	/// @brief Swaps the buffer handles and clears the draw buffer.
	///
	void ClearScreen();
	///
	/// @brief Adds a sprite to the sprite buffer at a given position.
	///
	void AddSpriteToBuffer(const Sprite *sprite, const Position &spritePosition);
	///
	/// @brief Copies contents from the draw buffer to the set buffer and
	///		   activates the draw buffer.
	///
	void SwapBuffers();

	///
	/// @brief Displays the sprite buffer.
	///
	void DisplayBuffer();
};


#endif