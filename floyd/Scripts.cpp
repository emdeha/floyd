#include "Scripts.h"
#include "World.h"


////////////////////
//  Level Script  //
////////////////////

void LevelScript::OnStart(World *world)
{

}

void LevelScript::OnUpdate(World *world)
{
	Level *currentLevel = world->GetCurrentLevel();	
	int currentLevelIdx = world->GetCurrentLevelIdx();

	if (currentLevelIdx == 1)
	{
		if (world->GetHero().HasTalkedToNPC())
		{
			currentLevel->UnblockExit();
			currentLevel->ShowTeleport();
			world->GetHero().SetHasTalkedToNPC(false);
		}
	}
	else if (currentLevelIdx == 2)
	{
		if (world->AreMonstersDead() && !currentLevel->IsExitUnblocked())
		{
			currentLevel->UnblockExit();
		}
	}
	else if (currentLevelIdx == 3)
	{
		if (currentLevel->IsExitDisplayConditionMet())
		{
			currentLevel->SpawnMonsters(world);
			currentLevel->SetIsExitDisplayConditionMet(false);
		}
		// WARN: (To my later self) If there are no monsters, this will erase the tile guarding the exit.
		if (world->AreMonstersDead() && !currentLevel->IsExitUnblocked())
		{
			currentLevel->UnblockExit();
		}
	}
	else if (currentLevelIdx == 4)
	{
	}
	else if (currentLevelIdx == 5)
	{
		if (world->GetHero().HasTalkedToNPC())
		{
			currentLevel->UnblockExit();
			currentLevel->ShowTeleport();
			world->GetHero().SetHasTalkedToNPC(false);
		}
	}
	else if (currentLevelIdx == BOSS_LEVEL)
	{
		if (world->AreMonstersDead() && !currentLevel->IsExitUnblocked())
		{
			currentLevel->UnblockExit();
		}
	}
	else if (currentLevelIdx == 7)
	{
	}
}

//////////////////////
//  Button Scripts  //
//////////////////////

void ButtonScripts::NewGameOnClick(World *world)
{
	world->Init();
	world->OnFreshStart();
	world->SwitchState(STATE_GAMEPLAY);
}

void ButtonScripts::LoadGameOnClick(World *world)
{
	world->OnSaveLoaded();
	world->SwitchState(STATE_GAMEPLAY);
}

void ButtonScripts::SaveGameOnClick(World *world)
{
	world->Serialize();
}

void ButtonScripts::QuitGameOnClick(World *world)
{
	world->SetIsRunning(false);
}