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
	
	const auto vectorSize = width_to_size_x_ * height_to_size_y_;
	cells_.resize(vectorSize);
	for (auto& arr : cells_)
	{
		arr.resize(0);
	}
	micro_block_matrix.resize(4000, std::vector<bool>(4000, true));	
	distances.resize(4000, std::vector<float>(4000, 1000));
	previous.resize(4000, std::vector<std::pair<int, int>>(4000, { -1, -1 }));
}

grid_list::~grid_list()
{
	for (auto cell : cells_)
	{
		for (auto ptr : cell)
		{
			delete ptr;
		}
		cell.clear();
	}
	cells_.clear();
	items_.clear();
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
	const auto inLineNumber = width_ / int(size_.x);
	const auto inRawNumber = index / inLineNumber;

	const auto result= Vector2f(
		float(index % inLineNumber) * size_.x,
		float(inRawNumber) * size_.y);

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
	const auto inLineNumber = width_ / int(micro_size_.x);
	const auto inRawNumber = micro_block_index / inLineNumber;

	const auto result = Vector2f(
		float(micro_block_index % inLineNumber) * micro_size_.x,
		float(inRawNumber) * micro_size_.y);

	return result;
}

void grid_list::make_route(
	const Vector2f start_pos,
	const Vector2f finish_pos,
	const float upper_left_x,
	const float upper_left_y,
	const float bottom_right_x,
	const float bottom_right_y,
	const int permissible_distance)
{
	route.clear();

	if (abs(start_pos.x - finish_pos.x) / micro_size_.x + abs(start_pos.y - finish_pos.y) / micro_size_.y < 1)
		return;

	const auto xMicroBlocksCount = int(ceil((bottom_right_x - upper_left_x) / micro_size_.x));
	const auto yMicroBlocksCount = int(ceil((bottom_right_y - upper_left_y) / micro_size_.y));

	const auto startXInd = int(round(upper_left_x / micro_size_.x));
	const auto startYInd = int(round(upper_left_y / micro_size_.y));

	auto curMicroBlockX = int(round(start_pos.x / micro_size_.x));
	auto curMicroBlockY = int(round(start_pos.y / micro_size_.y));

	auto lastMicroBlockX = int(round(finish_pos.x / micro_size_.x));
	auto lastMicroBlockY = int(round(finish_pos.y / micro_size_.y));

	auto isBreak = false;
	
	if (!micro_block_matrix[curMicroBlockX][curMicroBlockY]/* || !dynamicMicroBlockMatrix->at(curMicroBlockX)[curMicroBlockY]*/)
	{
		for (auto i = -1; i <= 1; i++)
		{
			for (auto j = -1; j <= 1; j++)
				if (curMicroBlockX + i > 0 && curMicroBlockX + i < width_to_micro_x_ &&
					curMicroBlockY + j > 0 && curMicroBlockY + j < height_to_micro_y_ && 
					micro_block_matrix[curMicroBlockX + i][curMicroBlockY + j]/* && dynamicMicroBlockMatrix->at(curMicroBlockX + i)[curMicroBlockY + j]*/)
				{
					curMicroBlockX += i;
					curMicroBlockY += j;
					isBreak = true;
					break;
				}
			if (isBreak)
				break;
		}
	}

	if (!micro_block_matrix[lastMicroBlockX][lastMicroBlockY]/* || !dynamicMicroBlockMatrix->at(lastMicroBlockX)[lastMicroBlockY]*/)
	{
		for (auto i = -permissible_distance; i <= permissible_distance; i++)
		{
			isBreak = false;
			for (auto j = -permissible_distance; j <= permissible_distance; j++)
				if (lastMicroBlockX + i > 0 && lastMicroBlockX + i < width_to_micro_x_ &&
					lastMicroBlockY + j > 0 && lastMicroBlockY + j < height_to_micro_y_ && 
					micro_block_matrix[lastMicroBlockX + i][lastMicroBlockY + j]/* && dynamicMicroBlockMatrix->at(lastMicroBlockX + i)[lastMicroBlockY + j]*/)
				{
					lastMicroBlockX += i;
					lastMicroBlockY += j;
					isBreak = true;
					break;
				}
			if (isBreak)
				break;
		}
	}

	if (!micro_block_matrix[lastMicroBlockX][lastMicroBlockY])
	{
		route.clear();
		return;
	}

	for (auto i = startXInd; i < startXInd + xMicroBlocksCount; i++)	
		for (auto j = startYInd; j < startYInd + yMicroBlocksCount; j++)		
			distances[i][j] = inf;

	distances[curMicroBlockX][curMicroBlockY] = 0;
	bfs(startXInd + xMicroBlocksCount, startYInd + yMicroBlocksCount, curMicroBlockX, curMicroBlockY, lastMicroBlockX, lastMicroBlockY);
}

