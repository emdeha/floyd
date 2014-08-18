#include "stdafx.h"
#include "AnimatedComponent.h"

#include "../Floyd_General/Utils.h"

#include <fstream>


AnimatedComponent::AnimatedComponent()
	: animPoints(0), currentAnimPointIdx(0),
	  IComponent(CTYPE_ANIMATED)
{
}

void AnimatedComponent::AddAnimPoint(const Position &animPoint)
{
	animPoints.push_back(animPoint);
}

void AnimatedComponent::UpdateAnim()
{
	++currentAnimPointIdx;
	if (currentAnimPointIdx >= animPoints.size())
	{
		currentAnimPointIdx	= 0;
	}
}

Position AnimatedComponent::GetCurrentAnimPos() const
{
	return animPoints[currentAnimPointIdx];
}

void AnimatedComponent::DoSerialization(std::ofstream &saveStream) const
{
	saveStream.write((char*)&currentAnimPointIdx, sizeof(currentAnimPointIdx));

	size_t animPointsSize = animPoints.size();
	saveStream.write((char*)&animPointsSize, sizeof(animPointsSize));
	for (auto animPoint = animPoints.begin(); animPoint != animPoints.end(); ++animPoint)
	{
		SerializePosition((*animPoint), saveStream);
	}
}
void AnimatedComponent::DoDeserialization(std::ifstream &loadStream)
{
	loadStream.read((char*)&currentAnimPointIdx, sizeof(currentAnimPointIdx));

	size_t animPointsSize = 0;
	loadStream.read((char*)&animPointsSize, sizeof(animPointsSize));
	for (size_t idx = 0; idx < animPointsSize; ++idx)
	{
		Position newPoint;
		DeserializePosition(newPoint, loadStream);
		AddAnimPoint(newPoint);
	}
}