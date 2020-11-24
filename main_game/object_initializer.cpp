#include "object_initializer.h"

std::map<entity_tag, std::string> object_initializer::mapped_tags = { {entity_tag::hero, "hero"}, {entity_tag::hare, "hare"}, {entity_tag::owl, "owl"}, {entity_tag::deer, "deer"}, {entity_tag::fox, "fox"}, {entity_tag::bear, "beer"}, {entity_tag::wolf, "wolf"},
{entity_tag::monster, "monster"}, {entity_tag::owlBoss, "owlBoss"}, {entity_tag::nightmare1, "nightmare1"}, {entity_tag::nightmare2, "nightmare2"}, {entity_tag::nightmare3, "nightmare3"},
{entity_tag::heroBag, "heroBag"}, {entity_tag::noose, "noose"}, {entity_tag::totem, "totem"}, {entity_tag::hareTrap, "hareTrap"}, {entity_tag::fence, "fence"}, {entity_tag::inkyBlackPen, "inkyBlackPen"},
	{entity_tag::unknownWreath, "inknownWreath"}, {entity_tag::hareWreath, "hareWreath"}, {entity_tag::owlWreath, "owlWreath"}, {entity_tag::tree, "tree"}, {entity_tag::grass, "grass"}, {entity_tag::spawn, "spawn"},
	{entity_tag::ground, "ground"}, {entity_tag::groundConnection, "groundConnection"}, {entity_tag::brazier, "brazier"}, {entity_tag::wreathTable, "wreathTable"}, {entity_tag::rock, "rock"},
	{entity_tag::stump, "stump"}, {entity_tag::droppedLoot, "droppedLoot"}, {entity_tag::mushroom, "mushroom"}, {entity_tag::log, "log"}, {entity_tag::bush, "bush"}, {entity_tag::roof, "roof"}, {entity_tag::lake, "lake"}, {entity_tag::root, "root"},
	{entity_tag::chamomile, "chamomile"}, {entity_tag::yarrow, "yarrow"}, {entity_tag::fern, "fern"}, {entity_tag::mugwort, "mugwort"}, {entity_tag::poppy, "poppy"}, {entity_tag::buildObject, "buildObject"}, {entity_tag::dropPoint, "dropPoint"},
	{entity_tag::emptyDraft, "emptyDraft"}, {entity_tag::emptyPage, "emptyPage"}, {entity_tag::emptyCell, "emptyCell"}, {entity_tag::selectedCell, "selectedCell"}, {entity_tag::clapWhirl, "clapWhirl"}, {entity_tag::emptyObject, "emptyObject"} };

std::map<std::string, entity_tag> object_initializer::mapped_strings = { {"hero", entity_tag::hero}, {"hare", entity_tag::hare}, {"owl", entity_tag::owl}, {"deer", entity_tag::deer}, {"fox", entity_tag::fox}, {"bear", entity_tag::bear}, {"wolf", entity_tag::wolf},
{"monster", entity_tag::monster}, {"owlBoss", entity_tag::owlBoss}, {"nightmare1", entity_tag::nightmare1}, {"nightmare2", entity_tag::nightmare2}, {"nightmare3", entity_tag::nightmare3},
{"heroBag", entity_tag::heroBag}, {"noose", entity_tag::noose}, {"totem", entity_tag::totem}, {"hareTrap", entity_tag::hareTrap}, {"fence", entity_tag::fence}, {"inkyBlackPen", entity_tag::inkyBlackPen},
	{"unknownWreath", entity_tag::unknownWreath}, {"hareWreath", entity_tag::hareWreath}, {"owlWreath", entity_tag::owlWreath}, {"tree", entity_tag::tree}, {"grass", entity_tag::grass}, {"spawn", entity_tag::spawn},
	{"ground", entity_tag::ground}, {"groundConnection", entity_tag::groundConnection}, {"brazier", entity_tag::brazier}, {"wreathTable", entity_tag::wreathTable}, {"rock", entity_tag::rock},
	{"stump", entity_tag::stump}, {"droppedLoot", entity_tag::droppedLoot}, {"mushroom", entity_tag::mushroom}, {"log", entity_tag::log}, {"bush", entity_tag::bush}, {"roof", entity_tag::roof}, {"lake", entity_tag::lake}, {"root", entity_tag::root},
	{"chamomile", entity_tag::chamomile}, {"yarrow", entity_tag::yarrow}, {"fern", entity_tag::fern}, {"mugwort", entity_tag::mugwort}, {"poppy", entity_tag::poppy}, {"buildObject", entity_tag::buildObject}, {"dropPoint", entity_tag::dropPoint},
	{"emptyDraft", entity_tag::emptyDraft}, {"emptyPage", entity_tag::emptyPage}, {"emptyCell", entity_tag::emptyCell}, {"selectedCell", entity_tag::selectedCell}, {"clapWhirl", entity_tag::clapWhirl}, {"emptyObject", entity_tag::emptyObject} };