void grid_list::bfs(const int xBorder, const int yBorder, int startX, int startY, const int finishX, const int finishY)
{
	std::queue<std::pair<int, int>> q;
	q.push(std::make_pair(startX, startY));

	float step;
	while (!q.empty()) 
	{
		const auto v = q.front();
		q.pop();
		auto isBreak = false;

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
				if (to.first < 0 || to.first > xBorder || to.second < 0 || to.second > yBorder)
					continue;

				if (abs(previous[v.first][v.second].first - v.first) + abs(previous[v.first][v.second].second - v.second) <= 2 &&
					v.first - previous[v.first][v.second].first == to.first - v.first && v.second - previous[v.first][v.second].second == to.second - v.second)
					step -= 0.0001f;


				if (distances[to.first][to.second] > distances[v.first][v.second] + step && (micro_block_matrix[to.first][to.second] != 0 && dynamic_micro_block_matrix->at(to.first)[to.second] != 0 || to.first == finishX && to.second == finishY))
				{
					distances[to.first][to.second] = distances[v.first][v.second] + step;
					previous[to.first][to.second] = v;
					q.push(to);
				}
				if (to.first == finishX && to.second == finishY)
				{
					isBreak = true;
					break;
				}
			}
		if (isBreak)
			break;
	}

	auto canCreateRoute = true;

	if (finishX == -1 || distances[finishX][finishY] == inf)
	{
		canCreateRoute = false;
		route.clear();		
	}

	if (canCreateRoute)
	{
		std::vector<std::pair<int, int>> path;

		auto currentMatrixCeil = previous[finishX][finishY];
		float iterations = 0;
		while (true)
		{
			iterations++;
			if (currentMatrixCeil == std::make_pair(-1, -1) || iterations > distances[finishX][finishY])
			{
				route.clear();
				return;
			}

			if (abs(previous[currentMatrixCeil.first][currentMatrixCeil.second].first - currentMatrixCeil.first) + abs(previous[currentMatrixCeil.first][currentMatrixCeil.second].second - currentMatrixCeil.second) > 2)
			{
				route.clear();
				return;
			}

			path.push_back(currentMatrixCeil);			
			currentMatrixCeil = previous[currentMatrixCeil.first][currentMatrixCeil.second];
			if (currentMatrixCeil == std::make_pair(startX, startY))
				break;
		}
		if (!path.empty())
			reverse(path.begin(), path.end());

		if (!path.empty() && path[0] == std::make_pair(startX, startY))
			path.erase(path.begin() + 0);

		// cut corners
		auto cnt = 0;
		while (cnt < int(path.size() - 3))
			if (path[cnt + 1].first - path[cnt].first == path[cnt + 2].first - path[cnt + 1].first && path[cnt + 1].second - path[cnt].second == path[cnt + 2].second - path[cnt + 1].second)
				path.erase(path.begin() + cnt);
			else
				cnt++;
		//------------

		route = path;
	}
}

bool grid_list::is_intersect_with_others(world_object* object, const std::vector<world_object*>& visibleTerrain, bool isDotAdjusted) const
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

	std::map<std::pair<int, int>, bool> checkBlocks = {};

	const auto i_start = int(round((object->getPosition().x - object->getMicroBlockCheckAreaBounds().x) / micro_size_.x));
	const auto i_end = int(round((object->getPosition().x + object->getMicroBlockCheckAreaBounds().x) / micro_size_.x));

	const auto j_start = int(round((object->getPosition().y - object->getMicroBlockCheckAreaBounds().y) / micro_size_.y));
	const auto j_end = int(round((object->getPosition().y + object->getMicroBlockCheckAreaBounds().y) / micro_size_.y));

	for (auto i = i_start; i < i_end; i++)
		for (auto j = j_start; j < j_end; j++)
			if (!(i < 0 || i > width_to_micro_x_ || j < 0 || j > height_to_micro_y_) && !micro_block_matrix[i][j])
				checkBlocks[{i, j}] = true;

	return object->isLockedPlace(checkBlocks);
}

void grid_list::set_locked_micro_blocks(world_object* item, const bool value, const bool dynamic_matrix)
{
	const auto worldItem = dynamic_cast<world_object*>(item);
	if (worldItem)
		for (const auto block : worldItem->getLockedMicroBlocks())
		{
			if (!(block.x < 0 || block.x > width_to_micro_x_ || block.y < 0 || block.y > height_to_micro_y_))
			{
				if (dynamic_matrix)
					dynamic_micro_block_matrix->at(block.x)[block.y] = value;
				else
					micro_block_matrix[block.x][block.y] = value;
			}
		}
}

void grid_list::add_item(world_object* item, const std::string& name, const float x, const float y)
{
	set_locked_micro_blocks(item);

	if (items_.find(name) != items_.end())
		throw std::invalid_argument("The key " + name + " already exists in the Grid.");

	auto index = get_index_by_point(x, y);	

	auto position = std::make_pair(index, int(cells_[index].size()));

	cells_[index].push_back(item);
	
	items_.insert({ name, position });
}

