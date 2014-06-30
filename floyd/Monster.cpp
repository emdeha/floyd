#include <fstream>
#include <iostream>
#include <sstream>

#include "Monster.h"
#include "World.h"


Monster::Monster() 
	: damage(0), health(0), position(1,1), prevPosition(1,1), deltaY(1), currentDelta(0), 
	  particleEmitInterval_s(1), prevTile(TILE_EMPTY) 
{
	lastTimeOfEmission_s = GetTimeSinceEpoch();
}

///
/// Monster file is as follows:
///	line one - health
/// line two - damage
///
// TODO: Generalize for monsters, particles, bosses, heroes
void Monster::Init(const std::string &monsterFile)
{
	std::ifstream monster(monsterFile);

	if (monster.is_open())
	{
		std::string line;
		int idx = 0;
		while (std::getline(monster, line).good())
		{
			switch (idx)
			{
			case LINE_HEALTH:
				std::stringstream(line) >> health;
				break;
			case LINE_DAMAGE:
				std::stringstream(line) >> damage;
				break;
			default:
				std::cerr << "Warning: Invalid line idx\n";
			}
			++idx;
		}
	}
	else
	{
		std::cerr << "Error: Opening hero file!\n"; 
		return;
	}

	monster.close();
}

void Monster::Update(World *world)
{
	// Come on, baby, lets do the boogie-woogie style!

	//if (currentDelta >= maxDeltaY)
	//{
	//	deltaY *= -1;
	//	currentDelta = 0;
	//}

	//prevPosition = position;
	//position.x += deltaY;
	//currentDelta++;

	time_t timeSinceStart_s = GetTimeSinceEpoch();
	if (timeSinceStart_s - lastTimeOfEmission_s > particleEmitInterval_s)
	{
		EmitParticle(world);
		lastTimeOfEmission_s = timeSinceStart_s;
	}
}

void Monster::EmitParticle(World *world)
{
	Position particleDir = GetRandomDirection();
	world->AddParticle(position.PositionAfterMove(particleDir), particleDir, damage, false);
}

void Monster::SetInitialPosition(Position newPosition)
{
	position = newPosition;
}

void Monster::GoToPrevPos()
{
	position = prevPosition;
}

Position Monster::GetPosition() const
{
	return position;
}
Position Monster::GetPrevPos() const
{
	return prevPosition;
}

int Monster::GetDamage() const
{
	return damage;
}
int Monster::GetHealth() const
{
	return health;
}

void Monster::ApplyDamage(int dmg)
{
	health -= dmg;
}

char Monster::GetPrevTile() const
{
	return prevTile;
}
void Monster::SetPrevTile(char newPrevTile)
{
	// TODO: Dirty hack. Better to use layers.
	prevTile = (newPrevTile != TILE_MONSTER && newPrevTile != TILE_HERO && newPrevTile != TILE_PARTICLE)
				? newPrevTile : TILE_EMPTY;
}

void Monster::Serialize(std::ofstream &saveStream) const
{
	if (saveStream.is_open())
	{
		saveStream << damage;
		saveStream << health;
		position.Serialize(saveStream);
		prevPosition.Serialize(saveStream);
		saveStream << currentDelta;
		saveStream << deltaY;
		saveStream << lastTimeOfEmission_s;
		saveStream << prevTile;
	}
	else
	{
		std::cerr << "Error: Cannot serialize Monster\n";
	}
}
void Monster::Deserialize(std::ifstream &loadStream)
{
	if (loadStream.is_open())
	{
		loadStream >> damage;
		loadStream >> health;
		position.Deserialize(loadStream);
		prevPosition.Deserialize(loadStream);
		loadStream >> currentDelta;
		loadStream >> deltaY;
		loadStream >> lastTimeOfEmission_s;
		loadStream >> prevTile;
	}
	else
	{
		std::cerr << "Error: Cannot deserialize Monster\n";
	}
}