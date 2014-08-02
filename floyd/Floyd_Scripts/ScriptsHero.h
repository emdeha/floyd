#ifndef FLOYD_SCRIPTS_HERO_H
#define FLOYD_SCRIPTS_HERO_H


class Entity;
class MovableComponent;
struct Tile;

namespace Floyd
{
void ScriptHero_OnKeyPressed(Entity *owner, char key);
void ScriptHero_OnCollision(Entity *owner, const Tile *collider);
}


#endif