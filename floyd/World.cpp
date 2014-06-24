#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include "World.h"
#include "Dirs.h"
#include "Wrapper.h"
#include "Scripts.h"


const int MANY_DAMAGE = 999;


std::string GetLevelName(const std::string &level)
{
	// TODO: Trim all whitespaces
	
	// We assume that the level is the last number in the string
	size_t levelTokenPos = level.find_last_of(':');
	std::string levelName = level.substr(levelTokenPos + 1);
	return levelName;
}

std::vector<std::string> GetLevelArrayOfCutscenes(const std::string &level)
{
	// TODO: Trim all whitespaces

	// We assume that the cutscene begins with 'c' and ends with a number
	// We don't differentiate between endScenes ('e'), cutScenes ('c') and npcScenes ('n')
	// We also add the cutscene extention		
	std::vector<std::string> result;

	if (level.find('c') == level.npos && 
		level.find('e') == level.npos &&
		level.find('n') == level.npos)
	{
		result.push_back("");
	}
	else
	{
		size_t currentCutsceneEndPos = level.find_first_of(':');
		size_t currentCutsceneStartPos = 0;
		std::string currentCutscene = level.substr(currentCutsceneStartPos, currentCutsceneEndPos);
		result.push_back(currentCutscene + EXT_LEVEL);
		while (level.find(":", currentCutsceneEndPos + 1) != level.npos)
		{
			currentCutsceneStartPos = currentCutsceneEndPos + 1;
			currentCutsceneEndPos = level.find_first_of(":", currentCutsceneStartPos);
			currentCutscene = level.substr(currentCutsceneStartPos, currentCutsceneEndPos - currentCutsceneStartPos);
			result.push_back(currentCutscene + EXT_LEVEL);
		}
	}
	return result;
}

std::pair<int, std::string> GetItemWithLevel(const std::string &item)
{
	// An item is of the following pair - <level_id>:<item_file_name>
	size_t itemNamePos = item.find(':');
	std::string itemFileName = item.substr(itemNamePos + 1);

	int itemLevel = 0;
	SafeLexicalCast(item.substr(0, itemNamePos), itemLevel);

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

World::World() : levels(0), currentLevelIdx(0) {}

void World::Init()
{
	InitLevels();
	InitItemsForLevels();
	InitShrinesForLevels();

	hero.Init(ResolveFileName(FILE_HERO_DEF, DIR_ENTITIES));

	Position startingPos = levels[currentLevelIdx].GetStartingPos();
	hero.SetInitialPosition(startingPos);

	InitLevelObjects();

	///
	/// Adding scripts
	///
	scripts.push_back(new LevelScript());

	for (auto script = scripts.begin(); script != scripts.end(); ++script)
	{
		(*script)->OnStart(this);
	}
}

void World::Display()
{
	levels[currentLevelIdx].Display(this);
}

void World::PollInput()
{
	if (hero.GetHealth() <= 0)
	{
		return;
	}

	if (_kbhit())
	{
		char dir = static_cast<char>(_getch());
		
		switch (dir)
		{
		case KEY_UP:
			hero.Move(DIR_UP);
			break;
		case KEY_LEFT:
			hero.Move(DIR_LEFT);
			break;
		case KEY_DOWN:
			hero.Move(DIR_DOWN);
			break;
		case KEY_RIGHT:
			hero.Move(DIR_RIGHT);
			break;
		case KEY_QUIT:
			std::cout << "QUIT\n";
			exit(1);
			break;
		case KEY_KILL_ALL:
			KillAllMonsters();
		default:
			break;
		}

		// TODO: Put code from the switch here
		hero.CheckInput(dir, this);
	}
}

void World::Update() 
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
		if (hero.GetHealth() < 0)
		{
			// Show Game Over screen
		}
		UpdateCollisions();

		if (currentLevelIdx == 6 && ! boss.IsDead()) // Level index of boss level
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

Position World::GetPlayerPos() const
{
	return hero.GetPosition(); 
}
Position World::GetPlayerPrevPos() const
{
	return hero.GetPrevPos();
}

Hero& World::GetHero()
{
	return hero;
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
			Item foundItem(item->GetName(), item->GetDefense(), item->GetDamage(), item->GetHealth(), 
						   item->GetAttribute(), item->GetPosition());
			itemsInCurrentLevel.erase(item);
			return foundItem;
		}
	}

	std::cerr << "Error: Item not found at position\n";
	return Item("", -1, -1, -1, ATTRIB_NONE, Position(-1, -1));
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
	return monsters.empty() && (! levels[currentLevelIdx].AreThereMonsterSpawnPositions());
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
}

void World::PrintInfo() const
{
	int heroHealth = hero.GetHealth();
	int heroDamage = hero.GetDamage();
	int heroDefense = hero.GetDefense();
	auto heroItems = hero.GetItemNames();

	int bossHealth = -1;
	if (currentLevelIdx == 6) // boss level
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

///////////////////////
//  Private Methods  //
///////////////////////

void World::UpdateCollisions()
{
	CheckHeroCollision();
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
		{
			hero.GoToPrevPos();
		}
		break;
	case TILE_TELEPORT:
	case TILE_DREAMS:
		{
			levels[currentLevelIdx].ShowEndscene();
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

			Item itemAtPos = RetrieveItemAtPos(currentHeroPos);
			hero.AddItem(&itemAtPos);

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

			Item shrineAtPos = RetrieveItemAtPos(currentHeroPos);
			hero.AddBuff(&shrineAtPos);
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
	Position currentHeroPos = hero.GetPosition();
	Tile currentTile = currentMap.GetTileAtPosition(currentHeroPos);

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

	Item newItem(itemName, itemDefense, itemDamage, itemHealth, itemAttribute, itemPos);

	assert(levels[currentLevelIdx].GetSpriteAtPosition(itemPos) == 'O' ||
		   levels[currentLevelIdx].GetSpriteAtPosition(itemPos) == 'I');
	
	itemsInCurrentLevel.push_back(newItem);
}

void World::InitLevels()
{
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
		std::cerr << "Error: Opening world file\n";
	}

	world.close();
}

void World::InitItemsForLevels()
{
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
		std::cerr << "Error: Opening items file\n";
	}

	items.close();
}

void World::InitShrinesForLevels()
{
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
		std::cerr << "Error: Opening items file\n";
	}

	shrines.close();
}

void World::TeleportHeroToPosition(const Position &newPosition)
{
	Position currentHeroPos = hero.GetPosition();
	Tile currentTile = levels[currentLevelIdx].GetMap().GetTileAtPosition(currentHeroPos);

	// 02-Jun-2014: Yes, I will. Due to the lack of layers I have to remove the player sprite 
	//				from the current position.
	levels[currentLevelIdx].SetSpriteAtPosition(hero.GetPosition(), currentTile.sprite);
	levels[currentLevelIdx].SetSpriteAtPosition(hero.GetPrevPos(), hero.GetPrevTile());

	// 01-Jun-2014: Will you break the space-time continuum?
	hero.SetInitialPosition(newPosition);
}

World::~World()
{
	for (auto iter = scripts.begin(); iter != scripts.end(); ++iter)
	{
		delete (*iter);
	}
	scripts.clear();
}