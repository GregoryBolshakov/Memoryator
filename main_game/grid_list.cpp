#include "grid_list.h"

#include <cmath>
#include <queue>

#include "helper.h"

grid_list::grid_list() : width_(0), height_(0)
{
}

Vector2f grid_list::get_block_size() const
{
	return size_;
}

grid_list::grid_list(const int width, const int height, const Vector2f size, const Vector2f micro_size)
{
	this->width_ = width;
	this->height_ = height;
	this->size_ = size;
	this->micro_size_ = micro_size;

	width_to_size_x_ = int(ceil(float(width) / size.x));
	height_to_size_y_ = int(ceil(float(height) / size.y));
	width_to_micro_x_ = int(ceil(float(width) / micro_size.x));
	height_to_micro_y_ = int(ceil(float(height) / micro_size.y));
	
	const auto vector_size = width_to_size_x_ * height_to_size_y_;
	cells_.resize(vector_size);
	micro_block_matrix.resize(4000, std::vector<bool>(4000, true));	
	distances.resize(4000, std::vector<float>(4000, 1000));
	previous.resize(4000, std::vector<std::pair<int, int>>(4000, { -1, -1 }));
}

int grid_list::get_index_by_point(const float x, const float y) const
{
	const auto x1 = width_ / int(size_.x);
	const auto y1 = int(y / size_.y);
	const auto result = x1 * y1 + int(x / size_.x);
	
	return int(result);
}

Vector2f grid_list::get_point_by_index(const int index) const
{
	const auto in_line_number = width_ / int(size_.x);
	const auto in_raw_number = index / in_line_number;

	const auto result= Vector2f(
		float(index % in_line_number) * size_.x,
		float(in_raw_number) * size_.y);

	return result;
}

int grid_list::get_micro_block_by_point(const float x, const float y) const
{
	const auto x1 = width_ / int(micro_size_.x);
	const auto y1 = int(y / micro_size_.y);
	const auto result = x1 * y1 + int(x / micro_size_.x);
	
	return int(result);
}

Vector2f grid_list::get_point_by_micro_block(const int micro_block_index) const
{
	const auto in_line_number = width_ / int(micro_size_.x);
	const auto in_raw_number = micro_block_index / in_line_number;

	const auto result = Vector2f(
		float(micro_block_index % in_line_number) * micro_size_.x,
		float(in_raw_number) * micro_size_.y);

	return result;
}

std::vector<std::pair<int, int>> grid_list::make_route(
	const Vector2f start_pos,
	const Vector2f finish_pos,
	const float upper_left_x,
	const float upper_left_y,
	const float bottom_right_x,
	const float bottom_right_y)
{
	route.clear();

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
	bfs(start_x_ind + x_micro_blocks_count, start_y_ind + y_micro_blocks_count, cur_micro_block_x, cur_micro_block_y, last_micro_block_x, last_micro_block_y);

	return route;
}

void grid_list::bfs(const int x_border, const int y_border, int start_x, int start_y, const int finish_x, const int finish_y)
{
	std::queue<std::pair<int, int>> q;
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
		//------------

		route = path;
	}
}

bool grid_list::is_intersect_with_others(world_object* object) const
{
	/*for (auto&anotherItem : visibleTerrain)
	{
		auto const anotherObject = dynamic_cast<WorldObject*>(anotherItem);
		if (!anotherObject)
			continue;

		if (anotherObject->isBackground || anotherObject->tag == Tag::hero || anotherObject->intangible)
			continue;

		if (anotherItem->isDotsAdjusted && isDotAdjusted)
			continue;
		
		if (Helper::getDist(position, anotherObject->getPosition()) < radius + anotherObject->getRadius())
			return true;
	}*/

	std::map<std::pair<int, int>, bool> check_blocks = {};

	const auto i_start = int(round((object->get_position().x - object->get_micro_block_check_area_bounds().x) / micro_size_.x));
	const auto i_end = int(round((object->get_position().x + object->get_micro_block_check_area_bounds().x) / micro_size_.x));

	const auto j_start = int(round((object->get_position().y - object->get_micro_block_check_area_bounds().y) / micro_size_.y));
	const auto j_end = int(round((object->get_position().y + object->get_micro_block_check_area_bounds().y) / micro_size_.y));

	for (auto i = i_start; i < i_end; i++)
		for (auto j = j_start; j < j_end; j++)
			if (!(i < 0 || i > width_to_micro_x_ || j < 0 || j > height_to_micro_y_) && !micro_block_matrix[i][j])
				check_blocks[{i, j}] = true;

	return object->is_locked_place(check_blocks);
}

void grid_list::set_locked_micro_blocks(const shared_ptr<world_object>& item, const bool value)
{
	const auto world_item = dynamic_pointer_cast<world_object>(item);
	if (world_item)
		for (const auto block : world_item->get_locked_micro_blocks())
		{
			if (!(block.x < 0 || block.x > width_to_micro_x_ || block.y < 0 || block.y > height_to_micro_y_))
				micro_block_matrix[block.x][block.y] = value;
		}
}

void grid_list::add_item(const shared_ptr<world_object>& item, const std::string& name, const float x, const float y)
{
	set_locked_micro_blocks(item);

	if (items_.find(name) != items_.end())
		throw std::invalid_argument("The key " + name + " already exists in the Grid.");

	auto index = get_index_by_point(x, y);

	auto position = std::make_pair(index, static_cast<int>(cells_[index].size()));

	cells_[index].push_back(item);
	
	items_.insert({ name, position });
}

