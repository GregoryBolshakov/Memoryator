#include "object_initializer.h"
#include "static_object.h"
#include "sprite_pack.h"
#include "rock.h"
#include "ground.h"
#include "ground_connection.h"
#include "dropped_loot.h"
#include "grass.h"
#include "ground.h"
#include "spawn.h"
#include "chamomile.h"
#include "yarrow.h"
#include "mugwort.h"
#include "fern.h"
#include "brazier.h"
#include "totem.h"
#include "hare_trap.h"
#include "fence.h"
#include "stump.h"
#include "forest_tree.h"
#include "wreath_table.h"
#include "mushroom.h"
#include "log.h"
#include "bush.h"
#include "lake.h"
#include "root.h"
#include  "roof.h"

#include "monster.h"
#include "deerchant.h"
#include "wolf.h"
#include "hare.h"
#include "deer.h"
#include "bear.h"
#include "owl.h"
#include "owl_boss.h"
#include "nightmare_first.h"
#include "nightmare_second.h"
#include "nightmare_third.h"
#include "noose.h"

std::map<entity_tag, std::string> object_initializer::mapped_tags = { {entity_tag::hero, "hero"}, {entity_tag::hare, "hare"}, {entity_tag::owl, "owl"}, {entity_tag::deer, "deer"}, {entity_tag::fox, "fox"}, {entity_tag::bear, "beer"}, {entity_tag::wolf, "wolf"},
{entity_tag::monster, "monster"}, {entity_tag::owl_boss, "owlBoss"}, {entity_tag::nightmare1, "nightmare1"}, {entity_tag::nightmare2, "nightmare2"}, {entity_tag::nightmare3, "nightmare3"},
{entity_tag::hero_bag, "heroBag"}, {entity_tag::noose, "noose"}, {entity_tag::totem, "totem"}, {entity_tag::hare_trap, "hareTrap"}, {entity_tag::fence, "fence"}, {entity_tag::inky_black_pen, "inkyBlackPen"},
	{entity_tag::unknown_wreath, "unknownWreath"}, {entity_tag::hare_wreath, "hareWreath"}, {entity_tag::owl_wreath, "owlWreath"}, {entity_tag::tree, "tree"}, {entity_tag::grass, "grass"}, {entity_tag::spawn, "spawn"},
	{entity_tag::ground, "ground"}, {entity_tag::ground_connection, "groundConnection"}, {entity_tag::brazier, "brazier"}, {entity_tag::wreath_table, "wreathTable"}, {entity_tag::rock, "rock"},
	{entity_tag::stump, "stump"}, {entity_tag::dropped_loot, "droppedLoot"}, {entity_tag::mushroom, "mushroom"}, {entity_tag::log, "log"}, {entity_tag::bush, "bush"}, {entity_tag::roof, "roof"}, {entity_tag::lake, "lake"}, {entity_tag::root, "root"},
	{entity_tag::chamomile, "chamomile"}, {entity_tag::yarrow, "yarrow"}, {entity_tag::fern, "fern"}, {entity_tag::mugwort, "mugwort"}, {entity_tag::poppy, "poppy"}, {entity_tag::build_object, "buildObject"}, {entity_tag::drop_point, "dropPoint"},
	{entity_tag::empty_draft, "emptyDraft"}, {entity_tag::empty_page, "emptyPage"}, {entity_tag::empty_cell, "emptyCell"}, {entity_tag::selected_cell, "selectedCell"}, {entity_tag::clap_whirl, "clapWhirl"}, {entity_tag::empty_object, "emptyObject"} };

