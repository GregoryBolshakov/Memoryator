#include "ObjectInitializer.h"
#include "Helper.h"

std::map<Tag, std::string> ObjectInitializer::mappedTags = { {Tag::hero1, "hero1"}, {Tag::hare, "hare"}, {Tag::owl, "owl"}, {Tag::deer, "deer"}, {Tag::fox, "fox"}, {Tag::bear, "beer"}, {Tag::wolf, "wolf"},
{Tag::monster, "monster"}, {Tag::owlBoss, "owlBoss"}, {Tag::nightmare1, "nightmare1"}, {Tag::nightmare2, "nightmare2"}, {Tag::nightmare3, "nightmare3"},
{Tag::heroBag, "heroBag"}, {Tag::noose, "noose"}, {Tag::totem, "totem"}, {Tag::hareTrap, "hareTrap"}, {Tag::fence, "fence"}, {Tag::inkyBlackPen, "inkyBlackPen"},
	{Tag::unknownWreath, "inknownWreath"}, {Tag::hareWreath, "hareWreath"}, {Tag::owlWreath, "owlWreath"}, {Tag::tree, "tree"}, {Tag::grass, "grass"}, {Tag::spawn, "spawn"},
	{Tag::ground, "ground"}, {Tag::groundConnection, "groundConnection"}, {Tag::brazier, "brazier"}, {Tag::wreathTable, "wreathTable"}, {Tag::rock, "rock"},
	{Tag::stump, "stump"}, {Tag::droppedLoot, "droppedLoot"}, {Tag::mushroom, "mushroom"}, {Tag::log, "log"}, {Tag::bush, "bush"}, {Tag::roof, "roof"}, {Tag::lake, "lake"},
	{Tag::chamomile, "chamomile"}, {Tag::yarrow, "yarrow"}, {Tag::fern, "fern"}, {Tag::mugwort, "mugwort"}, {Tag::poppy, "poppy"}, {Tag::buildObject, "buildObject"}, {Tag::dropPoint, "dropPoint"},
	{Tag::emptyDraft, "emptyDraft"}, {Tag::emptyPage, "emptyPage"}, {Tag::emptyCell, "emptyCell"}, {Tag::selectedCell, "selectedCell"}, {Tag::clapWhirl, "clapWhirl"}, {Tag::emptyObject, "emptyObject"} };

std::map<std::string, Tag> ObjectInitializer::mappedStrings = { {"hero1", Tag::hero1}, {"hare", Tag::hare}, {"owl", Tag::owl}, {"deer", Tag::deer}, {"fox", Tag::fox}, {"bear", Tag::bear}, {"wolf", Tag::wolf},
{"monster", Tag::monster}, {"owlBoss", Tag::owlBoss}, {"nightmare1", Tag::nightmare1}, {"nightmare2", Tag::nightmare2}, {"nightmare3", Tag::nightmare3},
{"heroBag", Tag::heroBag}, {"noose", Tag::noose}, {"totem", Tag::totem}, {"hareTrap", Tag::hareTrap}, {"fence", Tag::fence}, {"inkyBlackPen", Tag::inkyBlackPen},
	{"unknownWreath", Tag::unknownWreath}, {"hareWreath", Tag::hareWreath}, {"owlWreath", Tag::owlWreath}, {"tree", Tag::tree}, {"grass", Tag::grass}, {"spawn", Tag::spawn},
	{"ground", Tag::ground}, {"groundConnection", Tag::groundConnection}, {"brazier", Tag::brazier}, {"wreathTable", Tag::wreathTable}, {"rock", Tag::rock},
	{"stump", Tag::stump}, {"droppedLoot", Tag::droppedLoot}, {"mushroom", Tag::mushroom}, {"log", Tag::log}, {"bush", Tag::bush}, {"roof", Tag::roof}, {"lake", Tag::lake},
	{"chamomile", Tag::chamomile}, {"yarrow", Tag::yarrow}, {"fern", Tag::fern}, {"mugwort", Tag::mugwort}, {"poppy", Tag::poppy}, {"buildObject", Tag::buildObject}, {"dropPoint", Tag::dropPoint},
	{"emptyDraft", Tag::emptyDraft}, {"emptyPage", Tag::emptyPage}, {"emptyCell", Tag::emptyCell}, {"selectedCell", Tag::selectedCell}, {"clapWhirl", Tag::clapWhirl}, {"emptyObject", Tag::emptyObject} };

