#include "object_initializer.h"

std::map<Tag, std::string> object_initializer::mappedTags = { {Tag::hero, "hero"}, {Tag::hare, "hare"}, {Tag::owl, "owl"}, {Tag::deer, "deer"}, {Tag::fox, "fox"}, {Tag::bear, "beer"}, {Tag::wolf, "wolf"},
{Tag::monster, "monster"}, {Tag::owlBoss, "owlBoss"}, {Tag::nightmare1, "nightmare1"}, {Tag::nightmare2, "nightmare2"}, {Tag::nightmare3, "nightmare3"},
{Tag::heroBag, "heroBag"}, {Tag::noose, "noose"}, {Tag::totem, "totem"}, {Tag::hareTrap, "hareTrap"}, {Tag::fence, "fence"}, {Tag::inkyBlackPen, "inkyBlackPen"},
	{Tag::unknownWreath, "inknownWreath"}, {Tag::hareWreath, "hareWreath"}, {Tag::owlWreath, "owlWreath"}, {Tag::tree, "tree"}, {Tag::grass, "grass"}, {Tag::spawn, "spawn"},
	{Tag::ground, "ground"}, {Tag::groundConnection, "groundConnection"}, {Tag::brazier, "brazier"}, {Tag::wreathTable, "wreathTable"}, {Tag::rock, "rock"},
	{Tag::stump, "stump"}, {Tag::droppedLoot, "droppedLoot"}, {Tag::mushroom, "mushroom"}, {Tag::log, "log"}, {Tag::bush, "bush"}, {Tag::roof, "roof"}, {Tag::lake, "lake"}, {Tag::root, "root"},
	{Tag::chamomile, "chamomile"}, {Tag::yarrow, "yarrow"}, {Tag::fern, "fern"}, {Tag::mugwort, "mugwort"}, {Tag::poppy, "poppy"}, {Tag::buildObject, "buildObject"}, {Tag::dropPoint, "dropPoint"},
	{Tag::emptyDraft, "emptyDraft"}, {Tag::emptyPage, "emptyPage"}, {Tag::emptyCell, "emptyCell"}, {Tag::selectedCell, "selectedCell"}, {Tag::clapWhirl, "clapWhirl"}, {Tag::emptyObject, "emptyObject"} };

std::map<std::string, Tag> object_initializer::mappedStrings = { {"hero", Tag::hero}, {"hare", Tag::hare}, {"owl", Tag::owl}, {"deer", Tag::deer}, {"fox", Tag::fox}, {"bear", Tag::bear}, {"wolf", Tag::wolf},
{"monster", Tag::monster}, {"owlBoss", Tag::owlBoss}, {"nightmare1", Tag::nightmare1}, {"nightmare2", Tag::nightmare2}, {"nightmare3", Tag::nightmare3},
{"heroBag", Tag::heroBag}, {"noose", Tag::noose}, {"totem", Tag::totem}, {"hareTrap", Tag::hareTrap}, {"fence", Tag::fence}, {"inkyBlackPen", Tag::inkyBlackPen},
	{"unknownWreath", Tag::unknownWreath}, {"hareWreath", Tag::hareWreath}, {"owlWreath", Tag::owlWreath}, {"tree", Tag::tree}, {"grass", Tag::grass}, {"spawn", Tag::spawn},
	{"ground", Tag::ground}, {"groundConnection", Tag::groundConnection}, {"brazier", Tag::brazier}, {"wreathTable", Tag::wreathTable}, {"rock", Tag::rock},
	{"stump", Tag::stump}, {"droppedLoot", Tag::droppedLoot}, {"mushroom", Tag::mushroom}, {"log", Tag::log}, {"bush", Tag::bush}, {"roof", Tag::roof}, {"lake", Tag::lake}, {"root", Tag::root},
	{"chamomile", Tag::chamomile}, {"yarrow", Tag::yarrow}, {"fern", Tag::fern}, {"mugwort", Tag::mugwort}, {"poppy", Tag::poppy}, {"buildObject", Tag::buildObject}, {"dropPoint", Tag::dropPoint},
	{"emptyDraft", Tag::emptyDraft}, {"emptyPage", Tag::emptyPage}, {"emptyCell", Tag::emptyCell}, {"selectedCell", Tag::selectedCell}, {"clapWhirl", Tag::clapWhirl}, {"emptyObject", Tag::emptyObject} };

