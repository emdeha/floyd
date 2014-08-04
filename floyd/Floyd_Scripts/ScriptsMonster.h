#ifndef FLOYD_SCRIPTS_MONSTER_H
#define FLOYD_SCRIPTS_MONSTER_H


class Entity;
class World;
struct Tile;

namespace Floyd
{
void ScriptMonster_OnCollision(World *world, Entity *owner, const Tile *collider);
void ScriptMonster_OnUpdateAI(World *world, Entity *owner);
}


#endif