#ifndef FLOYD_STAT_COMPONENT_H
#define FLOYD_STAT_COMPONENT_H


#include "Component.h"
#include "../Floyd_Graphics/Sprite.h"


class StatComponent : public IComponent
{
public:
	int health;
	int defense;
	int damage;
	int maxHealth;

	Sprite healthBar;

	explicit StatComponent();
	StatComponent(int newHealth, int newDefense, int newDamage, int newMaxHealth);

public:
	void ApplyDamage(int dmg);

	Sprite* GetHealthBarAsSprite();

	void InitFromFile(const std::string &fileName);

private:
	void DoSerialization(std::ofstream &saveStream) const;
	void DoDeserialization(std::ifstream &loadStream);
};


#endif