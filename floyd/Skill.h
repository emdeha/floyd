#ifndef FLOYD_SKILL_H
#define FLOYD_SKILL_H


class World;

class Skill
{
private:
	char activationButton;

public:
	Skill(char newActivationButton);
	virtual ~Skill() {};

	void Apply(World *world);

	char GetActivationButton() const;

protected:
	virtual void OnApply(World *world) = 0;
};


class ParticleSkill : public Skill
{
private:
	int damage;

public:
	ParticleSkill(char newActivationButton, int newDamage);

protected:
	virtual void OnApply(World *world);
};


#endif