#include "object_initializer.h"

std::map<entity_tag, std::string> object_initializer::mappedTags = { {entity_tag::hero, "hero"}, {entity_tag::hare, "hare"}, {entity_tag::owl, "owl"}, {entity_tag::deer, "deer"}, {entity_tag::fox, "fox"}, {entity_tag::bear, "beer"}, {entity_tag::wolf, "wolf"},
{entity_tag::monster, "monster"}, {entity_tag::owlBoss, "owlBoss"}, {entity_tag::nightmare1, "nightmare1"}, {entity_tag::nightmare2, "nightmare2"}, {entity_tag::nightmare3, "nightmare3"},
{entity_tag::heroBag, "heroBag"}, {entity_tag::noose, "noose"}, {entity_tag::totem, "totem"}, {entity_tag::hareTrap, "hareTrap"}, {entity_tag::fence, "fence"}, {entity_tag::inkyBlackPen, "inkyBlackPen"},
	{entity_tag::unknownWreath, "inknownWreath"}, {entity_tag::hareWreath, "hareWreath"}, {entity_tag::owlWreath, "owlWreath"}, {entity_tag::tree, "tree"}, {entity_tag::grass, "grass"}, {entity_tag::spawn, "spawn"},
	{entity_tag::ground, "ground"}, {entity_tag::groundConnection, "groundConnection"}, {entity_tag::brazier, "brazier"}, {entity_tag::wreathTable, "wreathTable"}, {entity_tag::rock, "rock"},
	{entity_tag::stump, "stump"}, {entity_tag::droppedLoot, "droppedLoot"}, {entity_tag::mushroom, "mushroom"}, {entity_tag::log, "log"}, {entity_tag::bush, "bush"}, {entity_tag::roof, "roof"}, {entity_tag::lake, "lake"}, {entity_tag::root, "root"},
	{entity_tag::chamomile, "chamomile"}, {entity_tag::yarrow, "yarrow"}, {entity_tag::fern, "fern"}, {entity_tag::mugwort, "mugwort"}, {entity_tag::poppy, "poppy"}, {entity_tag::buildObject, "buildObject"}, {entity_tag::dropPoint, "dropPoint"},
	{entity_tag::emptyDraft, "emptyDraft"}, {entity_tag::emptyPage, "emptyPage"}, {entity_tag::emptyCell, "emptyCell"}, {entity_tag::selectedCell, "selectedCell"}, {entity_tag::clapWhirl, "clapWhirl"}, {entity_tag::emptyObject, "emptyObject"} };

std::map<std::string, entity_tag> object_initializer::mappedStrings = { {"hero", entity_tag::hero}, {"hare", entity_tag::hare}, {"owl", entity_tag::owl}, {"deer", entity_tag::deer}, {"fox", entity_tag::fox}, {"bear", entity_tag::bear}, {"wolf", entity_tag::wolf},
{"monster", entity_tag::monster}, {"owlBoss", entity_tag::owlBoss}, {"nightmare1", entity_tag::nightmare1}, {"nightmare2", entity_tag::nightmare2}, {"nightmare3", entity_tag::nightmare3},
{"heroBag", entity_tag::heroBag}, {"noose", entity_tag::noose}, {"totem", entity_tag::totem}, {"hareTrap", entity_tag::hareTrap}, {"fence", entity_tag::fence}, {"inkyBlackPen", entity_tag::inkyBlackPen},
	{"unknownWreath", entity_tag::unknownWreath}, {"hareWreath", entity_tag::hareWreath}, {"owlWreath", entity_tag::owlWreath}, {"tree", entity_tag::tree}, {"grass", entity_tag::grass}, {"spawn", entity_tag::spawn},
	{"ground", entity_tag::ground}, {"groundConnection", entity_tag::groundConnection}, {"brazier", entity_tag::brazier}, {"wreathTable", entity_tag::wreathTable}, {"rock", entity_tag::rock},
	{"stump", entity_tag::stump}, {"droppedLoot", entity_tag::droppedLoot}, {"mushroom", entity_tag::mushroom}, {"log", entity_tag::log}, {"bush", entity_tag::bush}, {"roof", entity_tag::roof}, {"lake", entity_tag::lake}, {"root", entity_tag::root},
	{"chamomile", entity_tag::chamomile}, {"yarrow", entity_tag::yarrow}, {"fern", entity_tag::fern}, {"mugwort", entity_tag::mugwort}, {"poppy", entity_tag::poppy}, {"buildObject", entity_tag::buildObject}, {"dropPoint", entity_tag::dropPoint},
	{"emptyDraft", entity_tag::emptyDraft}, {"emptyPage", entity_tag::emptyPage}, {"emptyCell", entity_tag::emptyCell}, {"selectedCell", entity_tag::selectedCell}, {"clapWhirl", entity_tag::clapWhirl}, {"emptyObject", entity_tag::emptyObject} };