std::map<std::string, entity_tag> object_initializer::mapped_strings = { {"hero", entity_tag::hero}, {"hare", entity_tag::hare}, {"owl", entity_tag::owl}, {"deer", entity_tag::deer}, {"fox", entity_tag::fox}, {"bear", entity_tag::bear}, {"wolf", entity_tag::wolf},
{"monster", entity_tag::monster}, {"owlBoss", entity_tag::owl_boss}, {"nightmare1", entity_tag::nightmare1}, {"nightmare2", entity_tag::nightmare2}, {"nightmare3", entity_tag::nightmare3},
{"heroBag", entity_tag::hero_bag}, {"noose", entity_tag::noose}, {"totem", entity_tag::totem}, {"hareTrap", entity_tag::hare_trap}, {"fence", entity_tag::fence}, {"inkyBlackPen", entity_tag::inky_black_pen},
	{"unknownWreath", entity_tag::unknown_wreath}, {"hareWreath", entity_tag::hare_wreath}, {"owlWreath", entity_tag::owl_wreath}, {"tree", entity_tag::tree}, {"grass", entity_tag::grass}, {"spawn", entity_tag::spawn},
	{"ground", entity_tag::ground}, {"groundConnection", entity_tag::ground_connection}, {"brazier", entity_tag::brazier}, {"wreathTable", entity_tag::wreath_table}, {"rock", entity_tag::rock},
	{"stump", entity_tag::stump}, {"droppedLoot", entity_tag::dropped_loot}, {"mushroom", entity_tag::mushroom}, {"log", entity_tag::log}, {"bush", entity_tag::bush}, {"roof", entity_tag::roof}, {"lake", entity_tag::lake}, {"root", entity_tag::root},
	{"chamomile", entity_tag::chamomile}, {"yarrow", entity_tag::yarrow}, {"fern", entity_tag::fern}, {"mugwort", entity_tag::mugwort}, {"poppy", entity_tag::poppy}, {"buildObject", entity_tag::build_object}, {"dropPoint", entity_tag::drop_point},
	{"emptyDraft", entity_tag::empty_draft}, {"emptyPage", entity_tag::empty_page}, {"emptyCell", entity_tag::empty_cell}, {"selectedCell", entity_tag::selected_cell}, {"clapWhirl", entity_tag::clap_whirl}, {"emptyObject", entity_tag::empty_object} };

object_initializer::object_initializer()
= default;


int object_initializer::new_name_id = 0;

shared_ptr<static_object> object_initializer::initialize_static_item(
	const entity_tag tag,
	const sf::Vector2f position,
	const int kind,
	const std::string& name,
	const int count,
	const biome biome,
	const bool mirrored,
	const std::vector<std::pair<entity_tag, int>>& inventory)
{
	shared_ptr<static_object> item;
	const auto calculated_kind = kind ? kind : get_random_type_by_biome(tag, biome);
	new_name_id++;
	const auto calculated_name = name.empty() ? std::to_string(new_name_id) : name;

	switch (tag)
	{
		case entity_tag::brazier:
		{
			item = make_shared<brazier>(calculated_name, position, calculated_kind);
			break;
		}
		case entity_tag::tree:
		{
			item = make_shared<forest_tree>(calculated_name, position, calculated_kind);
			break;
		}
		case entity_tag::ground:
		{
			item = make_shared<ground>(calculated_name, position, calculated_kind);
			break;
		}
		case entity_tag::ground_connection:
		{
			item = make_shared<ground_connection>(calculated_name, position, calculated_kind);
			break;
		}
		/*case entity_tag::grass:
		{
			item = new grass("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::chamomile:
		{
			item = new chamomile("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::wreathTable:
		{
			item = new wreath_table("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::yarrow:
		{
			item = new yarrow("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::fern:
		{
			item = new fern("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::hareTrap:
		{
			item = new hare_trap("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::rock:
		{
			item = new rock("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::fence:
		{
			item = new fence("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::stump:
		{
			item = new stump("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::droppedLoot:
		{
			item = new dropped_loot("item", sf::Vector2f(0, 0), -1, count);
			break;
		}
		case entity_tag::mugwort:
		{
			item = new mugwort("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::totem:
		{
			item = new totem("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::mushroom:
		{
			item = new mushroom("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::log:
		{
			item = new mr::log("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::bush:
		{
			item = new bush("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::lake:
		{
			item = new lake("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::root:
		{
			item = new root("item", sf::Vector2f(0, 0), -1);
			break;
		}
		case entity_tag::roof:
		{
			item = new roof("item", sf::Vector2f(0, 0), -1);
			break;
		}*/
		default:
			return nullptr;
	}

	item->init_pedestal();

	if (!inventory.empty())
		item->inventory = inventory;

	if (!mirrored || item->is_multi_ellipse)
		item->cancel_mirroring();

	return item;
}

