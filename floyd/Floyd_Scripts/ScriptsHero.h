#ifndef FLOYD_SCRIPTS_HERO_H
#define FLOYD_SCRIPTS_HERO_H


class Entity;
class MovableComponent;

namespace Floyd
{
void ScriptHero_OnKeyPressed(Entity *owner, char key);
}


#endif