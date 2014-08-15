#ifndef FLOYD_SCRIPT_MAP_H
#define FLOYD_SCRIPT_MAP_H


#include <string>

class World;
class Entity;
struct Tile;

namespace Floyd
{
typedef void (*OnCollision)(World*, Entity*, const Tile*);
typedef void (*OnKeyPressed)(World*, Entity*, char);
typedef void (*OnUpdateAI)(World*, Entity*);

OnCollision GetOnCollision(const std::string &group);
OnKeyPressed GetOnKeyPressed(const std::string &group);
OnUpdateAI GetOnUpdateAI(const std::string &group);
}


#endif