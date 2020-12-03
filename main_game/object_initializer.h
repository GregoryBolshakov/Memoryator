#pragma once

#include <map>
#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>

class world_object;
class dynamic_object;

class static_object;
class sprite_pack;
class rock;
class ground;
class ground_connection;
class dropped_loot;
class grass;
class ground;
class spawn;
class chamomile;
class yarrow;
class mugwort;
class fern;
class brazier;
class totem;
class hare_trap;
class fence;
class stump;
class forest_tree;
class wreath_table;
class mushroom;
class log;
class bush;
class lake;
class root;
class roof;

class monster;
class deerchant;
class wolf;
class hare;
class deer;
class bear;
class owl;
class owl_boss;
class nightmare_first;
class nightmare_second;
class nightmare_third;
class noose;
class clap_whirl;

enum class entity_tag;
enum class pack_tag;
enum class biome;

enum class biome { swampy_trees = 1, dark_woods = 2, birch_grove = 3, flower_valley = 4 };

class object_initializer
{
public:
	object_initializer();
	~object_initializer() = default;
	static std::shared_ptr<static_object> initialize_static_item(
		entity_tag tag,
		sf::Vector2f position,
		int kind,
		const std::string& name,
		int count,
		biome biome,
		bool mirrored = true,
		const std::vector<std::pair<entity_tag, int>>& inventory = {});
	static int new_name_id;
	static int get_random_type_by_biome(entity_tag tag, biome biome);
	static std::shared_ptr<dynamic_object> initialize_dynamic_item(
		entity_tag tag,
		sf::Vector2f position,
		const std::string& name,
		const std::shared_ptr<world_object>& owner = nullptr);
	static std::vector<std::shared_ptr<static_object>> vector_cast_to_static(const std::vector<std::shared_ptr<world_object>>& items);
	static std::vector<std::shared_ptr<dynamic_object>> vector_cast_to_dynamic(const std::vector<std::shared_ptr<world_object>>& items);

	static std::map<entity_tag, std::string> mapped_tags;
	static std::map<std::string, entity_tag> mapped_strings;
};
