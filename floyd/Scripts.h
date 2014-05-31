#ifndef FLOYD_SCRIPTS_H
#define FLOYD_SCRIPTS_H


class World;

class IScript 
{
public:
	virtual void OnStart(World *world) = 0;
	virtual void OnUpdate(World *world) = 0;
};

class LevelScript : public IScript
{
public:
	void OnStart(World *world);
	void OnUpdate(World *world);
};


#endif