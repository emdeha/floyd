#include "stdafx.h"
#include "ScriptsHero.h"

#include "../Floyd_ECS/Entity.h"
#include "../Floyd_General/Reporting.h"
#include "../Floyd_General/Utils.h"
#include "../Floyd_Level/Tile.h"
#include "../Floyd_World/World.h"
#include "../Floyd_World/Skill.h"

#include "../Floyd_ECS/StatComponent.h"
#include "../Floyd_ECS/InventoryComponent.h"
#include "../Floyd_ECS/QuestInfoComponent.h"
#include "../Floyd_ECS/TransformComponent.h"


void Move(Direction dir, TransformComponent *heroTransform)
{
	heroTransform->prevPosition = heroTransform->position;
	
	switch (dir)
	{
	case DIR_RIGHT:
		heroTransform->position.x += 1;
		break;
	case DIR_LEFT:
		heroTransform->position.x -= 1;
		break;
	case DIR_UP:
		heroTransform->position.y -= 1;
		break;
	case DIR_DOWN:
		heroTransform->position.y += 1;
		break;
	default:
		Report::Warning("Invalid direction", __LINE__, __FILE__);
		return;
	}

	if (heroTransform->position.y < 0)
	{
		heroTransform->position.y += 1;
	}
	if (heroTransform->position.x < 0)
	{
		heroTransform->position.x += 1;
	}
	// Validate position
}

void Floyd::ScriptHero_OnKeyPressed(World *world, Entity *owner, char key)
{
	TransformComponent *heroTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);

	switch (key)
	{
	case KEY_UP:
		Move(DIR_UP, heroTransform);
		break;
	case KEY_LEFT:
		Move(DIR_LEFT, heroTransform);
		break;
	case KEY_DOWN:
		Move(DIR_DOWN, heroTransform);
		break;
	case KEY_RIGHT:
		Move(DIR_RIGHT, heroTransform);
		break;
	default:
		break;
	}

	InventoryComponent *heroInventory = owner->GetComponentDirectly<InventoryComponent>(CTYPE_INVENTORY);

	if (heroInventory && heroInventory->skills.size() > 0)
	{
		for (auto skill = heroInventory->skills.begin(); skill != heroInventory->skills.end(); ++skill)
		{
			if ((*skill)->GetActivationButton() == key)
			{
				(*skill)->Apply(world);
			}
		}
	}
}

/////////////////
//  Collision  //
/////////////////

void Floyd::ScriptHero_OnCollision(World *world, Entity *owner, const Tile *collider)
{
	TransformComponent *heroTransform = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
	QuestInfoComponent *heroQuestInfo = owner->GetComponentDirectly<QuestInfoComponent>(CTYPE_QUEST_INFO);

	switch (collider->logicalSprite)
	{
	case TILE_WALL:
	case TILE_MONSTER_SPAWN:
	case TILE_EXIT_BLOCK:
		{
			heroTransform->GoToPrevPos();
		}
		break;
	case TILE_TELEPORT:
	case TILE_DREAMS:
		{
			// Hack for bug which causes the hero to be shown the end scene, even if the teleport
			// hasn't been revealed yet.
			if ((collider->logicalSprite == TILE_TELEPORT && collider->sprite == TILE_TELEPORT) ||
				 collider->logicalSprite == TILE_DREAMS)
			{
				world->GetCurrentLevel()->ShowEndscene();
			}
		}
		break;
	case TILE_NPC:
		{
			world->GetCurrentLevel()->ShowNPCscene();
			heroQuestInfo->hasTalkedToNPC = true;
			heroTransform->GoToPrevPos();
		}
		break;
	case TILE_STASH:
	case TILE_SHRINE:
		{
			if (world->IsItemAtPosActive(heroTransform->position))
			{
				Item itemAtPos = world->RetrieveItemAtPos(heroTransform->position);	
				owner->GetComponentDirectly<InventoryComponent>(CTYPE_INVENTORY)->AddItem(&itemAtPos);
			}

			if (collider->logicalSprite == TILE_STASH)
			{
				Level *currentLevel = world->GetCurrentLevel();
				if (currentLevel->AreThereMonsterSpawnPositions() && ! currentLevel->HasSpawnedMonstersForLevel())
				{
					currentLevel->SetIsExitDisplayConditionMet(true);
				}				
			}

			heroTransform->GoToPrevPos();
		}
		break;
	case TILE_MONSTER:
	case TILE_BOSS:
		{
			Position heroPos = heroTransform->position; // Makes sure the hero's not at the enemy's position.
			heroTransform->GoToPrevPos();

			auto entityToDamage = world->GetEntityAtPos(heroPos);
			if (entityToDamage)
			{
				StatComponent *heroStat = owner->GetComponentDirectly<StatComponent>(CTYPE_STAT);
				entityToDamage->GetComponentDirectly<StatComponent>(CTYPE_STAT)->ApplyDamage(heroStat->damage);
			}
		}
		break;
	case TILE_GO_DOWN:
	case TILE_GO_UP:
	case TILE_GO_LEFT:
	case TILE_GO_RIGHT:
		{
			Position entryPos =
				world->GetCurrentLevel()->GetNearestEntryPosForSprite(collider->sprite, heroTransform->position); 
							
			if (entryPos.IsPositive())
			{
				world->TeleportHeroToPosition(entryPos);
			}
		}
		break;
	case TILE_EXIT:
		{
			world->GoToNextLevel();
		}
		break;
	case TILE_KILL_BLOCK:
		{
			Position startingPos = world->GetCurrentLevel()->GetStartingPos();
			world->TeleportHeroToPosition(startingPos);
		}
		break;
	}
}