object_initializer::object_initializer()
= default;

object_initializer::~object_initializer()
= default;

int object_initializer::newNameId = 0;

static_object* object_initializer::initializeStaticItem(
	Tag itemClass,
	Vector2f itemPosition,
	int itemType,
	const std::string& itemName,
	int count,
	Biomes biome,	
	std::map<PackTag, sprite_pack>* packsMap,
	bool mirrored,
	const std::vector<std::pair<Tag, int>>& inventory)
{
	static_object* item = nullptr;

	switch (itemClass)
	{
		case Tag::tree:
		{
			item = new forest_tree("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::grass:
		{
			item = new grass("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::spawn:
		{
			item = new spawn("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::ground:
		{
			item = new ground("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::groundConnection:
		{
			item = new ground_connection("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::chamomile:
		{
			item = new chamomile("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::brazier:
		{
			item = new brazier("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::wreathTable:
		{
			item = new wreath_table("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::yarrow:
		{
			item = new yarrow("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fern:
		{
			item = new fern("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::hareTrap:
		{
			item = new hare_trap("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::rock:
		{
			item = new rock("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fence:
		{
			item = new fence("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::stump:
		{
			item = new stump("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::droppedLoot:
		{
			item = new dropped_loot("item", Vector2f(0, 0), -1, count);
			break;
		}
		case Tag::mugwort:
		{
			item = new mugwort("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::totem:
		{
			item = new totem("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::mushroom:
		{
			item = new mushroom("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::log:
		{
			item = new mr::log("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::bush:
		{
			item = new bush("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::lake:
		{
			item = new lake("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::root:
		{
			item = new root("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::roof:
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
	
	if (packsMap->count(sprites[0]->packTag) <= 0 ||
		packsMap->at(sprites[0]->packTag).getOriginalInfo(sprites[0]->packPart, sprites[0]->direction, sprites[0]->number).source_size == sprite_pack_structure::size(0, 0))
	{
		delete item;
		return nullptr;
	}
	const auto info = packsMap->at(sprites[0]->packTag).getOriginalInfo(sprites[0]->packPart, sprites[0]->direction, sprites[0]->number);
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
	Tag itemClass,
	Vector2f itemPosition,
	const std::string& itemName,
	std::map<PackTag, sprite_pack>* packsMap,
	world_object* owner)
{
	dynamic_object* item;
	std::string nameOfImage;

	switch (itemClass)
	{
	case Tag::hero:
	{
		item = new deerchant("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/hero/stand/down/1";		
		break;
	}
	case Tag::wolf:
	{
		item = new wolf("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/wolf/stand/down/1";
		break;
	}
	case Tag::hare:
	{
		item = new hare("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/hare/stand/down/1";
		break;
	}
	case Tag::deer:
	{
		item = new deer("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case Tag::bear:
	{
		item = new bear("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case Tag::owl:
	{
		item = new owl("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case Tag::noose:
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
	case Tag::owlBoss:
	{
		item = new owl_boss("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/owlBoss/stand/down/1";
		break;
	}
	case Tag::nightmare1:
	{
		item = new nightmare_first("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare1/stand/down/1";
		break;
	}
	case Tag::nightmare2:
	{
		item = new nightmare_second("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	case Tag::nightmare3:
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
	if (packsMap->count(sprites[0]->packTag) <= 0 || 
		packsMap->at(sprites[0]->packTag).getOriginalInfo(sprites[0]->packPart, sprites[0]->direction, sprites[0]->number).source_size == sprite_pack_structure::size(0, 0))
	{
		delete item;
		return nullptr;
	}
	const auto info = packsMap->at(sprites[0]->packTag).getOriginalInfo(sprites[0]->packPart, sprites[0]->direction, sprites[0]->number);
	const auto textureSize = Vector2f(float(info.source_size.w), float(info.source_size.h));
	item->setTextureSize(textureSize);
	return item;
}

int object_initializer::getRandomTypeByBiome(world_object* object, const Biomes biome)
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
			if (biome == SwampyTrees)
				return rand() % 3 + 11;
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

