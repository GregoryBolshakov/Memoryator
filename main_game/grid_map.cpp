#include "grid_map.h"

#include <cmath>
#include <queue>

void grid_map::init_matrices()
{
	const auto route_matrix_size = sf::Vector2u(world_metrics::constant_zone.width / world_metrics::route_block_size.x + 1,
		world_metrics::constant_zone.height / world_metrics::route_block_size.y + 1);
	locked_route_blocks_ = std::make_shared<bool_matrix_t>(route_matrix_size.x, std::vector<bool>(route_matrix_size.y));

	constant_blocks_ = std::make_shared<bool_matrix_t>(world_metrics::matrix_size.x, std::vector<bool>(world_metrics::matrix_size.y));

	micro_block_matrix.resize(4000, std::vector<bool>(4000, true));
	distances.resize(4000, std::vector<float>(4000, 1000));
	previous.resize(4000, std::vector<std::pair<int, int>>(4000, { -1, -1 }));
}

route_t grid_map::make_route(
	const sf::Vector2f start_pos,
	const sf::Vector2f finish_pos,
	const float upper_left_x,
	const float upper_left_y,
	const float bottom_right_x,
	const float bottom_right_y)
{
	/*route.clear();

	if (abs(start_pos.x - finish_pos.x) / micro_size_.x + abs(start_pos.y - finish_pos.y) / micro_size_.y < 1)
		return {};

	const auto x_micro_blocks_count = int(ceil((bottom_right_x - upper_left_x) / micro_size_.x));
	const auto y_micro_blocks_count = int(ceil((bottom_right_y - upper_left_y) / micro_size_.y));

	const auto start_x_ind = int(round(upper_left_x / micro_size_.x));
	const auto start_y_ind = int(round(upper_left_y / micro_size_.y));

	auto cur_micro_block_x = int(round(start_pos.x / micro_size_.x));
	auto cur_micro_block_y = int(round(start_pos.y / micro_size_.y));

	auto last_micro_block_x = int(round(finish_pos.x / micro_size_.x));
	auto last_micro_block_y = int(round(finish_pos.y / micro_size_.y));

	auto is_break = false;
	
	if (!micro_block_matrix[cur_micro_block_x][cur_micro_block_y])
	{		
		for (auto i = -1; i <= 1; i++)
		{
			for (auto j = -1; j <= 1; j++)
				if (cur_micro_block_x + i > 0 && cur_micro_block_x + i < width_to_micro_x_ &&
					cur_micro_block_y + j > 0 && cur_micro_block_y + j < height_to_micro_y_ && 
					micro_block_matrix[cur_micro_block_x + i][cur_micro_block_y + j])
				{
					cur_micro_block_x += i;
					cur_micro_block_y += j;
					is_break = true;
					break;
				}
			if (is_break)
				break;
		}
	}

	if (!micro_block_matrix[last_micro_block_x][last_micro_block_y])
	{
		const int max_search_distance = 20;

		for (int search_distance = 1; search_distance < max_search_distance; search_distance++)
		{
			for (int cnt_x = -search_distance; cnt_x <= search_distance; cnt_x++)
				if (last_micro_block_x + cnt_x > 0 && last_micro_block_x + cnt_x < width_to_micro_x_ &&
					last_micro_block_y - search_distance > 0 && last_micro_block_y - search_distance < height_to_micro_y_ &&
					micro_block_matrix[last_micro_block_x + cnt_x][last_micro_block_y - search_distance])
				{
					last_micro_block_x += cnt_x;
					last_micro_block_y += -search_distance;
					is_break = true;
					break;
				}

			if (is_break)
				break;

			for (int cnt_x = -search_distance; cnt_x <= search_distance; cnt_x++)
				if (last_micro_block_x + cnt_x > 0 && last_micro_block_x + cnt_x < width_to_micro_x_ &&
					last_micro_block_y + search_distance > 0 && last_micro_block_y + search_distance < height_to_micro_y_ &&
					micro_block_matrix[last_micro_block_x + cnt_x][last_micro_block_y + search_distance])
				{
					last_micro_block_x += cnt_x;
					last_micro_block_y += search_distance;
					is_break = true;
					break;
				}

			if (is_break)
				break;

			for (int cnt_y = -search_distance; cnt_y <= search_distance; cnt_y++)
				if (last_micro_block_x - search_distance && last_micro_block_x - search_distance < width_to_micro_x_ &&
					last_micro_block_y + cnt_y > 0 && last_micro_block_y + cnt_y < height_to_micro_y_ &&
					micro_block_matrix[last_micro_block_x - search_distance][last_micro_block_y + cnt_y])
				{
					last_micro_block_x += -search_distance;
					last_micro_block_y += cnt_y;
					is_break = true;
					break;
				}

			if (is_break)
				break;

			for (int cnt_y = -search_distance; cnt_y <= search_distance; cnt_y++)
				if (last_micro_block_x + search_distance && last_micro_block_x + search_distance < width_to_micro_x_ &&
					last_micro_block_y + cnt_y > 0 && last_micro_block_y + cnt_y < height_to_micro_y_ &&
					micro_block_matrix[last_micro_block_x + search_distance][last_micro_block_y + cnt_y])
				{
					last_micro_block_x += search_distance;
					last_micro_block_y += cnt_y;
					is_break = true;
					break;
				}
			
			if (is_break)
				break;
		}
	}

	if (!micro_block_matrix[last_micro_block_x][last_micro_block_y])
	{
		route.clear();
		return {};
	}

	for (auto i = start_x_ind; i < start_x_ind + x_micro_blocks_count; i++)	
		for (auto j = start_y_ind; j < start_y_ind + y_micro_blocks_count; j++)		
			distances[i][j] = inf;

	distances[cur_micro_block_x][cur_micro_block_y] = 0;
	bfs(start_x_ind + x_micro_blocks_count, start_y_ind + y_micro_blocks_count, cur_micro_block_x, cur_micro_block_y, last_micro_block_x, last_micro_block_y);*/

	return route_t{};
}

