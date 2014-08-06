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
#include "Floyd_Scripts/ScriptsHero.h"
#include "Floyd_Scripts/ScriptsMonster.h"
#include "Floyd_Scripts/ScriptsParticle.h"
#include "Floyd_Scripts/ScriptsBoss.h"


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

	CreateHero();

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
			RemoveAIEntities();
			break;
		case KEY_P:
			GoToNextLevel();
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
			auto controllables = GetComponentsOfType(CTYPE_CONTROLLABLE);
			for (auto ctrl = controllables.begin(); ctrl != controllables.end(); ++ctrl)
			{
				static_cast<ControllableComponent*>((*ctrl))->script((*ctrl)->owner, key);
			}
		}
		else if (currentState == STATE_MENU)
		{
			startupMenu.OnKeyPressed(key, this);
		}
	}
}

Tile World::GetTileAtPositionForCollision(const Position &position, const CollidableComponent *collidable) const
{
	auto colliders = GetComponentsOfType_const(CTYPE_COLLIDABLE);
	for (auto collider = colliders.begin(); collider != colliders.end(); ++collider)
	{
		const CollidableComponent *cleanCollider = static_cast<const CollidableComponent*>(*collider);
		if (cleanCollider->collisionInfo[0] != collidable->collisionInfo[0] &&
			cleanCollider->collisionInfo[1] != collidable->collisionInfo[1])
		{
			const TransformComponent *colliderTransform =
				cleanCollider->owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
			if (colliderTransform && colliderTransform->position.IsEqual(position))
			{
				return Tile(cleanCollider->collisionInfo[1], cleanCollider->collisionInfo[0], position);
			}
		}
	}

	return levels[currentLevelIdx].GetMap().GetTileAtPosition(position);
}

