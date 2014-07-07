#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <thread>
#include <chrono>

#include "World.h"
#include "Dirs.h"
#include "Wrapper.h"
#include "Scripts.h"
#include "Reporting.h"


const int MANY_DAMAGE = 999;


std::string GetLevelName(const std::string &level)
{
	std::string safeLevel = Trim(level);

	// We assume that the level is the last number in the string
	size_t levelTokenPos = safeLevel.find_last_of(':');
	std::string levelName = safeLevel.substr(levelTokenPos + 1);
	return levelName;
}

std::vector<std::string> GetLevelArrayOfCutscenes(const std::string &level)
{
	std::string safeLevel = Trim(level);

	// We assume that the cutscene begins with 'c' and ends with a number
	// We don't differentiate between endScenes ('e'), cutScenes ('c') and npcScenes ('n')
	// We also add the cutscene extention		
	std::vector<std::string> result;

	if (safeLevel.find('c') == safeLevel.npos && 
		safeLevel.find('e') == safeLevel.npos &&
		safeLevel.find('n') == safeLevel.npos)
	{
		result.push_back("");
	}
	else
	{
		size_t currentCutsceneEndPos = safeLevel.find_first_of(':');
		size_t currentCutsceneStartPos = 0;
		std::string currentCutscene = safeLevel.substr(currentCutsceneStartPos, currentCutsceneEndPos);
		result.push_back(currentCutscene + EXT_LEVEL);
		while (safeLevel.find(":", currentCutsceneEndPos + 1) != safeLevel.npos)
		{
			currentCutsceneStartPos = currentCutsceneEndPos + 1;
			currentCutsceneEndPos = safeLevel.find_first_of(":", currentCutsceneStartPos);
			currentCutscene = safeLevel.substr(currentCutsceneStartPos, currentCutsceneEndPos - currentCutsceneStartPos);
			result.push_back(currentCutscene + EXT_LEVEL);
		}
	}
	return result;
}

std::pair<int, std::string> GetItemWithLevel(const std::string &item)
{
	std::string safeItem = Trim(item);

	// An item is of the following pair - <level_id>:<item_file_name>
	size_t itemNamePos = safeItem.find(':');
	std::string itemFileName = safeItem.substr(itemNamePos + 1);

	int itemLevel = 0;
	SafeLexicalCast(safeItem.substr(0, itemNamePos), itemLevel);

	return std::make_pair(itemLevel, itemFileName);
}

std::pair<std::string, std::string> GetItemStatPairFromField(const std::string &field)
{
	// An item field is of the follwing pair - <stat_id>:<stat_value>.
	size_t itemStatPos = field.find(':');
	std::string statID = field.substr(0, itemStatPos);
	std::string statVal = field.substr(itemStatPos + 1);
	return std::make_pair(statID, statVal);
}


/////////////
//  World  //
/////////////

World::World() : levels(0), currentLevelIdx(0), isRunning(true), currentState(STATE_MENU) {}

void World::Init()
{
	InitLevels();
	InitItemsForLevels();
	InitShrinesForLevels();

	startupMenu.Init(ResolveFileName(FILE_MENU_DEF, DIR_WORLD));

	///
	/// Adding scripts
	///
	scripts.push_back(new LevelScript());

	for (auto script = scripts.begin(); script != scripts.end(); ++script)
	{
		(*script)->OnStart(this);
	}

	Logger::Log("World initialized", LOW);
}

