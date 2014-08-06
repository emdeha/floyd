#ifndef FLOYD_SCRIPTS_BOSS_H
#define FLOYD_SCRIPTS_BOSS_H


class Entity;
class World;
struct Tile;

namespace Floyd
{
void ScriptBoss_OnCollision(World *world, Entity *owner, const Tile *collider);
void ScriptBoss_OnUpdateAI(World *world, Entity	*owner);
}


#endif