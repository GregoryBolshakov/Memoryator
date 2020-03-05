#pragma once

#include <SFML/Graphics.hpp>

#include "effects_system.h"
#include "hero_bag.h"
#include "sprite_structures.h"
#include "text_system.h"

using namespace sf;

class inventory_system
{
public:
	inventory_system();
	~inventory_system();
	void init();
	std::vector<drawable_chain_element*> prepare_sprites(long long elapsed_time, std::map<pack_tag, sprite_pack>* packs_map);
	//void drawInventory(std::vector<std::pair<Tag, int>>* inventory, Vector2f position, RenderWindow& window);
	void reset_animation_values();
	void on_mouse_up();
	void inventory_bounding(std::vector<hero_bag>* bags) { bound_bags_ = bags; }
	void interact(long long elapsed_time);
	void reset_picked_cell() { picked_cell_ = nullptr; }
	bool get_used_mouse() const { return used_mouse_; }	
	cell& get_held_item() { return held_item_; }

	bool was_drawing = false;
	std::string debug_info = "", cursor_text = "";

	Sprite * selected_cell_background{};
	CircleShape bag_pos_dot;
private:
	//hero bags
	std::vector<hero_bag>* bound_bags_{};
	int current_moving_bag_ = -1;
	float min_dist_to_closed_ = 10e4;
	float min_dist_to_open_ = 10e4;

	//another inventories 
	int animation_counter_ = 1;

	//held item
	cell held_item_;
	float held_item_speed_ = 0;
	float drop_zone_radius_{};
	std::pair <entity_tag, int>* picked_cell_ = nullptr;
	std::string cells_file_directory_ = "Game/inventorySprites/cellsPath.txt";
	std::string bags_file_directory_ = "Game/inventorySprites/bagsPath.txt";

	void crash_into_other_bags(int cnt) const;
	void move_other_bags(int cur) const;
	static void init_max_counts(const std::string& file_path = "Game/maxCounts.txt");
	std::vector<bool> visited_in_displacement_;
	bool used_mouse_ = false;
	bool cursor_blur_using_ = false;
	bool success_init_ = false;
	Vector2f cursor_text_pos_ = {0, 0};
	text_system text_writer_;
	effects_system effects_system_;
};
