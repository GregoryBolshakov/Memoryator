#pragma once

#include "object_initializer.h"

#include <map>
#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>

class world_object;
class static_object;
class dynamic_object;
class scale_system;
class sprite_pack;
class grid_map;
enum class pack_tag;
enum class entity_tag;

using biome_matrix_t = std::vector<std::vector<biome>>;

class world_generator
{
public:
	world_generator(
		  std::shared_ptr<grid_map> grid_map
		, const std::shared_ptr<scale_system>& scale_system
		, const std::shared_ptr<std::map<pack_tag, sprite_pack>>& packs_map);

	void primordial_generation();
	void fill_zone();

	const std::shared_ptr<static_object>& initialize_static_item(
		  entity_tag item_class
		, sf::Vector2f item_position
		, int item_type
		, const std::string& item_name
		, int count = 1
		, biome biome = biome::dark_woods
		, bool mirrored = true
		, const std::vector<std::pair<entity_tag, int>>& inventory = {});
	
	const std::shared_ptr<dynamic_object>& initialize_dynamic_item(
		  entity_tag item_class
		, sf::Vector2f item_position
		, const std::string& item_name
		, const std::shared_ptr<world_object>& owner = nullptr);

	//active generation
	void in_block_generate(sf::Vector2u index);
	[[nodiscard]] bool whether_block_regeneretable(sf::Vector2u index) const;

	[[nodiscard]] const std::weak_ptr<dynamic_object>& player() const { return player_; }
	[[nodiscard]] const std::weak_ptr<static_object>& main_object() const { return main_object_; }
	[[nodiscard]] const std::map<std::string, std::shared_ptr<static_object>>& all_static_objects() const { return all_static_objects_; }
	[[nodiscard]] const std::map<std::string, std::shared_ptr<dynamic_object>>& all_dynamic_objects() const { return all_dynamic_objects_; }
private:
	std::shared_ptr<grid_map> grid_map_;
	std::weak_ptr<scale_system> scale_system_;
	std::weak_ptr<std::map<pack_tag, sprite_pack>> packs_map_;
	std::weak_ptr<dynamic_object> player_;
	std::weak_ptr<static_object> main_object_;
	std::map<std::string, std::shared_ptr<static_object>> all_static_objects_;
	std::map<std::string, std::shared_ptr<dynamic_object>> all_dynamic_objects_;
	biome_matrix_t biome_matrix;

	// block generation
	void generate_ground(sf::Vector2u index);
};