void grid_list::clear_cell(const int cell_index)
{
	for (auto& item : cells_[cell_index])
	{
		set_locked_micro_blocks(item, true);
		items_.erase(items_.find(item->get_name()));
	}
	cells_[cell_index].clear();
}

void grid_list::delete_item(const std::string& name)
{
	const auto position = items_.at(name);
	set_locked_micro_blocks(cells_[position.first][position.second], true);
	
	for (unsigned int i = position.second; i < cells_[position.first].size(); i++)
	{
		auto item_name = cells_[position.first][i]->get_name();
		items_.at(item_name).second -= 1;
	}
	
	cells_[position.first].erase(cells_[position.first].begin() + position.second);
	items_.erase(items_.find(name));
}

shared_ptr<world_object> grid_list::get_item_by_name(const std::string& name)
{
	const auto position = items_.at(name);
	return cells_[position.first][position.second];
}

std::vector<shared_ptr<world_object>> grid_list::get_items(const int block_index)
{
	return cells_[block_index];
}

std::vector<shared_ptr<world_object>> grid_list::get_items(float upper_left_x, float upper_left_y, float bottom_right_x, float bottom_right_y)
{
	std::vector<shared_ptr<world_object>> result{};
	
	if (upper_left_x < 0)
	{
		upper_left_x = 0;
	}
	if (bottom_right_x > static_cast<float>(width_))
	{
		bottom_right_x = static_cast<float>(width_);
	}
	if (upper_left_y < 0)
	{
		upper_left_y = 0;
	}
	if (bottom_right_y > static_cast<float>(height_))
	{
		bottom_right_y = static_cast<float>(height_);
	}
	const auto rows_count = int(ceil(double(bottom_right_y - upper_left_y) / int(size_.y)));
	auto first_column = int(get_index_by_point(upper_left_x, upper_left_y));
	auto last_column = int(get_index_by_point(bottom_right_x, upper_left_y));
	const auto columns_per_row = width_to_size_x_;
	const auto max_column = int(cells_.size()) - 1;

	for (auto i = 0; i <= rows_count; i++)
	{
		if (last_column > max_column)
			last_column = max_column;
		
		for (auto j = first_column; j <= last_column; j++)
		{
			result.insert(result.end(), cells_[j].begin(), cells_[j].end());
		}

		first_column += columns_per_row;
		last_column += columns_per_row;
	}

	return result;
}

std::vector<int> grid_list::get_blocks_around(float upper_left_x, float upper_left_y, float bottom_right_x, float bottom_right_y) const
{
	std::vector<int> result;

	if (upper_left_x < 0)
		upper_left_x = 0;
	if (bottom_right_x > float(width_))
		bottom_right_x = float(width_);
	if (upper_left_y < 0)
		upper_left_y = 0;
	if (bottom_right_y > float(height_))
		bottom_right_y = float(height_);

	const auto rows_count = int(ceil(double(bottom_right_y - upper_left_y) / size_.y));
	auto first_column = get_index_by_point(upper_left_x, upper_left_y);
	auto last_column = get_index_by_point(bottom_right_x, upper_left_y);
	const auto columns_per_row = width_to_size_x_;
	const auto max_column = int(cells_.size()) - 1;

	for (auto i = 0; i <= rows_count; i++)
	{
		if (last_column >= max_column)
			last_column = max_column;

		if (i == 0 || i == rows_count)
			for (auto j = first_column; j <= last_column; j++)
				result.push_back(j);
		else
		{
			result.push_back(first_column);
			result.push_back(last_column);
		}

		first_column += columns_per_row;
		last_column += columns_per_row;
	}

	return result;
}

std::vector<int> grid_list::get_blocks_in_sight(float upper_left_x, float upper_left_y, float bottom_right_x, float bottom_right_y) const
{
	std::vector<int> result;

	if (upper_left_x < 0)
		upper_left_x = 0;
	if (bottom_right_x > float(width_))
		bottom_right_x = float(width_);
	if (upper_left_y < 0)
		upper_left_y = 0;
	if (bottom_right_y > float(height_))
		bottom_right_y = float(height_);

	const auto rows_count = int(ceil(double(bottom_right_y - upper_left_y) / size_.y));
	auto first_column = get_index_by_point(upper_left_x, upper_left_y);
	auto last_column = get_index_by_point(bottom_right_x, upper_left_y);
	const auto columns_per_row = width_to_size_x_;
	const auto max_column = int(cells_.size()) - 1;

	for (auto i = 0; i <= rows_count; i++)
	{
		if (last_column >= max_column)
			last_column = max_column;

		for (auto j = first_column; j <= last_column; j++)
			result.push_back(j);

		first_column += columns_per_row;
		last_column += columns_per_row;
	}

	return result;
}

void grid_list::update_item_position(const std::string& name, const float x, const float y)
{
	auto position = items_.at(name);
	const auto item = cells_[position.first][position.second];
	cells_[position.first].erase(cells_[position.first].begin() + position.second);

	for (unsigned int i = position.second; i < cells_[position.first].size(); i++)
	{
		auto item_name = cells_[position.first][i]->get_name();
		items_.at(item_name).second -= 1;
	}
	auto index = get_index_by_point(x, y);
	position = std::make_pair(index, int(cells_[index].size()));
	items_.at(name) = position;
	cells_[index].push_back(item);
}

