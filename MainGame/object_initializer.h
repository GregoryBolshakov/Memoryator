#pragma once

#include "StaticObject.h"
#include "SpriteStructures.h"
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

enum Biomes { SwampyTrees = 1, DarkWoods = 2, BirchGrove = 3, FlowerValley = 4 };

class object_initializer
{
public:
	object_initializer();
	~object_initializer();
	static StaticObject* initializeStaticItem(
		Tag itemClass,
		Vector2f itemPosition,
		int itemType,
		const std::string& itemName, 
		int count,
		Biomes biome,		
		std::map<PackTag, sprite_pack>* packsMap,
		bool mirrored = true,
		const std::vector<std::pair<Tag, int>>& inventory = {});
	static int newNameId;
	static int getRandomTypeByBiome(WorldObject* object, Biomes biome);
	static DynamicObject* initializeDynamicItem(Tag itemClass,
		Vector2f itemPosition,
		const std::string& itemName,
		std::map<PackTag, sprite_pack>* packsMap,
		WorldObject* owner = nullptr);
	static std::vector<StaticObject*> vectorCastToStatic(const std::vector<WorldObject*>& items);
	static std::vector<DynamicObject*> vectorCastToDynamic(std::vector<WorldObject*> items);

	static std::map<Tag, std::string> mappedTags;
	static std::map<std::string, Tag> mappedStrings;
};
