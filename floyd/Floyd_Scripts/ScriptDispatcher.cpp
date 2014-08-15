#include "stdafx.h"
#include "ScriptDispatcher.h"

#include "ScriptsBoss.h"
#include "ScriptsHero.h"
#include "ScriptsMonster.h"
#include "ScriptsParticle.h"

#include "../Reporting.h"


Floyd::OnCollision Floyd::GetOnCollision(const std::string &group)
{
	if (group == "hero")
	{
		return ScriptHero_OnCollision;	
	}
	else if (group == "monster")
	{
		return ScriptMonster_OnCollision;
	}
	else if (group == "boss")
	{
		return ScriptBoss_OnCollision;
	}
	else if (group == "particle")
	{
		return ScriptParticle_OnCollision;
	}

	std::string msg = "Invalid group: ";
	msg += group;
	Report::Error(msg, __LINE__, __FILE__);

	return nullptr;
}

Floyd::OnKeyPressed Floyd::GetOnKeyPressed(const std::string &group)
{
	if (group == "hero")
	{
		return ScriptHero_OnKeyPressed;	
	}

	std::string msg = "Invalid group: ";
	msg += group;
	Report::Error(msg, __LINE__, __FILE__);

	return nullptr;
}

Floyd::OnUpdateAI Floyd::GetOnUpdateAI(const std::string &group)
{
	if (group == "monster")
	{
		return ScriptMonster_OnUpdateAI;
	}
	else if (group == "boss")
	{
		return ScriptBoss_OnUpdateAI;
	}
	else if (group == "particle")
	{
		return ScriptParticle_OnUpdateAI;
	}

	std::string msg = "Invalid group: ";
	msg += group;
	Report::Error(msg, __LINE__, __FILE__);

	return nullptr;
}