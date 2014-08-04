#ifndef FLOYD_SCRIPTS_PARTICLE_H
#define FLOYD_SCRIPTS_PARTICLE_H


class Entity;
class World;
struct Tile;

namespace Floyd
{
void ScriptParticle_OnCollision(World *world, Entity *owner, const Tile *collider);
void ScriptParticle_OnUpdateAI(World *world, Entity *owner);
}


#endif