void World::OnFreshStart()
{
	currentLevelIdx = 0;
	//hero.Init(ResolveFileName(FILE_HERO_DEF, DIR_ENTITIES));

	//Position startingPos = levels[currentLevelIdx].GetStartingPos();
	//hero.SetInitialPosition(startingPos);

	/// Begin init hero
	std::shared_ptr<Entity> heroEnt = std::make_shared<Entity>();

	std::shared_ptr<MovableComponent> heroMovable = std::make_shared<MovableComponent>();
	heroMovable->position = levels[currentLevelIdx].GetStartingPos();
	heroMovable->prevPosition = heroMovable->position;
	heroMovable->prevTile = ' ';

	std::shared_ptr<ControllableComponent> heroControllable = std::make_shared<ControllableComponent>();

	std::shared_ptr<StatComponent> heroStat = std::make_shared<StatComponent>(30, 0, 5, 30);

	std::shared_ptr<InventoryComponent> heroInventory = std::make_shared<InventoryComponent>();

	std::shared_ptr<CollidableComponent> heroCollidable = std::make_shared<CollidableComponent>();

	std::shared_ptr<QuestInfoComponent> heroQuestInfo = std::make_shared<QuestInfoComponent>();

	heroEnt->AddComponent(heroMovable);
	heroEnt->AddComponent(heroControllable);
	heroEnt->AddComponent(heroStat);
	heroEnt->AddComponent(heroInventory);
	heroEnt->AddComponent(heroCollidable);
	heroEnt->AddComponent(heroQuestInfo);

	entities.push_back(heroEnt);
	/// End init hero

	InitLevelObjects();

	Logger::Log("World::OnFreshStart", LOW);
}

// TODO: Pass the save file name.
void World::OnSaveLoaded()
{
	Deserialize();

	Logger::Log("World::OnSaveLoaded", LOW);
}

void World::PollInput()
{
	if (_kbhit())
	{
		char key = static_cast<char>(_getch());
		
		switch (key)
		{
		case KEY_KILL_ALL:
			KillAllMonsters();
			break;
		case KEY_ESC:
			SwitchState(STATE_MENU);
			startupMenu.ShowButton("resumeGame");
			startupMenu.HideButton("newGame");
			break;
		default:
			break;
		}

		if (currentState == STATE_GAMEPLAY && ! levels[currentLevelIdx].HasActiveCutscenes())
		{
			// TODO: Full ECS implementation may be more suitable.
			auto controllables = GetComponentsOfType(CTYPE_CONTROLLABLE);
			for (auto ctrl = controllables.begin(); ctrl != controllables.end(); ++ctrl)
			{
				//(*ctrl)->Update(this);	
			}
			//hero.CheckInput(key, this);
		}
		else if (currentState == STATE_MENU)
		{
			startupMenu.OnKeyPressed(key, this);
		}
	}
}

void World::Update() 
{
	if (currentState == STATE_GAMEPLAY)
	{
		if (levels[currentLevelIdx].HasBegan())
		{
			for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
			{
				if (monster->GetHealth() <= 0)
				{
					Tile emptyTile(TILE_EMPTY, TILE_EMPTY, monster->GetPosition());
					levels[currentLevelIdx].SetTileAtPosition(monster->GetPosition(), emptyTile);
					monsters.erase(monster);
					break;
				}
				monster->Update(this);
			}
			for (auto particle = particles.begin(); particle != particles.end(); ++particle)
			{
				particle->Update();
			}
			if (GetHero()->GetComponentDirectly<StatComponent>(CTYPE_STAT)->health < 0)//hero.GetHealth() < 0)
			{
				// Show Game Over screen
			}
			UpdateCollisions();

			if (currentLevelIdx == BOSS_LEVEL && ! boss.IsDead())
			{
				if (boss.GetHealth() <= 0)
				{
					Tile emptyTile(TILE_EMPTY, TILE_EMPTY, boss.GetPosition());
					levels[currentLevelIdx].SetTileAtPosition(boss.GetPosition(), emptyTile);
					boss.SetIsDead(true);
				}
				boss.Update(this);
			}

			for (auto script = scripts.begin(); script != scripts.end(); ++script)
			{
				(*script)->OnUpdate(this);
			}
		}

		levels[currentLevelIdx].UpdateLevelMatrix(this);

		if (levels[currentLevelIdx].HasBegan())
		{
			UpdateCollisions();
		}
	}
	else if (currentState == STATE_MENU)
	{
		// Update menu.
	}
	else
	{
		std::stringstream error;
		error << "Invalid state '" << currentState << "' in World::Update\n";
		Report::UnexpectedError(error.str(), __LINE__, __FILE__);
	}
}