object_initializer::object_initializer()
= default;


int object_initializer::new_name_id = 0;

shared_ptr<static_object> object_initializer::initialize_static_item(
	const entity_tag item_class,
	const Vector2f item_position,
	const int item_type,
	const std::string& item_name,
	const int count,
	const biomes biome,
	const shared_ptr<std::map<pack_tag, sprite_pack>>& packs_map,
	const bool mirrored,
	const std::vector<std::pair<entity_tag, int>>& inventory)
{
	shared_ptr<static_object> item;

	switch (item_class)
	{
		case entity_tag::brazier:
		{
			item = make_shared<brazier>("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::tree:
		{
			item = make_shared<forest_tree>("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::ground:
		{
			item = make_shared<ground>("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::groundConnection:
		{
			item = make_shared<ground_connection>("item", Vector2f(0, 0), -1);
			break;
		}
		/*case entity_tag::grass:
		{
			item = new grass("item", Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::chamomile:
		{
			item = new chamomile("item", Vector2f(0, 0), -1);
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
		}*/
		default:
			return nullptr;
	}

	const auto current_type = item_type == -1
		 ? get_random_type_by_biome(*item, biome)
		 : item_type;

	new_name_id++;

	item->set_position(Vector2f(item_position));
	item->setType(current_type);
	auto sprites = item->prepare_sprites(0);
	
	if (packs_map->count(sprites[0]->pack_tag) <= 0 ||
		packs_map->at(sprites[0]->pack_tag).get_original_info(sprites[0]->pack_part, sprites[0]->direction, sprites[0]->number).source_size == sprite_pack_structure::size(0, 0))
		return nullptr;

	const auto info = packs_map->at(sprites[0]->pack_tag).get_original_info(sprites[0]->pack_part, sprites[0]->direction, sprites[0]->number);
	const auto texture_size = Vector2f(float(info.source_size.w), float(info.source_size.h));
	item->set_texture_size(texture_size);
	const auto name = item_name.empty()
		                  ? std::to_string(new_name_id)
		                  : item_name;
	item->set_name(name);
	if (!inventory.empty())
		item->inventory = inventory;

	if (!mirrored || item->is_multi_ellipse)
		item->cancel_mirroring();

	return item;
}

shared_ptr<dynamic_object> object_initializer::initialize_dynamic_item(
	entity_tag item_class,
	Vector2f item_position,
	const std::string& item_name,
	const shared_ptr<std::map<pack_tag, sprite_pack>>& packs_map,
	const shared_ptr<world_object>& owner)
{
	shared_ptr<dynamic_object> item;
	std::string name_of_image;

	switch (item_class)
	{
	case entity_tag::hero:
	{
		item = make_shared<deerchant>("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/hero/stand/down/1";
		break;
	}
	/*case entity_tag::wolf:
	{
		item = new wolf("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/wolf/stand/down/1";
		break;
	}
	case entity_tag::hare:
	{
		item = new hare("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/hare/stand/down/1";
		break;
	}
	case entity_tag::deer:
	{
		item = new deer("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case entity_tag::bear:
	{
		item = new bear("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case entity_tag::owl:
	{
		item = new owl("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case entity_tag::noose:
	{
		item = new noose("item", Vector2f(0, 0), owner);
		name_of_image = "Game/worldSprites/noose/nooseLoop/1";
		break;
	}
	/*case Tag::clapWhirl:
	{
		item = new ClapWhirl("item", Vector2f(0, 0), owner);
		nameOfImage = "Game/worldSprites/nightmare3/clap/whirl";
		break;
	}
	case entity_tag::owlBoss:
	{
		item = new owl_boss("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/owlBoss/stand/down/1";
		break;
	}
	case entity_tag::nightmare1:
	{
		item = new nightmare_first("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/nightmare1/stand/down/1";
		break;
	}
	case entity_tag::nightmare2:
	{
		item = new nightmare_second("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	case entity_tag::nightmare3:
	{
		item = new nightmare_third("item", Vector2f(0, 0));
		name_of_image = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}*/
	default:
		return nullptr;
	}

	new_name_id++;
	name_of_image += ".png";

	const auto name = item_name.empty()
		? std::string(item->get_to_save_name()) + "_" + std::to_string(new_name_id)
		: item_name;

	item->set_name(name);
	item->set_position(Vector2f(item_position));
    auto sprites = item->prepare_sprites(0);
	if (packs_map->count(sprites[0]->pack_tag) <= 0 || 
		packs_map->at(sprites[0]->pack_tag).get_original_info(sprites[0]->pack_part, sprites[0]->direction, sprites[0]->number).source_size == sprite_pack_structure::size(0, 0))
		return nullptr;

	const auto info = packs_map->at(sprites[0]->pack_tag).get_original_info(sprites[0]->pack_part, sprites[0]->direction, sprites[0]->number);
	const auto texture_size = Vector2f(float(info.source_size.w), float(info.source_size.h));
	item->set_texture_size(texture_size);
	return item;
}

int object_initializer::get_random_type_by_biome(const world_object& object, const biomes biome)
{
	switch (object.tag)
	{
		case entity_tag::tree:
		{
			if (biome == birch_grove)
				return rand() % 7 + 1;
			if (biome == dark_woods)
				return rand() % 6 + 8;
			if (biome == swampy_trees)
				return rand() % 5 + 14;
			break;
		}
		case entity_tag::grass:
		{
			if (biome == birch_grove)
				return rand() % 8 + 1;
			if (biome == dark_woods)
				return rand() % 13 + 9;
			if (biome == swampy_trees)
				return rand() % 9 + 22;
			break;
		}
		case entity_tag::rock:
		{
			if (biome == birch_grove)
				return rand() % 8 + 1;
			if (biome == dark_woods)
				return rand() % 5 + 9;
			if (biome == swampy_trees)
				return  rand() % 5 + 14;
			break;
		}
		case entity_tag::stump:
		{
			if (biome == birch_grove)
				return rand() % 4 + 1;
			if (biome == dark_woods)
				return rand() % 6 + 5;
			if (biome == swampy_trees)
				return rand() % 3 + 11;
			break;
		}
		case entity_tag::bush:
		{
			if (biome == birch_grove)
				return rand() % 8 + 1;
			if (biome == swampy_trees)
				return rand() % 7 + 9;
			break;
		}
		case entity_tag::mushroom:
		{
			if (biome == birch_grove)
				return rand() % 3 + 1;
			if (biome == dark_woods)
				return rand() % 9 + 4;
			if (biome == swampy_trees)
				return rand() % 4 + 13;
			break;
		}
		default: return rand() % object.get_variety_of_types() + 1;
	}
	return 0;
}

std::vector<shared_ptr<static_object>> object_initializer::vector_cast_to_static(const std::vector<shared_ptr<world_object>>& items)
{
	std::vector< shared_ptr<static_object>> static_items{};
	static_items.reserve(items.size());
	for (const auto item : items)
		static_items.push_back(dynamic_pointer_cast<static_object>(item));
	return static_items;
}

std::vector< shared_ptr<dynamic_object>> object_initializer::vector_cast_to_dynamic(const std::vector<shared_ptr<world_object>>& items)
{
	std::vector<shared_ptr<dynamic_object>> dynamic_items{};
	dynamic_items.reserve(items.size());
	for (auto& item : items)
		dynamic_items.push_back(dynamic_pointer_cast<dynamic_object>(item));
	return dynamic_items;
}

