#pragma once

#include <unordered_map>
#include <windows.h>

#include "brazier.h"
#include "camera_system.h"
#include "dynamic_object.h"
#include "effects_system.h"
#include "empty_object.h"
#include "inventory_system.h"
#include "light_system.h"
#include "pedestal_controller.h"
#include "scale_system.h"
#include "shape_chain_element.h"
#include "world_generator.h"

using namespace sf;

class world_handler
{
public:
	world_handler(
		int width,
		int height,
		shared_ptr<camera_system> camera_system,
		shared_ptr<std::map<pack_tag, sprite_pack>> packs_map);
	
	~world_handler();

	//adding to the grid
	void birth_objects();

	// Getters
	const shared_ptr<grid_list>& get_static_grid() const { return static_grid_; }
	const shared_ptr<grid_list>& get_dynamic_grid() const { return dynamic_grid_; }
	const shared_ptr<inventory_system>& get_inventory_system() const { return inventory_system_; }
	//build_system& getBuildSystem() { return buildSystem; }
	const shared_ptr<world_generator>& get_world_generator() const { return world_generator_; }
	const shared_ptr<light_system>& get_light_system() const { return light_system_; }
	std::string get_mouse_display_name() const { return mouse_display_name_; }
	const shared_ptr<world_object>& get_selected_object() const { return selected_object_; }
	std::vector<shared_ptr<world_object>> get_local_terrain() const { return local_terrain_; }

	// Save-load logic
	void clear_world();
	void load();
	void save();
	
	// Base (draw, interact)
	void interact(Vector2f render_target_size, long long elapsed_time, Event event);
	void handle_events(Event& event);
	std::vector<std::unique_ptr<drawable_chain_element>> prepare_sprites(long long elapsed_time, bool only_background = false);
	//void draw(RenderWindow& window, long long elapsedTime);
	void set_item_from_build_system();
	//void drawVisibleItems(RenderWindow& window, long long elapsedTime, std::vector<SpriteChainElement*> sprites);
	Vector2f world_upper_left, world_bottom_right;
	//void runBuildSystemDrawing(RenderWindow& window, long long elapsedTime);
	//void runInventorySystemDrawing(RenderWindow& window, long long elapsedTime);
	void run_world_generator();
	pedestal_controller pedestal_controller;

	// Hero
	shared_ptr<dynamic_object> focused_object{};
	shared_ptr<brazier> main_object{};

	// Events
	void on_mouse_up(int current_mouse_button);
	bool get_hero_book_visibility() const { return is_hero_book_visible_; }
	void change_book_visibility() { is_hero_book_visible_ = !is_hero_book_visible_; }

	//void setObjectToBuild(entity_tag tag, int type = 1, bool instantBuild = false) { buildSystem.selected_object = tag; buildSystem.build_type = type; buildSystem.instant_build = instantBuild; }
	std::string debug_info = "";
private:
	// Hero
	const std::string hero_texture_name_ = "Game/worldSprites/hero/move/body/down/1.png";
	bool is_hero_book_visible_ = false;

	int width_;
	int height_;
	Vector2f block_size_ = { 1000, 1000 };
	Vector2f micro_block_size_ = { 20, 20 };
	shared_ptr<std::map<pack_tag, sprite_pack>> packs_map_;
	std::string sprite_name_file_directory_ = "Game/objects.txt";
	bool fixed_climbing_beyond(Vector2f &pos) const;

	// Time logic
	Clock timer_;
	long long time_for_new_save_ = 0;
	long long time_after_save_ = long(6e7);

	// Selection logic
	void set_transparent(std::vector<shared_ptr<world_object>>& visible_items);
	std::string mouse_display_name_;
	shared_ptr<world_object> selected_object_ = focused_object;
	Vector2f mouse_position() const;

	// Systems
	shared_ptr<camera_system> camera_system_; // initialized in constructor
	shared_ptr<effects_system> effect_system_ = make_shared<effects_system>();
	shared_ptr<inventory_system> inventory_system_ = make_shared<inventory_system>();
	//build_system buildSystem = make_shared<buildSystem>();
	shared_ptr<light_system> light_system_ = make_shared<light_system>();

	// Grids
	shared_ptr<grid_list> static_grid_{}; // initialized in constructor
	shared_ptr<grid_list> dynamic_grid_{}; // initialized in constructor
	std::vector<sprite_chain_element*> visible_background_, visible_terrain_;
	std::vector<shared_ptr<world_object>> local_terrain_;

	shared_ptr<world_generator> world_generator_{}; // initialized in constructor

	// Test
	std::vector<std::pair<entity_tag, int>>* test_inv_ = new std::vector<std::pair<entity_tag, int>>({ {entity_tag::chamomile, 2}, {entity_tag::chamomile, 2}, {entity_tag::chamomile, 2} });
};