shared_ptr<dynamic_object> object_initializer::initialize_dynamic_item(
	const entity_tag tag,
	const sf::Vector2f position,
	const std::string& name,
	const shared_ptr<world_object>& owner)
{
	shared_ptr<dynamic_object> item;
	new_name_id++;
	const auto calculated_name = name.empty()
		? mapped_tags.at(item->tag) + "_" + std::to_string(new_name_id)
		: name;
	
	switch (tag)
	{
	case entity_tag::hero:
	{
		item = make_shared<deerchant>(calculated_name, position, 1);
		break;
	}
	/*case entity_tag::wolf:
	{
		item = new wolf("item", sf::Vector2f(0, 0));
		name_of_image = "Game/worldSprites/wolf/stand/down/1";
		break;
	}
	case entity_tag::hare:
	{
		item = new hare("item", sf::Vector2f(0, 0));
		name_of_image = "Game/worldSprites/hare/stand/down/1";
		break;
	}
	case entity_tag::deer:
	{
		item = new deer("item", sf::Vector2f(0, 0));
		name_of_image = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case entity_tag::bear:
	{
		item = new bear("item", sf::Vector2f(0, 0));
		name_of_image = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case entity_tag::owl:
	{
		item = new owl("item", sf::Vector2f(0, 0));
		name_of_image = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case entity_tag::noose:
	{
		item = new noose("item", sf::Vector2f(0, 0), owner);
		name_of_image = "Game/worldSprites/noose/nooseLoop/1";
		break;
	}
	/*case Tag::clapWhirl:
	{
		item = new ClapWhirl("item", sf::Vector2f(0, 0), owner);
		nameOfImage = "Game/worldSprites/nightmare3/clap/whirl";
		break;
	}
	case entity_tag::owlBoss:
	{
		item = new owl_boss("item", sf::Vector2f(0, 0));
		name_of_image = "Game/worldSprites/owlBoss/stand/down/1";
		break;
	}
	case entity_tag::nightmare1:
	{
		item = new nightmare_first("item", sf::Vector2f(0, 0));
		name_of_image = "Game/worldSprites/nightmare1/stand/down/1";
		break;
	}
	case entity_tag::nightmare2:
	{
		item = new nightmare_second("item", sf::Vector2f(0, 0));
		name_of_image = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	case entity_tag::nightmare3:
	{
		item = new nightmare_third("item", sf::Vector2f(0, 0));
		name_of_image = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}*/
	default:
		return nullptr;
	}
	item->init_pedestal();
	return item;
}

int object_initializer::get_random_type_by_biome(const entity_tag tag, const biome biome)
{
	switch (tag)
	{
		case entity_tag::tree:
		{
			if (biome == biome::birch_grove)
				return rand() % 7 + 1;
			if (biome == biome::dark_woods)
				return rand() % 6 + 8;
			if (biome == biome::swampy_trees)
				return rand() % 5 + 14;
			break;
		}
		case entity_tag::grass:
		{
			if (biome == biome::birch_grove)
				return rand() % 8 + 1;
			if (biome == biome::dark_woods)
				return rand() % 13 + 9;
			if (biome == biome::swampy_trees)
				return rand() % 9 + 22;
			break;
		}
		case entity_tag::rock:
		{
			if (biome == biome::birch_grove)
				return rand() % 8 + 1;
			if (biome == biome::dark_woods)
				return rand() % 5 + 9;
			if (biome == biome::swampy_trees)
				return  rand() % 5 + 14;
			break;
		}
		case entity_tag::stump:
		{
			if (biome == biome::birch_grove)
				return rand() % 4 + 1;
			if (biome == biome::dark_woods)
				return rand() % 6 + 5;
			if (biome == biome::swampy_trees)
				return rand() % 3 + 11;
			break;
		}
		case entity_tag::bush:
		{
			if (biome == biome::birch_grove)
				return rand() % 8 + 1;
			if (biome == biome::swampy_trees)
				return rand() % 7 + 9;
			break;
		}
		case entity_tag::mushroom:
		{
			if (biome == biome::birch_grove)
				return rand() % 3 + 1;
			if (biome == biome::dark_woods)
				return rand() % 9 + 4;
			if (biome == biome::swampy_trees)
				return rand() % 4 + 13;
			break;
		}
		default: return 1;
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
	for (const auto& item : items)
		dynamic_items.push_back(dynamic_pointer_cast<dynamic_object>(item));
	return dynamic_items;
}