void World::Update() 
{
	if (currentState == STATE_GAMEPLAY)
	{
		levels[currentLevelIdx].Update();

		if ( ! levels[currentLevelIdx].HasActiveCutscenes())
		{
			// TODO: Components should pass owners automatically.
			auto collidables = GetComponentsOfType(CTYPE_COLLIDABLE);
			for (auto collidable = collidables.begin(); collidable != collidables.end(); ++collidable)
			{
				Entity *owner = (*collidable)->owner;
				// TODO: Check strange bug which crashes the game when we proceed to the next level without killing 
				//		 the monsters.
				Position ownerPos = owner->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM)->position;
				Tile tileUnderOwner = GetTileAtPositionForCollision(ownerPos, static_cast<CollidableComponent*>((*collidable)));
					//levels[currentLevelIdx].GetMap().GetTileAtPosition(ownerPos);
				CollidableComponent *cleanCollidable = static_cast<CollidableComponent*>((*collidable));
				if (cleanCollidable->onCollision)
				{
					cleanCollidable->onCollision(this, owner, &tileUnderOwner);
				}
			}

			auto aiControlled = GetComponentsOfType(CTYPE_AI);
			for (auto ai = aiControlled.begin(); ai != aiControlled.end(); ++ai)
			{
				Entity *owner = (*ai)->owner;
				static_cast<AIComponent*>((*ai))->onUpdateAI(this, owner);
			}

			for (auto script = scripts.begin(); script != scripts.end(); ++script)
			{
				(*script)->OnUpdate(this);
			}

			RemoveDeadEntities();
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

void World::SwitchState(WorldState newState)
{
	currentState = newState;
}
WorldState World::GetState() const
{
	return currentState;
}

//void World::AddParticle(const Position &position, const Position &direction, int damage, 
//						bool isEmittedFromHero)
//{
//	Particle newParticle;
//	newParticle.SetPosition(position);

//	newParticle.SetDirection(direction);
//	newParticle.SetDamage(damage);
//	newParticle.SetPrevTile(levels[currentLevelIdx].GetSpriteAtPosition(position));
//	newParticle.SetIsEmittedFromHero(isEmittedFromHero);

//	particles.push_back(newParticle);
//}

////////////////////////////////////////////////////////////////////////////////

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

std::vector<const std::shared_ptr<Entity>> World::GetEntitiesWithComponent_const(ComponentType cType) const
{
	std::vector<const std::shared_ptr<Entity>> result;

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

std::vector<const IComponent*> World::GetComponentsOfType_const(ComponentType cType) const
{
	std::vector<const IComponent*> result;

	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		const IComponent *component = (*entity)->GetComponent(cType);
		if (component)
		{
			result.push_back(component);
		}
	}

	return result;
}

std::shared_ptr<Entity> World::GetEntityAtPos(const Position &pos)
{
	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		Position entityPos = (*entity)->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM)->position;
		if (entityPos.IsEqual(pos))
		{
			return (*entity);
		}
	}

	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////

Position World::GetPlayerPos()// const
{
	// TODO: Refactor
	Entity *heroEntity = GetHero();
	
	if (heroEntity)
	{
		TransformComponent *transform = heroEntity->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
		return transform->position;
	}
	else
	{
		std::cerr << "Error: No entity with Transform component found\n";
		return Position();//hero.GetPosition(); 
	}
}
Position World::GetPlayerPrevPos()// const
{
	// TODO: Refactor
	Entity *heroEntity = GetHero();

	if (heroEntity)
	{
		TransformComponent *transform = heroEntity->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
		return transform->position;
	}
	else
	{
		std::cerr << "Error: No entity with Transform component found\n";
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

const Entity* World::GetHero_const() const
{
	const Entity *heroEntity = GetEntitiesWithComponent_const(CTYPE_CONTROLLABLE)[0].get(); 

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

bool World::IsRunning() const
{
	return isRunning;
}
void World::SetIsRunning(bool newIsRunning)
{
	isRunning = newIsRunning;
}

//Monster* World::GetMonsterAtPos(const Position &position)
//{
//	for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
//	{
//		if (monster->GetPosition().IsEqual(position))
//		{
//			return &(*monster);
//		}
//	}

//	return nullptr;
//}

void World::SpawnMonsterAtPos(const Position &position)
{
	CreateMonster(position);
	//Monster newMonster;
	//// Защо, юначе, убиваш ти тез чудовища? Защото не съм ги инитнал!
	//// Very important step begins!!!
	//newMonster.Init(ResolveFileName(FILE_MONSTER_DEF, DIR_ENTITIES));
	//// Very important step ends!!!
	//newMonster.SetInitialPosition(position);
	//monsters.push_back(newMonster);
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
	return Item("", -1, -1, -1, ATTRIB_NONE, Position(-1, -1), false, false);
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

void World::GoToNextLevel()
{
	currentLevelIdx++;
	levels[currentLevelIdx].ResetLastCutsceneInterval();
	Position startingPos = levels[currentLevelIdx].GetStartingPos();
	TransformComponent *heroTransform = GetHero()->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
	heroTransform->prevPosition = startingPos;
	heroTransform->position = startingPos;
	//monsters.clear();
	//particles.clear();
	RemoveAIEntities();
	InitLevelObjects();
}

std::vector<std::pair<const Sprite*, Position>> World::GetSpritesForDrawing() const
{
	std::vector<std::pair<const Sprite*, Position>> sprites;

	switch (currentState)
	{
	case STATE_GAMEPLAY:
		{
			const Sprite *levelSprite = levels[currentLevelIdx].GetMapAsSprite();
			sprites.push_back(std::make_pair(levelSprite, Position(0, 0)));

			if ( ! levels[currentLevelIdx].HasActiveCutscenes())
			{
				auto drawables = GetEntitiesWithComponent_const(CTYPE_DRAWABLE);
				for (auto drawable = drawables.begin(); drawable != drawables.end(); ++drawable)
				{
					TransformComponent *transform = (*drawable)->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);
					if (transform)
					{
						DrawableComponent *draw = (*drawable)->GetComponentDirectly<DrawableComponent>(CTYPE_DRAWABLE);
						sprites.push_back(std::make_pair(&draw->sprite, transform->position));
					}
				}

				const Sprite* infoAsSprite =
					GetHero_const()->GetComponentDirectly<InventoryComponent>(CTYPE_INVENTORY)->GetInfoAsSprite();
				sprites.push_back(std::make_pair(infoAsSprite, Position(0, 21)));
			}
		}
		break;
	case STATE_MENU:
		{
			auto buttons = startupMenu.GetButtonsForDrawing();
			for (auto button = buttons.begin(); button != buttons.end(); ++button)
			{
				sprites.push_back(std::make_pair(button->second->GetSprite(),
												 Position(0, button->first)));
			}
		}
		break;
	default:
		Report::UnexpectedError("The world is in no state.", __LINE__, __FILE__);
		break;
	}

	if (sprites.empty())
	{
		Report::Error("No drawable objects in World!", __LINE__, __FILE__);
	}

	return sprites;
}

bool World::AreMonstersDead() const
{
	if (this->GetComponentsOfType_const(CTYPE_AI).empty()) // What if not all AIs are bad? 
														   // Take **particles** for example!
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void World::KillAllMonsters()
{
	//for (auto monster = monsters.begin(); monster != monsters.end(); ++monster)
	//{
	//	monster->ApplyDamage(MANY_DAMAGE);
	//}
	//for (auto particle = particles.begin(); particle != particles.end(); ++particle)
	//{
	//	// TODO: Doesn't set the tile properly.
	//	levels[currentLevelIdx].SetSpriteAtPosition(particle->GetPosition(), TILE_EMPTY);
	//}
	//particles.clear();

	//boss.ApplyDamage(MANY_DAMAGE);
}

// TODO: Refactor
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

// TODO: Refactor
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

// TODO: Items should get their positions from the level file
void World::InitLevelObjects()
{
	RemoveAIEntities();

	LevelMap map = levels[currentLevelIdx].GetMap();

	auto monsterTiles = map.GetTilesForLogicalSprite(TILE_MONSTER);
	for (auto monster = monsterTiles.begin(); monster != monsterTiles.end(); ++monster)
	{
		CreateMonster(monster->position);
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
		CreateBoss(map.GetTilesForLogicalSprite(TILE_BOSS)[0].position); // TODO: Unsafe. Make better.

		//auto bossTile = map.GetTilesForLogicalSprite(TILE_BOSS)[0];
		//boss.Init(ResolveFileName(FILE_BOSS_DEF, DIR_ENTITIES));
		//boss.SetInitialPosition(bossTile.position);
	}

	levels[currentLevelIdx].RemoveWorldSpecificTiles();
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

	assert(levels[currentLevelIdx].GetSpriteAtPosition(itemPos) == 'O' ||
		   levels[currentLevelIdx].GetSpriteAtPosition(itemPos) == 'I');

	bool isBuff = levels[currentLevelIdx].GetSpriteAtPosition(itemPos) == 'I' ? true : false;

	Item newItem(itemName, itemDefense, itemDamage, itemHealth, itemAttribute, itemPos, true, isBuff);
	
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

void World::CreateHero()
{
	std::shared_ptr<Entity> heroEnt = std::make_shared<Entity>();

	std::shared_ptr<TransformComponent> heroTransform = std::make_shared<TransformComponent>();
	heroTransform->position = levels[currentLevelIdx].GetStartingPos();
	heroTransform->prevPosition = heroTransform->position;

	std::shared_ptr<ControllableComponent> heroControllable = std::make_shared<ControllableComponent>();
	heroControllable->script = Floyd::ScriptHero_OnKeyPressed;

	std::shared_ptr<StatComponent> heroStat = std::make_shared<StatComponent>(30, 0, 5, 30);

	std::shared_ptr<InventoryComponent> heroInventory = std::make_shared<InventoryComponent>();

	std::shared_ptr<CollidableComponent> heroCollidable = std::make_shared<CollidableComponent>();
	heroCollidable->collisionInfo[0] = '|';
	heroCollidable->collisionInfo[1] = '|';
	heroCollidable->onCollision = Floyd::ScriptHero_OnCollision;

	std::shared_ptr<QuestInfoComponent> heroQuestInfo = std::make_shared<QuestInfoComponent>();

	std::shared_ptr<DrawableComponent> heroDrawable = std::make_shared<DrawableComponent>();
	heroDrawable->sprite = Sprite(1, 1);
	heroDrawable->sprite.LoadTextureFromRawData("|\n");

	heroEnt->AddComponent(heroTransform);
	heroEnt->AddComponent(heroControllable);
	heroEnt->AddComponent(heroStat);
	heroEnt->AddComponent(heroInventory);
	heroEnt->AddComponent(heroCollidable);
	heroEnt->AddComponent(heroQuestInfo);
	heroEnt->AddComponent(heroDrawable);

	heroInventory->UpdateInfoSprite();

	entities.push_back(heroEnt);
}

void World::CreateMonster(const Position &pos)
{
	std::shared_ptr<TransformComponent> monsterTransform = std::make_shared<TransformComponent>();
	monsterTransform->position = pos;
	monsterTransform->prevPosition = pos;

	std::shared_ptr<StatComponent> monsterStat = std::make_shared<StatComponent>();
	monsterStat->InitFromFile(ResolveFileName(FILE_MONSTER_DEF, DIR_ENTITIES));

	std::shared_ptr<CollidableComponent> monsterCollidable = std::make_shared<CollidableComponent>();
	monsterCollidable->collisionInfo[0] = 'M';
	monsterCollidable->collisionInfo[1] = 'M';
	monsterCollidable->onCollision = Floyd::ScriptMonster_OnCollision;

	std::shared_ptr<AIComponent> monsterAI = std::make_shared<AIComponent>();
	monsterAI->onUpdateAI = Floyd::ScriptMonster_OnUpdateAI;

	std::shared_ptr<DrawableComponent> monsterDrawable = std::make_shared<DrawableComponent>();
	monsterDrawable->sprite = Sprite(1, 1);
	monsterDrawable->sprite.LoadTextureFromRawData("M\n");

	std::shared_ptr<ParticleEmitterComponent> monsterEmitter = std::make_shared<ParticleEmitterComponent>();
	monsterEmitter->particleEmitInterval_s = 3;
	monsterEmitter->particlesPerEmission = 1;

	std::shared_ptr<Entity> monsterEnt = std::make_shared<Entity>();
	monsterEnt->AddComponent(monsterTransform);
	monsterEnt->AddComponent(monsterStat);
	monsterEnt->AddComponent(monsterCollidable);
	monsterEnt->AddComponent(monsterAI);
	monsterEnt->AddComponent(monsterDrawable);
	monsterEnt->AddComponent(monsterEmitter);

	entities.push_back(monsterEnt);
}

void World::CreateBoss(const Position &pos)
{
	std::shared_ptr<TransformComponent> bossTransform = std::make_shared<TransformComponent>();
	bossTransform->position = pos;
	bossTransform->prevPosition = pos;

	std::shared_ptr<StatComponent> bossStat = std::make_shared<StatComponent>();
	bossStat->InitFromFile(ResolveFileName(FILE_BOSS_DEF, DIR_ENTITIES));

	std::shared_ptr<CollidableComponent> bossCollidable = std::make_shared<CollidableComponent>();
	bossCollidable->collisionInfo[0] = 'B';
	bossCollidable->collisionInfo[1] = 'B';
	bossCollidable->onCollision = Floyd::ScriptBoss_OnCollision;

	std::shared_ptr<AIComponent> bossAI = std::make_shared<AIComponent>();
	bossAI->onUpdateAI = Floyd::ScriptBoss_OnUpdateAI;

	std::shared_ptr<DrawableComponent> bossDrawable = std::make_shared<DrawableComponent>();
	bossDrawable->sprite = Sprite(1, 1);
	bossDrawable->sprite.LoadTextureFromRawData("M\n"); // TODO: replace with BOSS_TILE

	std::shared_ptr<ParticleEmitterComponent> bossEmitter = std::make_shared<ParticleEmitterComponent>();
	bossEmitter->particleEmitInterval_s = 3;
	bossEmitter->particlesPerEmission = 4;

	std::shared_ptr<AnimatedComponent> bossAnimated = std::make_shared<AnimatedComponent>();
	bossAnimated->AddAnimPoint(Position(pos.x + 1, pos.y - 1));
	bossAnimated->AddAnimPoint(Position(pos.x + 2, pos.y - 1));
	bossAnimated->AddAnimPoint(Position(pos.x + 3, pos.y));
	bossAnimated->AddAnimPoint(Position(pos.x + 2, pos.y + 1));
	bossAnimated->AddAnimPoint(Position(pos.x + 1, pos.y + 1));
	bossAnimated->AddAnimPoint(pos);

	std::shared_ptr<Entity> bossEnt = std::make_shared<Entity>();
	bossEnt->AddComponent(bossTransform);
	bossEnt->AddComponent(bossStat);
	bossEnt->AddComponent(bossCollidable);
	bossEnt->AddComponent(bossAI);
	bossEnt->AddComponent(bossDrawable);
	bossEnt->AddComponent(bossEmitter);
	bossEnt->AddComponent(bossAnimated);

	entities.push_back(bossEnt);
}

void World::CreateParticle(const Position &pos, const Position &dir, int damage,
						   bool isEmittedFromHero)
{
	std::shared_ptr<TransformComponent> particleTransform = std::make_shared<TransformComponent>(pos, pos, dir);

	std::shared_ptr<StatComponent> particleStat = std::make_shared<StatComponent>(1, 0, damage, 0);

	std::shared_ptr<CollidableComponent> particleCollidable = std::make_shared<CollidableComponent>();
	particleCollidable->collisionInfo[0] = '.';
	particleCollidable->collisionInfo[1] = '.';
	particleCollidable->onCollision = Floyd::ScriptParticle_OnCollision;

	std::shared_ptr<AIComponent> particleAI = std::make_shared<AIComponent>();
	particleAI->onUpdateAI = Floyd::ScriptParticle_OnUpdateAI;

	std::shared_ptr<DrawableComponent> particleDrawable = std::make_shared<DrawableComponent>();
	particleDrawable->sprite = Sprite(1, 1);
	particleDrawable->sprite.LoadTextureFromRawData(".\n");

	std::shared_ptr<ParticleComponent> particleParticle = std::make_shared<ParticleComponent>(isEmittedFromHero);

	std::shared_ptr<Entity> particleEnt = std::make_shared<Entity>();
	particleEnt->AddComponent(particleTransform);
	particleEnt->AddComponent(particleCollidable);
	particleEnt->AddComponent(particleAI);
	particleEnt->AddComponent(particleDrawable);
	particleEnt->AddComponent(particleParticle);
	particleEnt->AddComponent(particleStat);

	entities.push_back(particleEnt);
}

void World::RemoveAIEntities()
{
	entities.erase(std::remove_if(entities.begin(), entities.end(),
								  [](std::shared_ptr<Entity> entity)
								  {
									  return entity->GetComponent(CTYPE_AI) != nullptr;
								  }),
				   entities.end());
}

void World::RemoveDeadEntities()
{
	entities.erase(std::remove_if(entities.begin(), entities.end(),
								  [](std::shared_ptr<Entity> entity)
								  {
									  StatComponent *stat = entity->GetComponentDirectly<StatComponent>(CTYPE_STAT);
									  if (stat)
									  {
										  return stat->health <= 0;
									  }
									  else return false;
								  }),
				   entities.end());
}

void World::TeleportHeroToPosition(const Position &newPosition)
{
	TransformComponent *heroTransform = GetHero()->GetComponentDirectly<TransformComponent>(CTYPE_TRANSFORM);

	heroTransform->position = newPosition;
	heroTransform->prevPosition = newPosition;
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