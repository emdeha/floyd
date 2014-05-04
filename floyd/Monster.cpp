#include "Monster.h"


void Monster::Update()
{
	if (currentDelta < maxDeltaY)
	{
		prevPosition = position;
		position.x += deltaY;
		currentDelta++;
	}
	else 
	{
		deltaY *= -1;
		currentDelta = 0;
	}
}