void World::Display()
{
	if (currentState == STATE_GAMEPLAY)
	{
		levels[currentLevelIdx].Display(this);
	}
	else if (currentState == STATE_MENU)
	{
		startupMenu.Display();
	}
	else
	{
		std::stringstream error;
		error << "Invalid state '" << currentState << "' in World::Display\n";
		Report::UnexpectedError(error.str(), __LINE__, __FILE__);
	}
}


void World::SwitchState(WorldState newState)
{
	currentState = newState;
}

void World::AddParticle(const Position &position, const Position &direction, int damage, 
						bool isEmittedFromHero)
{
	Particle newParticle;
	newParticle.SetPosition(position);

	newParticle.SetDirection(direction);
	newParticle.SetDamage(damage);
	newParticle.SetPrevTile(levels[currentLevelIdx].GetSpriteAtPosition(position));
	newParticle.SetIsEmittedFromHero(isEmittedFromHero);

	particles.push_back(newParticle);
}

///////////////////////////////////////////////////////////////////////////////

std::vector<std::shared_ptr<Entity>> World::GetEntitiesWithComponent(ComponentType cType)
{
	std::vector<std::shared_ptr<Entity>> result;

	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if ((*entity)->GetComponent(cType))
		{
			result.push_back((*entity));
		}
	}

	return result;
}