void grid_list::clear_cell(const int cell_index)
{
	for (auto& item : cells_[cell_index])
	{
		set_locked_micro_blocks(item, true);
		items_.erase(items_.find(item->getName()));		
	}
	cells_[cell_index].clear();
}

void grid_list::delete_item(const std::string& name)
{
	const auto position = items_.at(name);
	set_locked_micro_blocks(cells_[position.first][position.second], true);
	
	for (unsigned int i = position.second; i < cells_[position.first].size(); i++)
	{
		const auto itemToUpdate = dynamic_cast<world_object*>(cells_[position.first][i]);
		auto itemName = itemToUpdate->getName();
		items_.at(itemName).second -= 1;
	}
	
	cells_[position.first].erase(cells_[position.first].begin() + position.second);
	items_.erase(items_.find(name));
}

world_object* grid_list::get_item_by_name(const std::string& name)
{
	const auto position = items_.at(name);
	return cells_[position.first][position.second];
}

std::vector<world_object*> grid_list::get_items(const int block_index)
{
	return cells_[block_index];
}

std::vector<world_object*> grid_list::get_items(float upper_left_x, float upper_left_y, float bottom_right_x, float bottom_right_y)
{
	std::vector<world_object*> result = {};
	
	if (upper_left_x < 0)
	{
		upper_left_x = 0;
	}
	if (bottom_right_x > float(width_))
	{
		bottom_right_x = float(width_);
	}
	if (upper_left_y < 0)
	{
		upper_left_y = 0;
	}
	if (bottom_right_y > float(height_))
	{
		bottom_right_y = float(height_);
	}
	const auto rowsCount = int(ceil(double(bottom_right_y - upper_left_y) / int(size_.y)));
	auto firstColumn = int(get_index_by_point(upper_left_x, upper_left_y));
	auto lastColumn = int(get_index_by_point(bottom_right_x, upper_left_y));
	const auto columnsPerRow = width_to_size_x_;
	const auto maxColumn = int(cells_.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		if (lastColumn > maxColumn)
			lastColumn = maxColumn;
		
		for (auto j = firstColumn; j <= lastColumn; j++)
		{
			result.insert(result.end(), cells_[j].begin(), cells_[j].end());
		}

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
	}

	return result;
}

std::vector<int> grid_list::get_blocks_around(float upperLeftX, float upperLeftY, float bottomRightX, float bottomRightY) const
{
	std::vector<int> result;

	if (upperLeftX < 0)
		upperLeftX = 0;
	if (bottomRightX > float(width_))
		bottomRightX = float(width_);
	if (upperLeftY < 0)
		upperLeftY = 0;
	if (bottomRightY > float(height_))
		bottomRightY = float(height_);

	const auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / size_.y));
	auto firstColumn = get_index_by_point(upperLeftX, upperLeftY);
	auto lastColumn = get_index_by_point(bottomRightX, upperLeftY);
	const auto columnsPerRow = width_to_size_x_;
	const auto maxColumn = int(cells_.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		if (i == 0 || i == rowsCount)
			for (auto j = firstColumn; j <= lastColumn; j++)
				result.push_back(j);
		else
		{
			result.push_back(firstColumn);
			result.push_back(lastColumn);
		}

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
	}

	return result;
}

std::vector<int> grid_list::get_blocks_in_sight(float upperLeftX, float upperLeftY, float bottomRightX, float bottomRightY) const
{
	std::vector<int> result;

	if (upperLeftX < 0)
		upperLeftX = 0;
	if (bottomRightX > float(width_))
		bottomRightX = float(width_);
	if (upperLeftY < 0)
		upperLeftY = 0;
	if (bottomRightY > float(height_))
		bottomRightY = float(height_);

	const auto rowsCount = int(ceil(double(bottomRightY - upperLeftY) / size_.y));
	auto firstColumn = get_index_by_point(upperLeftX, upperLeftY);
	auto lastColumn = get_index_by_point(bottomRightX, upperLeftY);
	const auto columnsPerRow = width_to_size_x_;
	const auto maxColumn = int(cells_.size()) - 1;

	for (auto i = 0; i <= rowsCount; i++)
	{
		if (lastColumn >= maxColumn)
			lastColumn = maxColumn;

		for (auto j = firstColumn; j <= lastColumn; j++)
			result.push_back(j);

		firstColumn += columnsPerRow;
		lastColumn += columnsPerRow;
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
		const auto itemToUpdate = dynamic_cast<world_object*>(cells_[position.first][i]);
		auto itemName = itemToUpdate->getName();
		items_.at(itemName).second -= 1;
	}
	auto index = get_index_by_point(x, y);
	position = std::make_pair(index, int(cells_[index].size()));
	items_.at(name) = position;
	cells_[index].push_back(item);
}

