#pragma once

#include "pedestal_controller.h"

#include <memory>
#include <unordered_map>
#include <Windows.h>
#include <SFML/System/Vector2.hpp>

using std::shared_ptr;
using std::weak_ptr;
using sf::Vector2f;

class camera_system;
class sprite_pack;
class grid_map;
class inventory_system;
class light_system;
class effect_system;
class light_system;
class world_generator;
class dynamic_object;
class drawable_chain_element;
class brazier;
class effects_system;
enum class pack_tag;

class world_handler
{
public:
	world_handler(const shared_ptr<camera_system>& camera_system);
	~world_handler();

	//adding to the grid
	void birth_objects();

	// Getters
	const shared_ptr<grid_map>& get_grid() const { return grid_map_; }
	const shared_ptr<inventory_system>& get_inventory_system() const { return inventory_system_; }
	//build_system& getBuildSystem() { return buildSystem; }
	const shared_ptr<world_generator>& get_world_generator() const { return world_generator_; }
	const shared_ptr<light_system>& get_light_system() const { return light_system_; }
	const weak_ptr<camera_system>& get_camera_system() const { return camera_system_; }
	std::string get_mouse_display_name() const { return mouse_display_name_; }
	const weak_ptr<world_object>& get_mouse_selected_object() const { return mouse_selected_object_; }
	const weak_ptr<dynamic_object>& get_player() const { return player_; }
	const weak_ptr<brazier>& get_main_object() const { return main_object_; }

	// Save-load logic
	void load();
	void save();
	
	// Base (draw, interact)
	sf::Vector2f interact_movement(const shared_ptr<dynamic_object>& dynamic_item, long long elapsed_time) const;
	void interact(long long elapsed_time);
	void handle_events(sf::Event& event);
	std::vector<std::unique_ptr<drawable_chain_element>> prepare_sprites(long long elapsed_time, bool only_background = false);
	void set_item_from_build_system();
	sf::Vector2f world_upper_left, world_bottom_right;
	//void runBuildSystemDrawing(RenderWindow& window, long long elapsedTime);
	//void runInventorySystemDrawing(RenderWindow& window, long long elapsedTime);
	void run_world_generator();
	pedestal_controller pedestal_controller;

	// sf::Events
	void on_mouse_up(int current_mouse_button);

	//void setObjectToBuild(entity_tag tag, int type = 1, bool instantBuild = false) { buildSystem.selected_object = tag; buildSystem.build_type = type; buildSystem.instant_build = instantBuild; }
	std::string debug_info = "";
private:
	// Hero
	std::weak_ptr<dynamic_object> player_;
	std::weak_ptr<brazier> main_object_;
	sf::Vector2f whole_world_offset_;

	// Systems
	weak_ptr<camera_system> camera_system_;
	shared_ptr<effects_system> effect_system_;
	shared_ptr<inventory_system> inventory_system_;
	//build_system buildSystem = make_shared<buildSystem>();
	shared_ptr<light_system> light_system_;

	static bool fixed_climbing_beyond(sf::Vector2f &pos);

	// Time logic
	sf::Clock timer_;

	// Selection logic
	void mouse_selection_logic(const shared_ptr<world_object>& visible_item);
	std::string mouse_display_name_;
	weak_ptr<world_object> mouse_selected_object_;
	float min_selection_capacity_ = 1e6f, min_selection_distance_ = 1e6f;

	// Grids
	shared_ptr<grid_map> grid_map_;
	shared_ptr<world_generator> world_generator_;
};