void grid_map::bfs(const int x_border, const int y_border, int start_x, int start_y, const int finish_x, const int finish_y)
{
	/*std::queue<std::pair<int, int>> q;
	q.push(std::make_pair(start_x, start_y));

	float step;
	while (!q.empty()) 
	{
		const auto v = q.front();
		q.pop();
		auto is_break = false;

		for (auto i = -1; i <= 1; i++)
			for (auto j = -1; j <= 1; j++)
			{				
				if (i == 0 && j == 0)
					continue;
				if (abs(i) == abs(j))
					step = 1.42f;
				else
					step = 1.0f;

				auto to = std::make_pair(v.first + i, v.second + j);
				if (to.first < 0 || to.first > x_border || to.second < 0 || to.second > y_border)
					continue;

				if (abs(previous[v.first][v.second].first - v.first) + abs(previous[v.first][v.second].second - v.second) <= 2 &&
					v.first - previous[v.first][v.second].first == to.first - v.first && v.second - previous[v.first][v.second].second == to.second - v.second)
					step -= 0.0001f;


				if (distances[to.first][to.second] > distances[v.first][v.second] + step && (micro_block_matrix[to.first][to.second] != 0 || to.first == finish_x && to.second == finish_y))
				{
					distances[to.first][to.second] = distances[v.first][v.second] + step;
					previous[to.first][to.second] = v;
					q.push(to);
				}
				if (to.first == finish_x && to.second == finish_y)
				{
					is_break = true;
					break;
				}
			}
		if (is_break)
			break;
	}

	auto can_create_route = true;

	if (finish_x == -1 || distances[finish_x][finish_y] == inf)
	{
		can_create_route = false;
		route.clear();		
	}

	if (can_create_route)
	{
		std::vector<std::pair<int, int>> path;

		auto current_matrix_ceil = previous[finish_x][finish_y];
		float iterations = 0;
		while (true)
		{
			iterations++;
			if (current_matrix_ceil == std::make_pair(-1, -1) || iterations > distances[finish_x][finish_y])
			{
				route.clear();
				return;
			}

			if (abs(previous[current_matrix_ceil.first][current_matrix_ceil.second].first - current_matrix_ceil.first) + abs(previous[current_matrix_ceil.first][current_matrix_ceil.second].second - current_matrix_ceil.second) > 2)
			{
				route.clear();
				return;
			}

			path.push_back(current_matrix_ceil);			
			current_matrix_ceil = previous[current_matrix_ceil.first][current_matrix_ceil.second];
			if (current_matrix_ceil == std::make_pair(start_x, start_y))
				break;
		}
		if (!path.empty())
			reverse(path.begin(), path.end());

		if (!path.empty() && path[0] == std::make_pair(start_x, start_y))
			path.erase(path.begin() + 0);

		// cut corners
		/*auto cnt = 0;
		while (cnt < int(path.size() - 3))
			if (path[cnt + 1].first - path[cnt].first == path[cnt + 2].first - path[cnt + 1].first && path[cnt + 1].second - path[cnt].second == path[cnt + 2].second - path[cnt + 1].second)
				path.erase(path.begin() + cnt);
			else
				cnt++;*/
		/*//------------

		route = path;
	}*/
}

sf::Vector2u grid_map::get_block_by_point(const sf::Vector2f point)
{
	return sf::Vector2u(point / world_metrics::block_size);
}

sf::Vector2f grid_map::get_point_by_block(sf::Vector2u index)
{
	return sf::Vector2f(index) * world_metrics::block_size;
}

void grid_map::set_locked_micro_blocks(const std::shared_ptr<world_object>& item, const bool value)
{
	/*const auto world_item = dynamic_pointer_cast<world_object>(item);
	if (world_item)
		for (const auto block : world_item->get_locked_micro_blocks())
		{
			if (!(block.x < 0 || block.x > width_to_micro_x_ || block.y < 0 || block.y > height_to_micro_y_))
				micro_block_matrix[block.x][block.y] = value;
		}*/
}
