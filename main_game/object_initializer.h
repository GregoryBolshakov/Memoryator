#pragma once

#include "static_object.h"
#include "sprite_structures.h"
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
//#include "clap_whirl.h"
#include <unordered_map>

enum biomes { swampy_trees = 1, dark_woods = 2, birch_grove = 3, flower_valley = 4 };

class object_initializer
{
public:
	object_initializer();
	~object_initializer();
	static static_object* initialize_static_item(
		entity_tag item_class,
		Vector2f item_position,
		int item_type,
		const std::string& item_name, 
		int count,
		biomes biome,		
		std::map<pack_tag, sprite_pack>* packs_map,
		bool mirrored = true,
		const std::vector<std::pair<entity_tag, int>>& inventory = {});
	static int new_name_id;
	static int get_random_type_by_biome(world_object* object, biomes biome);
	static dynamic_object* initialize_dynamic_item(
		entity_tag item_class,
		Vector2f item_position,
		const std::string& item_name,
		std::map<pack_tag, sprite_pack>* packs_map,
		world_object* owner = nullptr);
	static std::vector<static_object*> vector_cast_to_static(const std::vector<world_object*>& items);
	static std::vector<dynamic_object*> vector_cast_to_dynamic(std::vector<world_object*> items);

	static std::map<entity_tag, std::string> mapped_tags;
	static std::map<std::string, entity_tag> mapped_strings;
};