ObjectInitializer::ObjectInitializer()
{
}

ObjectInitializer::~ObjectInitializer()
{
}

int ObjectInitializer::newNameId = 0;

StaticObject* ObjectInitializer::initializeStaticItem(
	Tag itemClass,
	Vector2f itemPosition,
	int itemType,
	std::string itemName,
	int count,
	Biomes biome,	
	std::unordered_map<std::string, BoardSprite>* spriteMap,
	bool mirrored,
	std::vector<std::pair<Tag, int>> inventory)
{
	StaticObject* item = nullptr;

	switch (itemClass)
	{
		case Tag::tree:
		{
			item = new ForestTree("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::grass:
		{
			item = new Grass("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::spawn:
		{
			item = new Spawn("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::ground:
		{
			item = new Ground("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::groundConnection:
		{
			item = new GroundConnection("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::chamomile:
		{
			item = new Chamomile("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::brazier:
		{
			item = new Brazier("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::wreathTable:
		{
			item = new WreathTable("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::yarrow:
		{
			item = new Yarrow("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fern:
		{
			item = new Fern("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::hareTrap:
		{
			item = new HareTrap("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::rock:
		{
			item = new Rock("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fence:
		{
			item = new Fence("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::stump:
		{
			item = new Stump("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::droppedLoot:
		{
			item = new DroppedLoot("item", Vector2f(0, 0), -1, count);
			break;
		}
		case Tag::mugwort:
		{
			item = new Mugwort("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::totem:
		{
			item = new Totem("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::mushroom:
		{
			item = new Mushroom("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::log:
		{
			item = new Log("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::bush:
		{
			item = new Bush("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::lake:
		{
			item = new Lake("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::roof:
		{
			item = new Roof("item", Vector2f(0, 0), -1);
			break;
		}
		default:
		{
			item = new Spawn("item", Vector2f(0, 0), -1);
			break;
		}
	}

	if (!mirrored)
		item->manuallyDisableMirroring();

	int currentType = itemType == -1
		? currentType = getRandomTypeByBiome(item, biome)
		: currentType = itemType;

	newNameId++;

	item->setPosition(Vector2f(itemPosition));
	item->setType(currentType);
	item->prepareSpriteNames(0);
	FloatRect textureBounds = FloatRect(0, 0, 0, 0);
	if (spriteMap->count(item->additionalSprites[0].path) > 0)
	textureBounds = spriteMap->at(item->additionalSprites[0].path).sprite.getGlobalBounds();
	const auto textureSize = Vector2f(int(textureBounds.width), int(textureBounds.height));
	item->setTextureSize(textureSize);
	const std::string name = itemName.empty()
		? item->additionalSprites[0].path + "_" + std::to_string(newNameId)
		: itemName;
	item->setName(name);
	if (!inventory.empty())
		item->inventory = inventory;	
	return item;
}

DynamicObject* ObjectInitializer::initializeDynamicItem(
	Tag itemClass,
	Vector2f itemPosition,
	std::string itemName,
	std::unordered_map<std::string, BoardSprite>* spriteMap,
	WorldObject* owner)
{
	DynamicObject* item;
	std::string nameOfImage;

	switch (itemClass)
	{
	case Tag::hero1:
	{
		item = new Deerchant("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/hero/stand/down/1";		
		break;
	}
	case Tag::wolf:
	{
		item = new Wolf("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/wolf/stand/down/1";
		break;
	}
	case Tag::hare:
	{
		item = new Hare("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/hare/stand/down/1";
		break;
	}
	case Tag::deer:
	{
		item = new Deer("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case Tag::bear:
	{
		item = new Bear("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case Tag::owl:
	{
		item = new Owl("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case Tag::noose:
	{
		item = new Noose("item", Vector2f(0, 0), owner);
		nameOfImage = "Game/worldSprites/noose/nooseLoop/1";
		break;
	}
	case Tag::clapWhirl:
	{
		item = new ClapWhirl("item", Vector2f(0, 0), owner);
		nameOfImage = "Game/worldSprites/nightmare3/clap/whirl";
		break;
	}
	case Tag::owlBoss:
	{
		item = new OwlBoss("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/owlBoss/stand/down/1";
		break;
	}
	case Tag::nightmare1:
	{
		item = new Nightmare1("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare1/stand/down/1";
		break;
	}
	case Tag::nightmare2:
	{
		item = new Nightmare2("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	case Tag::nightmare3:
	{
		item = new Nightmare3("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	default:
	{
		item = new Nightmare2("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	}

	newNameId++;
	nameOfImage += ".png";

	const std::string name = itemName.empty()
		? std::string(item->getToSaveName()) + "_" + std::to_string(newNameId)
		: itemName;

	item->setName(name);
	item->setPosition(Vector2f(itemPosition));
	const Vector2f textureSize = Vector2f(spriteMap->at(nameOfImage).texture.getSize());
	item->setTextureSize(textureSize);
	return item;
}

int ObjectInitializer::getRandomTypeByBiome(WorldObject* object, Biomes biome)
{
	switch (object->tag)
	{
		case Tag::tree:
		{
			if (biome == BirchGrove)
				return rand() % 7 + 1;
			if (biome == DarkWoods)
				return rand() % 6 + 8;
			if (biome == SwampyTrees)
				return rand() % 5 + 14;
			break;
		}
		case Tag::grass:
		{
			if (biome == BirchGrove)
				return rand() % 8 + 1;
			if (biome == DarkWoods)
				return rand() % 13 + 9;
			if (biome == SwampyTrees)
				return rand() % 9 + 22;
			break;
		}
		case Tag::rock:
		{
			if (biome == BirchGrove)
				return rand() % 8 + 1;
			if (biome == DarkWoods)
				return rand() % 5 + 9;
			if (biome == SwampyTrees)
				return  rand() % 5 + 14;
			break;
		}
		case Tag::stump:
		{
			if (biome == BirchGrove)
				return rand() % 4 + 1;
			if (biome == DarkWoods)
				return rand() % 6 + 5;
			break;
		}
		case Tag::bush:
		{
			if (biome == BirchGrove)
				return rand() % 8 + 1;
			if (biome == SwampyTrees)
				return rand() % 7 + 9;
			break;
		}
		case Tag::mushroom:
		{
			if (biome == BirchGrove)
				return rand() % 3 + 1;
			if (biome == DarkWoods)
				return rand() % 9 + 4;
			break;
		}
		default: return rand() % object->getVarietyOfTypes() + 1;
	}
}

std::vector<StaticObject*> ObjectInitializer::vectorCastToStatic(std::vector<WorldObject*> items)
{
	std::vector<StaticObject*> staticItems = *(new std::vector<StaticObject*>());
	for (auto& item : items)
		staticItems.push_back(dynamic_cast<StaticObject*>(item));
	return staticItems;
}

std::vector<DynamicObject*> ObjectInitializer::vectorCastToDynamic(std::vector<WorldObject*> items)
{
	std::vector<DynamicObject*> dynamicItems = *(new std::vector<DynamicObject*>());
	for (auto& item : items)
		dynamicItems.push_back(dynamic_cast<DynamicObject*>(item));
	return dynamicItems;
}