std::vector<IComponent*> World::GetComponentsOfType(ComponentType cType)
{
	std::vector<IComponent*> result;

	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		IComponent *component = (*entity)->GetComponent(cType);
		if (component)
		{
			result.push_back(component);
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////

Position World::GetPlayerPos()// const
{
	// TODO: Refactor
	Entity *heroEntity = GetHero();
	
	if (heroEntity)
	{
		MovableComponent *movable = heroEntity->GetComponentDirectly<MovableComponent>(CTYPE_MOVABLE);
		return movable->position;
	}
	else
	{
		std::cerr << "Error: No entity with Controllable component found\n";
		return Position();//hero.GetPosition(); 
	}
}
Position World::GetPlayerPrevPos()// const
{
	// TODO: Refactor
	Entity *heroEntity = GetHero();

	if (heroEntity)
	{
		MovableComponent *movable = heroEntity->GetComponentDirectly<MovableComponent>(CTYPE_MOVABLE);
		return movable->position;
	}
	else
	{
		std::cerr << "Error: No entity with Controllable component found\n";
		return Position();//hero.GetPosition(); 
	}
}

//Hero& World::GetHero()
//{
//	return hero;
//}
Entity* World::GetHero()
{
	Entity *heroEntity = GetEntitiesWithComponent(CTYPE_CONTROLLABLE)[0].get(); 

	if (heroEntity)
	{
		return heroEntity;
	}
	else
	{
		std::cerr << "Error: No Hero entity found\n";
		return nullptr;
	}
}

Boss& World::GetBoss()
{
	return boss;
}

std::vector<Monster>& World::GetMonsters()
{
	return monsters;
}
std::vector<Particle>& World::GetParticles()
{
	return particles;
}

bool World::IsRunning() const
{
	return isRunning;
}
void World::SetIsRunning(bool newIsRunning)
{
	isRunning = newIsRunning;
}

Monster* World::GetMonsterAtPos(const Position &position)
{
	for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
	{
		if (monster->GetPosition().IsEqual(position))
		{
			return &(*monster);
		}
	}

	return nullptr;
}

void World::SpawnMonsterAtPos(const Position &position)
{
	Monster newMonster;
	// Защо, юначе, убиваш ти тез чудовища? Защото не съм ги инитнал!
	// Very important step begins!!!
	newMonster.Init(ResolveFileName(FILE_MONSTER_DEF, DIR_ENTITIES));
	// Very important step ends!!!
	newMonster.SetInitialPosition(position);
	monsters.push_back(newMonster);
}

Item World::RetrieveItemAtPos(const Position &position)
{
	for (auto item = itemsInCurrentLevel.begin(); item != itemsInCurrentLevel.end(); ++item)
	{
		if (item->GetPosition().IsEqual(position))
		{
			item->SetIsActive(false);
			return (*item);
		}
	}

	Report::Error("Item not found at position", __LINE__, __FILE__);
	return Item("", -1, -1, -1, ATTRIB_NONE, Position(-1, -1), false);
}

bool World::IsItemAtPosActive(const Position &position) const
{
	for (auto item = itemsInCurrentLevel.begin(); item != itemsInCurrentLevel.end(); ++item)
	{
		if (item->GetPosition().IsEqual(position))
		{
			if (item->IsActive())
			{
				return true;
			}
			else return false;
		}
	}

	return false;
}

Level* World::GetCurrentLevel()
{
	return &levels[currentLevelIdx];
}

int World::GetCurrentLevelIdx()
{
	return currentLevelIdx;
}

bool World::AreMonstersDead() const
{
	if (monsters.empty() && ! levels[currentLevelIdx].AreThereMonsterSpawnPositions())
	{
		if (currentLevelIdx == BOSS_LEVEL)
		{
			if (boss.IsDead())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		return true;
	}
	else 
	{
		return false;
	}
}

void World::KillAllMonsters()
{
	for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
	{
		monster->ApplyDamage(MANY_DAMAGE);
	}
	for (auto particle = particles.begin(); particle != particles.end(); ++particle)
	{
		// TODO: Doesn't set the tile properly.
		levels[currentLevelIdx].SetSpriteAtPosition(particle->GetPosition(), TILE_EMPTY);
	}
	particles.clear();

	boss.ApplyDamage(MANY_DAMAGE);
}

void World::PrintInfo()// const
{
	// TODO: Refactor
	StatComponent *heroStat = static_cast<StatComponent*>(GetHero()->GetComponent(CTYPE_STAT));
	int heroHealth = heroStat->health;//hero.GetHealth();
	int heroDamage = heroStat->damage;//hero.GetDamage();
	int heroDefense = heroStat->defense;//hero.GetDefense();

	InventoryComponent *heroInventory = static_cast<InventoryComponent*>(GetHero()->GetComponent(CTYPE_INVENTOY));
	auto heroItems = heroInventory->ownedItemNames;//hero.GetItemNames();

	int bossHealth = -1;
	if (currentLevelIdx == BOSS_LEVEL)
	{
		bossHealth = boss.GetHealth();
	}

	std::cout << "\n\n";
	std::cout << "Health: " << heroHealth;
	if (bossHealth >= 0)
	{
		float bossHealth = float(boss.GetHealth()) / float(boss.GetMaxHealth());
		std::cout << "                       " << GetHealthBar(bossHealth);
	}
	std::cout << std::endl;
	std::cout << "Damage: " << heroDamage << '\n';
	std::cout << "Defense: " << heroDefense << '\n';

	size_t itemNamesSize = heroItems.size();
	if (itemNamesSize > 0)
	{
		std::cout << "Items: ";
		for (size_t idx = 0; idx < itemNamesSize; ++idx)
		{
			std::cout << heroItems[idx];
			if (idx < itemNamesSize - 1)
			{
				std::cout << ", ";
			}
		}
		std::cout << '\n';
	}
}

void World::Serialize()// const
{
	std::string saveFileName = ResolveFileName(FILE_WORLD_DEF, DIR_SAVE); 
	std::ofstream save(saveFileName, std::ios::binary);	

	if (save.is_open())
	{
		save.write((char*)&currentLevelIdx, sizeof(size_t));
		size_t monstersSize = monsters.size();
		save.write((char*)&monstersSize, sizeof(size_t));
		size_t particlesSize = particles.size();
		save.write((char*)&particlesSize, sizeof(size_t));
		size_t itemsInCurrentLevelSize = itemsInCurrentLevel.size();
		save.write((char*)&itemsInCurrentLevelSize, sizeof(size_t));

		for (size_t idx = 0; idx < monsters.size(); ++idx)
		{
			monsters[idx].Serialize(save);
		}
		for (size_t idx = 0; idx < particles.size(); ++idx)
		{
			particles[idx].Serialize(save);
		}
		for (size_t idx = 0; idx < itemsInCurrentLevel.size(); ++idx)
		{
			itemsInCurrentLevel[idx].Serialize(save);
		}

		//hero.Serialize(save);
		GetHero()->Serialize(save);
		if (currentLevelIdx == BOSS_LEVEL)
		{
			boss.Serialize(save);
		}

		levels[currentLevelIdx].Serialize(save);
	}
	else
	{
		std::stringstream error;
		error << "Can't serialize world from '" << saveFileName << "'\n";
		Report::UnexpectedError(error.str(), __LINE__, __FILE__);
	}

	save.close();
}

void World::Deserialize()
{
	std::string loadFileName = ResolveFileName(FILE_WORLD_DEF, DIR_SAVE); 
	std::ifstream load(loadFileName, std::ios::binary);	

	if (load.is_open())
	{
		load.read((char*)&currentLevelIdx, sizeof(size_t));
		size_t monstersCount = 0;
		load.read((char*)&monstersCount, sizeof(size_t));
		size_t particlesCount = 0;
		load.read((char*)&particlesCount, sizeof(size_t));
		size_t itemsCount = 0;
		load.read((char*)&itemsCount, sizeof(size_t));

		for (size_t idx = 0; idx < monstersCount; ++idx)
		{
			Monster newMonster;
			newMonster.Deserialize(load);
			monsters.push_back(newMonster);
		}
		for (size_t idx = 0; idx < particlesCount; ++idx)
		{
			Particle newParticle;
			newParticle.Deserialize(load);
			particles.push_back(newParticle);
		}
		for (size_t idx = 0; idx < itemsCount; ++idx)
		{
			Item newItem;
			newItem.Deserialize(load);
			itemsInCurrentLevel.push_back(newItem);
		}

		//hero.Deserialize(load);
		GetHero()->Deserialize(load);
		if (currentLevelIdx == BOSS_LEVEL)
		{
			boss.Deserialize(load);
		}

		levels[currentLevelIdx].Deserialize(load);
	}
	else
	{
		std::stringstream error;
		error << "Can't deserialize world from '" << loadFileName << "'\n";
		Report::UnexpectedError(error.str(), __LINE__, __FILE__);
	}

	load.close();
}

///////////////////////
//  Private Methods  //
///////////////////////

void World::UpdateCollisions()
{
	//CheckHeroCollision();
	//GetHero()->GetComponentDirectly<CollidableComponent>(CTYPE_COLLIDABLE)->Update(this);

	CheckMonsterCollision();
	CheckParticleCollision();
	CheckBossCollision();
}

void World::CheckHeroCollision()
{
	LevelMap currentMap = levels[currentLevelIdx].GetMap();
	Position currentHeroPos = hero.GetPosition();
	Tile currentTile = currentMap.GetTileAtPosition(currentHeroPos);

	if ( ! levels[currentLevelIdx].IsPositionInsideMap(currentHeroPos))
	{
		Position startingPos = levels[currentLevelIdx].GetStartingPos();
		TeleportHeroToPosition(startingPos);
	}
 
	switch (currentTile.logicalSprite)
	{
	case TILE_WALL:
	case TILE_MONSTER_SPAWN:
	case TILE_EXIT_BLOCK:
		{
			hero.GoToPrevPos();
		}
		break;
	case TILE_TELEPORT:
	case TILE_DREAMS:
		{
			// Hack for bug which causes the hero to be shown the end scene, even if the teleport
			// hasn't been revealed yet.
			if ((currentTile.logicalSprite == TILE_TELEPORT && 
				currentTile.sprite == TILE_TELEPORT) || currentTile.logicalSprite == TILE_DREAMS)
			{
				levels[currentLevelIdx].ShowEndscene();
			}
		}
		break;
	case TILE_NPC:
		{
			levels[currentLevelIdx].ShowNPCscene();
			hero.SetHasTalkedToNPC(true);
			hero.GoToPrevPos();
		}
		break;
	case TILE_STASH:
		{
			hero.GoToPrevPos();

			if (IsItemAtPosActive(currentHeroPos))
			{
				Item itemAtPos = RetrieveItemAtPos(currentHeroPos);
				hero.AddItem(&itemAtPos);
			}

			if (levels[currentLevelIdx].AreThereMonsterSpawnPositions() &&
				!levels[currentLevelIdx].HasSpawnedMonstersForLevel())
			{
				levels[currentLevelIdx].SetIsExitDisplayConditionMet(true);
			}
		}
		break;
	case TILE_SHRINE:
		{
			hero.GoToPrevPos();

			if (IsItemAtPosActive(currentHeroPos))
			{
				Item shrineAtPos = RetrieveItemAtPos(currentHeroPos);
				hero.AddBuff(&shrineAtPos);
			}
		}
		break;
	case TILE_MONSTER:
		{
			Monster *currentMonster = GetMonsterAtPos(currentHeroPos);
			if (currentMonster)
			{
				currentMonster->ApplyDamage(hero.GetDamage());
			}
			hero.GoToPrevPos();
		}
		break;
	case TILE_BOSS:
		{
			boss.ApplyDamage(hero.GetDamage());
			hero.GoToPrevPos();
		}
		break;
	case TILE_GO_DOWN:
	case TILE_GO_UP:
	case TILE_GO_LEFT:
	case TILE_GO_RIGHT:
		{
			Position entryPos = levels[currentLevelIdx].GetNearestEntryPosForSprite(currentTile.sprite, 
																				    currentHeroPos);
			if (entryPos.IsPositive())
			{
				TeleportHeroToPosition(entryPos);
				hero.SetPrevTile(levels[currentLevelIdx].GetSpriteAtPosition(entryPos));
			}
		}
		break;
	case TILE_EXIT:
		{
			currentLevelIdx++;
			levels[currentLevelIdx].ResetLastCutsceneInterval();
			Position startingPos = levels[currentLevelIdx].GetStartingPos();
			hero.SetInitialPosition(startingPos);
			hero.SetPrevTile(levels[currentLevelIdx].GetSpriteAtPosition(startingPos));
			monsters.clear();
			particles.clear();
			InitLevelObjects();
		}
		break;
	case TILE_KILL_BLOCK:
		{
			// hero.Hurt(MANY_DAMAGE);
			Position startingPos = levels[currentLevelIdx].GetStartingPos();
			TeleportHeroToPosition(startingPos);
			// Show "Game Over" screen
		}
		break;
	}
}

void World::CheckMonsterCollision()
{
	LevelMap currentMap = levels[currentLevelIdx].GetMap();
	for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
	{
		Position monsterPos = monster->GetPosition();
		char monsterTile = currentMap.GetTileAtPosition(monsterPos).sprite;
		switch (monsterTile)
		{
		case TILE_WALL:
			monster->GoToPrevPos();
			break;
		case TILE_HERO:
			//monster->GoToPrevPos();
			break;
		}
	}
}

void World::CheckParticleCollision()
{
	LevelMap currentMap = levels[currentLevelIdx].GetMap();
	auto particle = particles.begin();
	while (particle != particles.end())
	{
		Position particlePos = particle->GetPosition();
		
		if ( ! levels[currentLevelIdx].IsPositionInsideMap(particlePos))
		{
			levels[currentLevelIdx].SetSpriteAtPosition(particle->GetPrevPos(), 
													    particle->GetPrevTile());
			particle = particles.erase(particle);
			break;
		}

		Tile particleTile = currentMap.GetTileAtPosition(particlePos);
		if (particleTile.sprite == TILE_WALL || particleTile.sprite == TILE_MONSTER || 
			particleTile.sprite == TILE_STASH || particleTile.sprite == TILE_NPC || 
			particleTile.sprite == TILE_TELEPORT || particleTile.sprite == TILE_DREAMS || 
			particleTile.sprite == TILE_EXIT || particleTile.sprite == TILE_HERO || 
			particleTile.sprite == TILE_BOSS)
		{
			if (particleTile.sprite == TILE_HERO) 
			{
				hero.Hurt(particle->GetDamage());
			}
			else if (particleTile.sprite == TILE_MONSTER && particle->IsEmittedFromHero())
			{
				GetMonsterAtPos(particlePos)->ApplyDamage(particle->GetDamage());
			}
			else if (particleTile.sprite == TILE_BOSS && particle->IsEmittedFromHero())
			{
				boss.ApplyDamage(particle->GetDamage());
			}

			// Particles get destroyed when they hit an object.
			levels[currentLevelIdx].SetSpriteAtPosition(particle->GetPrevPos(), 
														particle->GetPrevTile());
			particle = particles.erase(particle);
		}
		else
		{
			++particle;
		}
	}
}

void World::CheckBossCollision()
{
	LevelMap currentMap = levels[currentLevelIdx].GetMap();
	Position currentBossPos = boss.GetPosition();
	Tile currentTile = currentMap.GetTileAtPosition(currentBossPos);

	switch (currentTile.sprite)
	{
	case TILE_HERO:
		{
			//hero.Hurt(boss.GetDamage());
		}
		break;
	}
}

void World::InitLevelObjects()
{
	monsters.clear();
	particles.clear();

	LevelMap map = levels[currentLevelIdx].GetMap();

	auto monsterTiles = map.GetTilesForLogicalSprite(TILE_MONSTER);
	for (auto monster = monsterTiles.begin(); monster != monsterTiles.end(); ++monster)
	{
		Monster newMonster;
		newMonster.Init(ResolveFileName(FILE_MONSTER_DEF, DIR_ENTITIES));
		newMonster.SetInitialPosition(monster->position);
		monsters.push_back(newMonster);
	}

	auto itemPair = itemsForLevel.find(currentLevelIdx + 1); // We use indices corresponding to the 
														     // level files' names
	if (itemPair != itemsForLevel.end())
	{
		auto itemFileNames = itemPair->second;
		for (auto itemFileName = itemFileNames.begin(); itemFileName != itemFileNames.end(); ++itemFileName)
		{
			// Magnificent loading takes place!!!
			InitItemFromFile((*itemFileName));		
		}
	}

	auto shrinePair = shrinesForLevel.find(currentLevelIdx + 1);
	if (shrinePair != shrinesForLevel.end())
	{
		auto shrineFileNames = shrinePair->second;
		for (auto shrineFileName = shrineFileNames.begin(); shrineFileName != shrineFileNames.end(); ++shrineFileName)
		{
			// Magnificent loading takes place!!!
			InitItemFromFile((*shrineFileName));
		}
	}

	if (map.HasTileWithLogicalSprite(TILE_BOSS))
	{
		auto bossTile = map.GetTilesForLogicalSprite(TILE_BOSS)[0];
		boss.Init(ResolveFileName(FILE_BOSS_DEF, DIR_ENTITIES));
		boss.SetInitialPosition(bossTile.position);
	}
}

// TODO: Create a simple wrapper around files? Maybe no... Simple ain't always simple.
void World::InitItemFromFile(const std::string &fileName)
{
	int itemDamage = 0;
	int itemDefense = 0;
	int itemHealth = 0;
	ItemAttribute itemAttribute = ATTRIB_NONE;
	std::string itemName;
	Position itemPos(0, 0);

	std::ifstream item(ResolveFileName(fileName, DIR_ENTITIES));

	if (item.is_open())
	{
		std::string itemField;
		while (std::getline(item, itemField).good())
		{
			auto statPair = GetItemStatPairFromField(itemField);

			std::string statID = statPair.first;
			std::string statVal = statPair.second;
			if (statID == "damage")
			{
				SafeLexicalCast<int>(statVal, itemDamage);
			}
			else if (statID == "defense")
			{
				SafeLexicalCast<int>(statVal, itemDefense);
			}
			else if (statID == "health")
			{
				SafeLexicalCast<int>(statVal, itemHealth);
			}
			else if (statID == "name")
			{
				itemName = statVal;
			}
			else if (statID == "attrib")
			{
				if (statVal == "PARTICLE")
				{
					itemAttribute = ATTRIB_PARTICLE;
				}
			}
			else if (statID == "position")
			{
				size_t delimPos = statVal.find(',');
				SafeLexicalCast<int>(statVal.substr(0, delimPos), itemPos.x);
				SafeLexicalCast<int>(statVal.substr(delimPos + 1), itemPos.y);
			}
		}
	}

	item.close();

	Item newItem(itemName, itemDefense, itemDamage, itemHealth, itemAttribute, itemPos, true);

	assert(levels[currentLevelIdx].GetSpriteAtPosition(itemPos) == 'O' ||
		   levels[currentLevelIdx].GetSpriteAtPosition(itemPos) == 'I');
	
	itemsInCurrentLevel.push_back(newItem);
}

void World::InitLevels()
{
	levels.clear();

	std::ifstream world(ResolveFileName(FILE_WORLD_DEF, DIR_WORLD)); 

	if (world.is_open())
	{
		std::string levelDef;
		while (std::getline(world, levelDef, ';').good())
		{
			Level newLevel;
			std::string levelName = GetLevelName(levelDef); 
			newLevel.Init(levelName + EXT_LEVEL);
			std::vector<std::string> cutscenes = GetLevelArrayOfCutscenes(levelDef);
			newLevel.InitCutscenes(cutscenes);
			levels.push_back(newLevel);
		}
	}
	else
	{
		Report::UnexpectedError("Can't open world file", __LINE__, __FILE__);
	}

	world.close();
}

void World::InitItemsForLevels()
{
	itemsForLevel.clear();

	std::ifstream items(ResolveFileName(FILE_ITEMS_DEF, DIR_ENTITIES));

	if (items.is_open())
	{
		std::string itemDef;
		while (std::getline(items, itemDef, ';').good())
		{
			auto itemPair = GetItemWithLevel(itemDef);
			auto placeToAddItemPair = itemsForLevel.find(itemPair.first); 
			if (placeToAddItemPair != itemsForLevel.end())
			{
				placeToAddItemPair->second.push_back(itemPair.second);
			}
			else
			{
				std::vector<std::string> firstItem;
				firstItem.push_back(itemPair.second);
				itemsForLevel.insert(std::make_pair(itemPair.first, firstItem));
			}
		}
	}
	else
	{
		Report::UnexpectedError("Can't open items file", __LINE__, __FILE__);
	}

	items.close();
}

void World::InitShrinesForLevels()
{
	shrinesForLevel.clear();

	std::ifstream shrines(ResolveFileName(FILE_SHRINES_DEF, DIR_ENTITIES));

	if (shrines.is_open())
	{
		std::string shrineDef;
		while (std::getline(shrines, shrineDef, ';').good())
		{
			auto shrinePair = GetItemWithLevel(shrineDef);
			auto placeToAddShrinePair = shrinesForLevel.find(shrinePair.first); 
			if (placeToAddShrinePair != shrinesForLevel.end())
			{
				placeToAddShrinePair->second.push_back(shrinePair.second);
			}
			else
			{
				std::vector<std::string> firstShrine;
				firstShrine.push_back(shrinePair.second);
				shrinesForLevel.insert(std::make_pair(shrinePair.first, firstShrine));
			}
		}
	}
	else
	{
		Report::UnexpectedError("Can't open shrines file", __LINE__, __FILE__);
	}

	shrines.close();
}

void World::TeleportHeroToPosition(const Position &newPosition)
{
	MovableComponent *heroMovable = GetHero()->GetComponentDirectly<MovableComponent>(CTYPE_MOVABLE);

	Position currentHeroPos = heroMovable->position;//hero.GetPosition();
	Tile currentTile = levels[currentLevelIdx].GetMap().GetTileAtPosition(currentHeroPos);

	// 02-Jun-2014: Yes, I will. Due to the lack of layers I have to remove the player sprite 
	//				from the current position.
	levels[currentLevelIdx].SetSpriteAtPosition(heroMovable->position, currentTile.sprite);
	levels[currentLevelIdx].SetSpriteAtPosition(heroMovable->prevPosition, heroMovable->prevTile);

	// 01-Jun-2014: Will you break the space-time continuum?
	//hero.SetInitialPosition(newPosition);
	heroMovable->position = newPosition;
	heroMovable->prevPosition = newPosition;
}

World::~World()
{
	for (auto iter = scripts.begin(); iter != scripts.end(); ++iter)
	{
		delete (*iter);
	}
	scripts.clear();

	Logger::Log("Destroying World", LOW);
}