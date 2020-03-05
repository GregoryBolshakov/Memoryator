#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

#include "world_object.h"

const float inf = 1e3;

class grid_list
{
public:
	grid_list();
	grid_list(int width, int height, Vector2f size, Vector2f micro_size);
	~grid_list();

	[[nodiscard]] std::unordered_map<std::string, std::pair<int, int>> get_items_of_greed() const { return items_; }
	[[nodiscard]] Vector2f get_block_size() const;
	[[nodiscard]] int get_index_by_point(float x, float y) const;
	[[nodiscard]] Vector2f get_point_by_index(int index) const;
	[[nodiscard]] int get_micro_block_by_point(float x, float y) const;
	[[nodiscard]] Vector2f get_point_by_micro_block(int micro_block_index) const;
	[[nodiscard]] std::vector<std::vector<world_object*>> get_cells() const { return cells_; }
	
	size_t get_block_items_amount(const int index) { return cells_[index].size(); }
	void make_route(Vector2f start_pos, Vector2f finish_pos, float upper_left_x, float upper_left_y, float bottom_right_x, float bottom_right_y, int permissible_distance = 1);
	void add_item(world_object* item, const std::string& name, float x, float y);
	world_object* get_item_by_name(const std::string& name);
	std::vector<world_object*> get_items(float upper_left_x, float upper_left_y, float bottom_right_x, float bottom_right_y);
	std::vector<world_object*> get_items(int block_index);
	void update_item_position(const std::string& name, float x, float y);

	
	void clear_cell(int cell_index);
	void delete_item(const std::string& name);
	void set_locked_micro_blocks(world_object* item, bool value = false, bool dynamic_matrix = false);
	void bound_dynamic_matrix(std::vector<std::vector<bool>>* matrix) { this->dynamic_micro_block_matrix = matrix; }
	
	[[nodiscard]] size_t get_size() const { return items_.size(); }

	[[nodiscard]] std::vector<int> get_blocks_around(float upper_left_x, float upper_left_y, float bottom_right_x, float bottom_right_y) const;
	[[nodiscard]] std::vector<int> get_blocks_in_sight(float upper_left_x, float upper_left_y, float bottom_right_x, float bottom_right_y) const;
	bool is_intersect_with_others(world_object* object) const;

	std::vector<std::vector<bool>> micro_block_matrix;
	std::vector<std::vector<bool>>* dynamic_micro_block_matrix = nullptr;
	std::vector<std::vector<float>> distances;
	std::vector<std::vector<std::pair<int, int>>> previous;
	std::vector<std::pair<int, int>> route = {{}};
	std::pair<int, int> reserve_destination = std::make_pair(-1, -1);

private:
	int width_;
	int height_;
	Vector2f size_ = { 0, 0 };
	Vector2f micro_size_ = { 0, 0 };
	std::vector<std::vector<world_object*>> cells_;
	std::unordered_map<std::string, std::pair<int, int>> items_;	
	void bfs(int x_border, int y_border, int start_x, int start_y, int finish_x, int finish_y);
	int width_to_size_x_{};
	int height_to_size_y_{};
	int width_to_micro_x_{};
	int height_to_micro_y_{};
};


