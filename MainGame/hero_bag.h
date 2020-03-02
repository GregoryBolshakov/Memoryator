#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "sprite_structures.h"

using namespace sf;

enum bag_state { bag_open = 1, bag_closed = 2, bag_opening = 3, bag_closing = 4, ejected = 5 };

struct bag_sprite_chain
{
	sprite_chain_element* closed_bag = new sprite_chain_element();
	sprite_chain_element* closed_bag_selected = new sprite_chain_element();
	sprite_chain_element* closed_bag_big = new sprite_chain_element();
	sprite_chain_element* opened_bag = new sprite_chain_element();
	sprite_chain_element* opened_bag_selected = new sprite_chain_element();
};

struct cell
{
	Vector2f position;
	std::pair<entity_tag, int> content;
};

class hero_bag
{
public:
	hero_bag();
	~hero_bag();
	void initialize(Vector2f position, bool is_selectable, std::vector<std::pair<entity_tag, int>> inventory = test_inventory);
	static cell create_cell(Vector2f position, std::pair<entity_tag, int> content);
	[[nodiscard]] Vector2f get_position() const { return position_; }
	[[nodiscard]] Vector2f get_size_closed() const { return size_closed_; }
	[[nodiscard]] Vector2f get_size_open() const { return size_open_; }
	[[nodiscard]] Vector2f get_last_mouse_pos() const { return last_mouse_pos_; }
	[[nodiscard]] float get_radius() const;
	int get_selected_cell(Vector2f position);
	bool static can_afford(std::vector<std::pair<entity_tag, int>> recipe, std::vector<hero_bag>* bags, cell* held_item = nullptr);
	void static take_items(std::vector<std::pair<entity_tag, int>> recipe, std::vector<hero_bag>* bags, cell* held_item = nullptr);
	bool static put_items_in(std::vector<std::pair<entity_tag, int>>* loot, std::vector<hero_bag>* bags);
	bool static put_item_in(std::pair<entity_tag, int>* loot, std::vector<hero_bag>* bags);
	static std::vector<std::pair<entity_tag, int>> cells_to_inventory(const std::vector<cell>& cells);
	void mouse_move();
	void set_position(const Vector2f position) { this->position_ = position; }
	void fix_cells();
	void fix_pos();

	static std::unordered_map<entity_tag, int> items_max_count;
	static std::vector<std::pair<entity_tag, int>> test_inventory;
	static std::vector<std::pair<entity_tag, int>> empty_inventory;

	Vector2f texture_open_offset;
	Vector2f texture_closed_offset;
	std::vector<cell> cells;

	//animation
	sprite_chain_element* prepare_sprite(long long elapsed_time, std::map<pack_tag, sprite_pack>* packs_map);
	void draw_circuit(RenderWindow* window);

	long long state_change_time = 100000;
	long long state_changing_time = 0;
	float speed = 0.001f;
	bag_state current_state = bag_closed;
	float min_dist_to_border = 0;
	float closed_radius = 0;
	float opened_radius = 0;
	Vector2f selection_zone_closed_offset;
	Vector2f selection_zone_opened_offset;
	Vector2f shift_vector = { 0, 0 };
	Vector2f move_position = { -1, -1 };
	bool ready_to_change_state = false;
	bool ready_to_eject = false;
	bool was_moved = false;
private:
	bag_sprite_chain bag_sprite_chain_;
	Vector2f position_;
	Vector2f texture_size_closed_;
	Vector2f texture_size_open_;
	Vector2f size_closed_;
	Vector2f size_open_;
	Vector2f last_mouse_pos_ = { 0, 0 };
	bool is_selectable_{};
	std::vector<Vector2f> cells_pos_ = { {-0.1f, -0.15f}, {0.135f, -0.15f}, {-0.2f, 0.014f}, {0.018f, 0.0f}, {0.241f, 0.004f}, {-0.08f, 0.150f}, {0.131f, 0.150f} };
};
