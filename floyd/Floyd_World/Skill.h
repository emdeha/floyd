#ifndef FLOYD_SKILL_H
#define FLOYD_SKILL_H


#include <fstream>


class World;

class Skill
{
private:
	char activationButton;
	std::string skillName;

public:
	Skill();
	Skill(char newActivationButton, const std::string &newSkillName);
	virtual ~Skill() {};

	void Apply(World *world);

	void Serialize(std::ofstream &saveStream) const;
	void Deserialize(std::ifstream &loadStream);

	char GetActivationButton() const;
	const std::string& GetSkillName() const;

protected:
	virtual void OnApply(World *world) = 0;

	virtual void DoSerialization(std::ofstream &saveStream) const = 0;
	virtual void DoDeserialization(std::ifstream &loadStream) = 0;
};


class ParticleSkill : public Skill
{
private:
	int damage;

public:
	ParticleSkill();
	ParticleSkill(char newActivationButton, const std::string &newSkillName, int newDamage);

protected:
	virtual void OnApply(World *world);

	virtual void DoSerialization(std::ofstream &saveStream) const;
	virtual void DoDeserialization(std::ifstream &loadStream);
};


#endif