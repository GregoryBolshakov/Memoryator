#pragma once

#include "world_metrics.h"

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

class world_object;

using bool_matrix_t = std::vector<std::vector<bool>>;
using route_t = std::vector<std::pair<int, int>>;

const float inf = 1e3;

class grid_map
{
public:
	grid_map();
	~grid_map() = default;

	static sf::Vector2u get_block_by_point(sf::Vector2f point);
	static sf::Vector2f get_point_by_block(sf::Vector2u index);
	void set_locked_micro_blocks(const std::shared_ptr<world_object>& item, bool value = false);
	route_t make_route(sf::Vector2f start_pos, sf::Vector2f finish_pos, float upper_left_x, float upper_left_y, float bottom_right_x, float bottom_right_y);
	//template<typename T>
	//void process_visible_zone(T function_for_each_block);

	[[nodiscard]] const std::shared_ptr<bool_matrix_t>& get_locked_route_blocks() const { return locked_route_blocks_; }
	[[nodiscard]] const std::shared_ptr<bool_matrix_t>& get_constant_blocks() const { return constant_blocks_; }

	void add_constant_block(const sf::Vector2u index) const { (*constant_blocks_)[index.x][index.y] = true; }

	// route generation-----------------------------------------
	std::vector<std::vector<bool>> micro_block_matrix;
	std::vector<std::vector<float>> distances;
	std::vector<std::vector<std::pair<int, int>>> previous;
	std::vector<std::pair<int, int>> route = {{}};
	std::pair<int, int> reserve_destination = std::make_pair(-1, -1);
	//----------------------------------------------------------
private:
	void bfs(int x_border, int y_border, int start_x, int start_y, int finish_x, int finish_y);

	std::shared_ptr<bool_matrix_t> locked_route_blocks_, constant_blocks_;
};

/*template <typename T>
void grid_map::process_visible_zone(T function_for_each_block)
{
	auto iterator_block = get_block_by_point(sf::Vector2f(world_metrix::visible_area.left, world_metrix::visible_area.top));
} */