object_initializer::object_initializer()
= default;

object_initializer::~object_initializer()
= default;

int object_initializer::newNameId = 0;

static_object* object_initializer::initializeStaticItem(
	entity_tag itemClass,
	Vector2f itemPosition,
	int itemType,
	const std::string& itemName,
	int count,
	biomes biome,	
	std::map<pack_tag, sprite_pack>* packsMap,
	bool mirrored,
	const std::vector<std::pair<entity_tag, int>>& inventory)
{
	static_object* item = nullptr;

	switch (itemClass)
	{
		case entity_tag::tree:
		{
			item = new forest_tree("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::grass:
		{
			item = new grass("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::spawn:
		{
			item = new spawn("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::ground:
		{
			item = new ground("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::groundConnection:
		{
			item = new ground_connection("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::chamomile:
		{
			item = new chamomile("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::brazier:
		{
			item = new brazier("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::wreathTable:
		{
			item = new wreath_table("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::yarrow:
		{
			item = new yarrow("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::fern:
		{
			item = new fern("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::hareTrap:
		{
			item = new hare_trap("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::rock:
		{
			item = new rock("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::fence:
		{
			item = new fence("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::stump:
		{
			item = new stump("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::droppedLoot:
		{
			item = new dropped_loot("item", Vector2f(0, 0), -1, count);
			break;
		}
		case entity_tag::mugwort:
		{
			item = new mugwort("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::totem:
		{
			item = new totem("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::mushroom:
		{
			item = new mushroom("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::log:
		{
			item = new mr::log("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::bush:
		{
			item = new bush("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::lake:
		{
			item = new lake("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::root:
		{
			item = new root("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::roof:
		{
			item = new roof("item", Vector2f(0, 0), -1);
			break;
		}
		default:
		{
			item = new spawn("item", Vector2f(0, 0), -1);
			break;
		}
	}

	const auto currentType = itemType == -1
		 ? getRandomTypeByBiome(item, biome)
		 : itemType;

	newNameId++;

	item->setPosition(Vector2f(itemPosition));
	item->setType(currentType);
	auto sprites = item->prepareSprites(0);
	
	if (packsMap->count(sprites[0]->pack_tag) <= 0 ||
		packsMap->at(sprites[0]->pack_tag).getOriginalInfo(sprites[0]->packPart, sprites[0]->direction, sprites[0]->number).source_size == sprite_pack_structure::size(0, 0))
	{
		delete item;
		return nullptr;
	}
	const auto info = packsMap->at(sprites[0]->pack_tag).getOriginalInfo(sprites[0]->packPart, sprites[0]->direction, sprites[0]->number);
	const auto textureSize = Vector2f(float(info.source_size.w), float(info.source_size.h));
	item->setTextureSize(textureSize);
	const auto name = itemName.empty()
		                  ? std::to_string(newNameId)
		                  : itemName;
	item->setName(name);
	if (!inventory.empty())
		item->inventory = inventory;

	if (!mirrored || item->isMultiEllipse)
		item->cancelMirroring();

	return item;
}

dynamic_object* object_initializer::initializeDynamicItem(
	entity_tag itemClass,
	Vector2f itemPosition,
	const std::string& itemName,
	std::map<pack_tag, sprite_pack>* packsMap,
	world_object* owner)
{
	dynamic_object* item;
	std::string nameOfImage;

	switch (itemClass)
	{
	case entity_tag::hero:
	{
		item = new deerchant("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/hero/stand/down/1";		
		break;
	}
	case entity_tag::wolf:
	{
		item = new wolf("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/wolf/stand/down/1";
		break;
	}
	case entity_tag::hare:
	{
		item = new hare("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/hare/stand/down/1";
		break;
	}
	case entity_tag::deer:
	{
		item = new deer("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case entity_tag::bear:
	{
		item = new bear("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case entity_tag::owl:
	{
		item = new owl("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case entity_tag::noose:
	{
		item = new noose("item", Vector2f(0, 0), owner);
		nameOfImage = "Game/worldSprites/noose/nooseLoop/1";
		break;
	}
	/*case Tag::clapWhirl:
	{
		item = new ClapWhirl("item", Vector2f(0, 0), owner);
		nameOfImage = "Game/worldSprites/nightmare3/clap/whirl";
		break;
	}*/
	case entity_tag::owlBoss:
	{
		item = new owl_boss("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/owlBoss/stand/down/1";
		break;
	}
	case entity_tag::nightmare1:
	{
		item = new nightmare_first("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare1/stand/down/1";
		break;
	}
	case entity_tag::nightmare2:
	{
		item = new nightmare_second("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	case entity_tag::nightmare3:
	{
		item = new nightmare_third("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	default:
	{
		item = new nightmare_second("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	}

	newNameId++;
	nameOfImage += ".png";

	const auto name = itemName.empty()
		                  ? std::string(item->getToSaveName()) + "_" + std::to_string(newNameId)
		                  : itemName;

	item->setName(name);
	item->setPosition(Vector2f(itemPosition));
    auto sprites = item->prepareSprites(0);
	if (packsMap->count(sprites[0]->pack_tag) <= 0 || 
		packsMap->at(sprites[0]->pack_tag).getOriginalInfo(sprites[0]->packPart, sprites[0]->direction, sprites[0]->number).source_size == sprite_pack_structure::size(0, 0))
	{
		delete item;
		return nullptr;
	}
	const auto info = packsMap->at(sprites[0]->pack_tag).getOriginalInfo(sprites[0]->packPart, sprites[0]->direction, sprites[0]->number);
	const auto textureSize = Vector2f(float(info.source_size.w), float(info.source_size.h));
	item->setTextureSize(textureSize);
	return item;
}

int object_initializer::getRandomTypeByBiome(world_object* object, const biomes biome)
{
	switch (object->tag)
	{
		case entity_tag::tree:
		{
			if (biome == BirchGrove)
				return rand() % 7 + 1;
			if (biome == DarkWoods)
				return rand() % 6 + 8;
			if (biome == SwampyTrees)
				return rand() % 5 + 14;
			break;
		}
		case entity_tag::grass:
		{
			if (biome == BirchGrove)
				return rand() % 8 + 1;
			if (biome == DarkWoods)
				return rand() % 13 + 9;
			if (biome == SwampyTrees)
				return rand() % 9 + 22;
			break;
		}
		case entity_tag::rock:
		{
			if (biome == BirchGrove)
				return rand() % 8 + 1;
			if (biome == DarkWoods)
				return rand() % 5 + 9;
			if (biome == SwampyTrees)
				return  rand() % 5 + 14;
			break;
		}
		case entity_tag::stump:
		{
			if (biome == BirchGrove)
				return rand() % 4 + 1;
			if (biome == DarkWoods)
				return rand() % 6 + 5;
			if (biome == SwampyTrees)
				return rand() % 3 + 11;
			break;
		}
		case entity_tag::bush:
		{
			if (biome == BirchGrove)
				return rand() % 8 + 1;
			if (biome == SwampyTrees)
				return rand() % 7 + 9;
			break;
		}
		case entity_tag::mushroom:
		{
			if (biome == BirchGrove)
				return rand() % 3 + 1;
			if (biome == DarkWoods)
				return rand() % 9 + 4;
			if (biome == SwampyTrees)
				return rand() % 4 + 13;
			break;
		}
		default: return rand() % object->getVarietyOfTypes() + 1;
	}
	return 0;
}

std::vector<static_object*> object_initializer::vectorCastToStatic(const std::vector<world_object*>& items)
{
	std::vector<static_object*> staticItems = {};
	for (auto item : items)
		staticItems.push_back(dynamic_cast<static_object*>(item));
	return staticItems;
}

std::vector<dynamic_object*> object_initializer::vectorCastToDynamic(std::vector<world_object*> items)
{
	auto dynamicItems = *(new std::vector<dynamic_object*>());
	for (auto& item : items)
		dynamicItems.push_back(dynamic_cast<dynamic_object*>(item));
	return dynamicItems;
}

