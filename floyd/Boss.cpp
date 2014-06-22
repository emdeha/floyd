#include <fstream>
#include <iostream>
#include <sstream>

#include "Boss.h"
#include "World.h"


// Forgive me Father for I have sinned:
Position dirs[8];

void InitDirs()
{
	dirs[0] = Position( 0, -1);
	dirs[1] = Position( 1, -1);
	dirs[2] = Position( 1,  0);
	dirs[3] = Position( 1,  1);
	dirs[4] = Position( 0,  1);
	dirs[5] = Position(-1,  1);
	dirs[6] = Position(-1,  0);
	dirs[7] = Position(-1, -1);
}

Boss::Boss()
	: damage(0), health(0), position(1,1), prevPosition(1,1), prevTile(TILE_EMPTY), currentWaypoint(0),
	  particleEmitInterval_s(3), amountOfParticlesPerEmission(4), isDead(false)
{
	lastTimeOfEmission_s = GetTimeSinceEpoch();
	InitDirs();
}

///
/// Boss file is as follows:
/// line one - health
/// line two - damage
/// line three - defense
/// line four - reference to StatSprite file
///
void Boss::Init(const std::string &bossFile)
{
	std::ifstream boss(bossFile);

	if (boss.is_open())
	{
		std::string line;
		int idx = 0;
		while (std::getline(boss, line).good())
		{
			switch (idx)
			{
			case LINE_HEALTH:
				std::stringstream(line) >> health;
				maxHealth = health;
				break;
			case LINE_DAMAGE:
				std::stringstream(line) >> damage;
				break;
			case LINE_DEFENSE:
				std::stringstream(line) >> defense;
				break;
			case LINE_STATSPRITE_FILE:
				// Load StatSprite
				break;
			default:
				std::cerr << "Warning: Invalid line idx\n";
			}
			++idx;
		}
	}
	else
	{
		std::cerr << "Error: Opening boss file!\n"; 
		return;
	}

	boss.close();
}

void Boss::Update(World *world)
{
	//prevPosition = position;
	//position = wp[currentWaypoint];

	//++currentWaypoint;
	//if (currentWaypoint > 5)
	//{
	//	currentWaypoint	= 0;
	//}

	time_t timeSinceStart_s = GetTimeSinceEpoch();
	if (timeSinceStart_s - lastTimeOfEmission_s > particleEmitInterval_s)
	{
		EmitParticlesInCircle(world);
		lastTimeOfEmission_s = timeSinceStart_s;
	}
}

void Boss::EmitParticlesInCircle(World *world)
{
	assert(amountOfParticlesPerEmission < 9);

	world->AddParticle(position.PositionAfterMove(dirs[0]), dirs[0], damage, false);
	world->AddParticle(position.PositionAfterMove(dirs[2]), dirs[2], damage, false);
	world->AddParticle(position.PositionAfterMove(dirs[4]), dirs[4], damage, false);
	world->AddParticle(position.PositionAfterMove(dirs[6]), dirs[6], damage, false);
}

void Boss::SetInitialPosition(Position newPosition)
{
	position = newPosition;

	// Scripted boss path.
	wp[0] = Position(position.x + 1, position.y - 1);
	wp[1] = Position(position.x + 2, position.y - 1);
	wp[2] = Position(position.x + 3, position.y);
	wp[3] = Position(position.x + 2, position.y + 1);
	wp[4] = Position(position.x + 1, position.y + 1);
	wp[5] = position;
}

void Boss::GoToPrevPos()
{
	position = prevPosition;
}

Position Boss::GetPosition() const
{
	return position;
}
Position Boss::GetPrevPos() const
{
	return prevPosition;
}

int Boss::GetDamage() const
{
	return damage;
}
int Boss::GetHealth() const
{
	return health;
}
int Boss::GetDefense() const
{
	return defense;
}

int Boss::GetMaxHealth() const
{
	return maxHealth;
}

void Boss::ApplyDamage(int dmg)
{
	health -= dmg / (defense == 0 ? 1 : defense);
}

char Boss::GetPrevTile() const
{
	return prevTile;
}
void Boss::SetPrevTile(char newPrevTile)
{
	// TODO: Dirty hack. Better to use layers.
	prevTile = (newPrevTile != TILE_MONSTER && newPrevTile != TILE_HERO && newPrevTile != TILE_PARTICLE)
				? newPrevTile : TILE_EMPTY;
}

bool Boss::IsDead() const
{
	return isDead;
}
void Boss::SetIsDead(bool newIsDead)
{
	isDead = newIsDead;
}